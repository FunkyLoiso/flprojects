#ifndef SPOI_TO_MSV_H
#define SPOI_TO_MSV_H

#include <QMap>
#include <QSemaphore>
#include "../commonclasses/qmultimapts.h"

#include "../io/spoireader.h"
#include "../io/msvwriter.h"
#include "../264codec/x264encoder.h"
#include "../audiocodec/audioencoder.h"

class SPOI_to_MSV : public QObject
{
	Q_OBJECT
public:
	SPOI_to_MSV();
	bool OpenFolder(char *name);
	bool SetDestenation(char *name);
	bool Convert();
	int getMaxTime_ms(){return reader->GetTotalPlaytime_ms();};

signals:
	void Progress(int time_ms);

private:
	QMap<StreamType, DecoderPair> decoders;

	SPOIReader *reader;
	msvWriter writer;
	QMap<StreamType, X264Encoder *> videoEncoders;
	AudioEncoder *audioEncoder;
	QSemaphore *outMapFree[NumDataTypes], outMapUsed;
	QMultiMapTS<int, DecodedBuff> outMap;
	bool endReached;

private slots:
	void EndReached()
	{
		endReached = true;
		printf("\nSPOI_to_MSV: endReached is true");
	};
};

#endif /* SPOI_TO_MSV_H */
