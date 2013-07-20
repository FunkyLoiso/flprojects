// SpoiInterface35.h : header file for use the DataSource DLL
//

#pragma once

struct SSummaryInfo {			// Суммарная информация по всему видеофайлу
	unsigned int nFrame;		// Общее количкство кадров данных
	unsigned int playbackTime;	// Время воспроизведения в тиках
};

enum StreamType { // Определение типов данных (идентификаторы потока) внутри кадра
	UndefinedDataID = 0,
	SoundDataID,
	VideoDataChannel_0,
	VideoDataChannel_1,
	VideoDataChannel_2,
	VideoDataChannel_3,
	VideoDataChannel_4,
	VideoDataChannel_5,
	NumDataTypes
};

struct SFrame { // Структура, описывающая данные одного на определенный момент времени

	unsigned int time;	// Время кадра в тиках
	StreamType id;		// Идентификатор потока
	void *frameData;	// Данные кадра, сжатые по JPEG2000, или звуковые данные	
	int size;			// Размер данных в байтах
};

class CSpoiInterface {

public:

	virtual bool openFile (const char *fileName) = 0;
	virtual bool openDevice() = 0;
	virtual void close() = 0;

	// Заполняет структуру outSummaryInfo суммарной информацией по всей записи
	virtual bool getInfo(SSummaryInfo & outSummaryInfo) = 0;

	// Заполняет структуру outFrame данными из frameIndex кадра
	virtual bool getFrame(unsigned int frameIndex, SFrame &outFrame) = 0;

	// Находит номер кадра, ближайший к заданному времени
	virtual int findFrame(unsigned int time) = 0;
};


