#include <QFile>
#include <QPair>

#include "msvreader.h"

bool msvReader::OpenFile(char *szFileName)
{
	if(file.isOpen()) file.close();
	file.setFileName(QString(szFileName));
	file.open(QFile::ReadOnly);
	msvFileHeader fileHeader;
	file.read((char *)&fileHeader, sizeof(fileHeader));
	if(fileHeader.msvMagic != msvMagic) {fprintf(stderr, "\nFile %s not supported", szFileName); return false;}
	totalPlaytime_ms = fileHeader.totalPlaytime_ms;
	buildIndex();//»ндекс файла дл€ ускорение поиска

	return true;
}

unsigned int msvReader::GetTotalPlaytime_ms()
{
	if(file.isOpen()) return totalPlaytime_ms;
	return 0;
}

void msvReader::buildIndex()
{
	unsigned int oldPos = file.pos();
	fileIndexes.clear();

	file.seek(sizeof(msvFileHeader));//ѕереместимс€ в начало данных
	msvFrameHeader *header;
	//unsigned char buffer[bufferSize];//ѕочему это не работает о____ќ
	unsigned char *buffer = new unsigned char[bufferSize];
	unsigned int actRead = 0;
	unsigned int frameN = 0;
	while(actRead = file.read((char *)buffer, bufferSize))
	{
		header = (msvFrameHeader *)buffer;
		while((((unsigned char*) header) - buffer) + header->frameSize + sizeof(msvFrameHeader) <= actRead)
		{//ѕока рассто€ние от начала буфера до текущего заголовка + размер када + размер заголовка меньше длины буфера
			//если номер текущего кадра делетс€ на цело на {количество кадров на индекс}, то добавл€ем пару номер/индекс
			if(!(frameN++ % framesPerIndex))//Ќулевой кадр тоже будет..
				{
					IndexPair pair;
					pair.time_ms = header->time_ms;
					pair.filePos = file.pos() - actRead + (((unsigned char*) header) - buffer);
//					MY_DEBUG printf("\nIndex added: for time %d at %d", pair.time_ms, pair.filePos);
					fileIndexes.append(pair);
				}
			header = (msvFrameHeader *)( ((unsigned char *)header) + header->frameSize + sizeof(msvFrameHeader));
		}
		//ѕереместим указатель в файле к заголовку кадра, который не влез (или не переместим, если всЄ влезло)
		file.seek(file.pos() - actRead + (((unsigned char*) header) - buffer));

	}
	file.seek(oldPos);
	delete [] buffer;
}

void msvReader::run()
{
	while(!file.atEnd())
	{
		totalRead = file.read((char *)buffer, bufferSize);
		//if(totalRead == 0) {/*Pause(true); */return;}/* онец воспроизведени€*/
		header = (msvFrameHeader *)buffer;
		buffPos = buffer;
		unsigned int nextFramePos = sizeof(msvFrameHeader) + header->frameSize;

		if(nextFramePos > totalRead)
		{
			fprintf(stderr, "r\nreader: frame didn't fit in buffer! Size: %d, filePos: %d, read: %d", header->frameSize, file.pos(), totalRead);
			file.seek(file.pos() - totalRead + nextFramePos);
			return;
		}
		bool lastFrame = false;
		while(!lastFrame)
		{
			pauseMutex.lock();
			seekMutex.lock();
			nextFramePos = (buffPos - buffer) + sizeof(msvFrameHeader) + header->frameSize;

			if(nextFramePos + sizeof(msvFrameHeader) < totalRead)	//≈сли в буфер влез заголовок следующего кадра
			{
				msvFrameHeader *tmpHeader = (msvFrameHeader *)(buffPos + sizeof(msvFrameHeader) + header->frameSize);
				if(nextFramePos + sizeof(msvFrameHeader) + tmpHeader->frameSize > totalRead)// но сам кадр не влез
				{
					lastFrame = true;
				}
			}
			else	//или если даже заголовок следующего кадра не поместилс€
			{
				lastFrame = true;
			}

			if(decoders->contains(header->stream))
			{
				unsigned char *frame = new unsigned char[header->frameSize];
				memcpy(frame, ((char *)header) + sizeof(msvFrameHeader), header->frameSize);
				decoders->value(header->stream).decoder->Enqueue(CodedBuff(frame, 0, header->frameSize, 0, header->time_ms, (unsigned int)buffer));
				//decoders->value(header->stream).decoder->Enqueue(CodedBuff((unsigned char *) header, 0, header->frameSize, 0, header->time_ms, (unsigned int)buffer));
			}
			buffPos += sizeof(msvFrameHeader) + header->frameSize;
			header = (msvFrameHeader *)buffPos;
			seekMutex.unlock();
			pauseMutex.unlock();

		}
		file.seek(file.pos() - (totalRead - (buffPos - buffer)));//ѕереместимс€ на начало кадра, который не влез
		//MY_DEBUG printf("\nreader: buff update done");
		//MY_DEBUG printf("fileptr moved %d\n",-(totalRead - (buffPos - currentBuffer)));
	}
}

void msvReader::Seek(unsigned int time_ms)
{
	MY_DEBUG printf("\nWe are in Seek(int)");
	if(time_ms >= totalPlaytime_ms) return;
	//ѕереместим указатели в файле
	for(int i = 1; i < fileIndexes.count(); ++i)
	{
		if(fileIndexes.at(i).time_ms > (unsigned int)time_ms)
		{
			file.seek(fileIndexes.at(i-1).filePos);
			MY_DEBUG printf("\nIndex for %d ms at %d chosen", fileIndexes.at(i-1).time_ms, fileIndexes.at(i-1).filePos);
			break;
		}
		if(i == fileIndexes.count() - 1)
		{
			//“ут окажемс€, если нам нужен самый последний индекс
			file.seek(fileIndexes.at(i).filePos);
			MY_DEBUG printf("\nIndex for %d ms at %d chosen", fileIndexes.at(i).time_ms, fileIndexes.at(i).filePos);
		}
	}
	//unsigned char buffer[bufferSize];v___v
	unsigned char *tmpBuff = new unsigned char[bufferSize];
	unsigned int actRead = 0;
	msvFrameHeader *tmpHeader;
	while(actRead = file.read((char *)tmpBuff, bufferSize))
	{
		tmpHeader = (msvFrameHeader *)tmpBuff;
		while((((unsigned char*) tmpHeader) - tmpBuff) + tmpHeader->frameSize + sizeof(msvFrameHeader) < actRead)
		{//ѕока рассто€ние от начала буфера до текущего заголовка + размер када + размер заголовка меньше длины буфера
			if(tmpHeader->time_ms >= time_ms/* && header->keyFrame == 1*/)
			{//”становимс€ на кадр со следующим временем
				file.seek(file.pos() - actRead + (((unsigned char*) tmpHeader) - tmpBuff));

				seekMutex.lock();
				totalRead = file.read((char *)buffer, bufferSize);
				header = (msvFrameHeader *)buffer;
				buffPos = buffer;
				seekMutex.unlock();

				MY_DEBUG printf("\nNext frame time is %d", header->time_ms);
				MY_DEBUG printf("\nSeek: filePos is %d", (int)file.pos());
				delete [] tmpBuff;
				return;
			}
			tmpHeader = (msvFrameHeader *)( ((unsigned char *)tmpHeader) + tmpHeader->frameSize + sizeof(msvFrameHeader) );
		}
		//ѕереместим указатель в файле к заголовку кадра, который не влез (или не переместим, если всЄ влезло)
		file.seek(file.pos() - actRead + (((unsigned char*) tmpHeader) - tmpBuff));
	}
	delete [] tmpBuff;
}

