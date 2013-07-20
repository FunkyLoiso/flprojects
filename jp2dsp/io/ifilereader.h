#ifndef IFILEREADER_H
#define IFILEREADER_H

#include <QThread>
#include <QMap>
#include <QMutex>

#include "../commonclasses/common.h"

class IFileReader : public QThread
{
public:
	Q_OBJECT
	public:
		IFileReader(QMap<StreamType, DecoderPair> *decoders) : decoders(decoders), isPaused(false){};
		virtual bool OpenFile(char *szFileName) = 0;
		virtual void Seek(unsigned int time_ms) = 0;
		virtual unsigned int GetTotalPlaytime_ms() = 0;
		virtual void Pause(bool pause) = 0;

		bool isPaused;

	signals:
		void EndReached();

	protected:
		struct IndexPair
		{
			unsigned int time_ms;
			unsigned int filePos;
		};
		QMap<StreamType, DecoderPair> *decoders;
};

#endif /*IFILEREADER_H*/
