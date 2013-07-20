#ifndef JP2DSPWIDGET_H
#define JP2DSPWIDGET_H

#define OPENGL

#include <QGLWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QQueue>

#include <windows.h>

#include "../io/msvwriter.h"
#include "../frogavcodec/include/ffmpeg/swscale.h"

class JP2DecThread;
class Frame;

class JP2DspWidget : public QGLWidget, public IAVFrameRenderer
{
	Q_OBJECT
public:
	JP2DspWidget(const QGLFormat& format, QWidget* parent=0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f=0);
	virtual ~JP2DspWidget();

    QSize minimumSizeHint() const {return QSize(100, 100);};
    QSize sizeHint() const {return QSize(1408, 1050);};

    StreamType stream;

signals:
	//void Rendered();
	void Destroing(StreamType);//Посылается из начала деструктора, чтобы сообщить плееру

public slots:
	/*Возвращает true, если свободен, иначе false и рендерить не будет*/
	void Render(Frame *frame);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

#ifndef OPENGL
    virtual void paintEvent(QPaintEvent *e)
    {
    	QPainter painter(this);
    	painter.begin(this);
      //paintGL();
    	painter.drawImage(0, 0, QImage(image, renderW, renderH, QImage::Format_RGB32));
  //    emit repaint2D(painter, e);
    	painter.end();
    }
#endif

private:
	static const QThread::Priority priority = QThread::LowPriority;

	unsigned int renderW;	//Размер изображения, посылаемого в видеокарту
	unsigned int renderH;
	unsigned char *image;
	unsigned int curImageAlloc;

	int resReduction;
//	QMutex decFinishedMutex;
//	int threadsFinished;

	LARGE_INTEGER timerFreq;
	LARGE_INTEGER lastTimerVal;
	int framesRendered;
	int frN;
	int FramesDropped;

//	QWaitCondition localWC;
	SwsContext *swsContext;

	QQueue<Frame *> renderQueue;
	QMutex renderQueueMutex;
};

#endif /*JP2DSPWIDGET_H*/
