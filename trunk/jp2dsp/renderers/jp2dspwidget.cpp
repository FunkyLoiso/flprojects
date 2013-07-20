#include <QFile>

#include "jp2dspwidget.h"
#include "../ThreadClasses/JP2DecThread.h"

#include "../commonclasses/frame.h"

#include "../GLEXT.H"

#define MIN(l, r) (l > r ? r : l)
#define MAX(l, r) (l > r ? l : r)

//extern void Install_YCbCr2RGB_Shaider();

long clip0_255(long val)
{
	if(val > 255) return 255;
	if(val < 0) return 0;
	return val;
}

JP2DspWidget::JP2DspWidget(const QGLFormat& format, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f) :
	QGLWidget(format, parent, shareWidget, f), resReduction(0), curImageAlloc(1)
{
	frN = 0;
	framesRendered = 0;
	FramesDropped = 0;

	image = (unsigned char *)malloc(1);
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&lastTimerVal);
//	setAutoBufferSwap(false);
}

void JP2DspWidget::Render(Frame *frame)
{
	renderQueueMutex.lock();
	renderQueue.enqueue(frame);
	renderQueueMutex.unlock();
	update();
//	repaint();
//	updateGL();
}

JP2DspWidget::~JP2DspWidget()
{
	emit Destroing(stream);
}

void JP2DspWidget::initializeGL()
{
#ifdef OPENGL
	//glClearColor(0.8, 0.7, 0.6, 0);
//	static GLfloat Matrix_YCbCr2RGB[16] =
//	{
//		1,		1,			1,		0,
//		0,		-0.344136,	1.773,	0,
//		1.402,	-0.714136,	0,		0,
//		0,		0,			0,		0
//	};
//	glMatrixMode(GL_COLOR);
//	glLoadMatrixf(Matrix_YCbCr2RGB);
//	glPixelTransferf(GL_GREEN_BIAS, -0.5);
//	glPixelTransferf(GL_BLUE_BIAS, -0.5);
//	Install_YCbCr2RGB_Shaider();
#endif
}

void JP2DspWidget::resizeGL(int width, int height)
{
	if(height < 1) height = 1;
	if(width < 1) width = 1;

	//Вычислим коэффициент уменьшения разрешения
//	float ratio = MIN((float)width/imageW, (float)height/imageH);
//	if(ratio > 0.75) resReduction = 0;
//	else if(ratio <= 0.75 && ratio > 0.32) resReduction = 1;
//	else if(ratio <= 0.32 && ratio > 0.2) resReduction = 2;
//	else resReduction = 3;
//	resReduction = 0;/*!!!!!!!!!!!*/

//	renderW = imageW / (1<<resReduction);
//	renderH = imageH / (1<<resReduction);
	renderW = width;
	renderH = height;

	//factor = MIN((float)width/renderW, (float)height/renderH);
//	factor = 1.0f;

#ifdef OPENGL
    glViewport(0, 0, width, height);//... v__v
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    //glOrtho(0.0f, width, -height, 0.0f, -1.0f, 1.0f);
#endif
}

void JP2DspWidget::paintGL()
{
	if(renderQueue.empty()) return;
	if(curImageAlloc < renderW * renderH * 2)
	{
		image = (unsigned char *)realloc(image, renderW * renderH * 2);
	}
	Frame *f = renderQueue.last();
	float factor = MIN((float)renderW/f->width, (float)renderH/f->height);
	int dspW = (int)(f->width*factor), dspH = (int)(f->height*factor);
	swsContext = sws_getContext(f->width, f->height, f->pixFmt, dspW, dspH, PIX_FMT_BGR565, /*SWS_PARAM_DEFAULT | */SWS_FAST_BILINEAR | SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2, NULL, NULL, NULL);

	int outLineSize = (dspW + dspW%2) * 2;//TODO: Этот жуткий кусок ещё не совсем проверен...
	//int outLineSize = dspW * 2;

	unsigned char *ptr = image;
	sws_scale(swsContext, f->data, f->linesize, 0, f->height, &ptr, &outLineSize);
	sws_freeContext(swsContext);
#ifdef OPENGL
	glPixelZoom(1.0f, -1.0f);
//	glPixelZoom(factor, -factor);

	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(0, renderH);
	glDrawPixels(dspW, dspH, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, image);
//	glFinish();
//	swapBuffers();
	if(renderQueue.size() > 1)
	{
		FramesDropped += renderQueue.size() - 1;
//		printf("frames dropped: %d", FramesDropped);
	}
#endif
//	setWindowTitle(QString("rendered %1 frames").arg(++framesRendered));
	if(++framesRendered%11 == 5)
	{
		LARGE_INTEGER tmp;
		QueryPerformanceCounter(&tmp);
		setWindowTitle(QString("%1(%2x%3): %4 fps, dropped %5").arg(stream - 2).arg(f->width).arg(f->height).arg((float)(timerFreq.QuadPart*5)/(tmp.QuadPart - lastTimerVal.QuadPart)).arg(FramesDropped));
//		setWindowTitle(QString("dropped %1").arg(FramesDropped));
		lastTimerVal = tmp;
		framesRendered = 0;
	}
	renderQueueMutex.lock();
	/*for(int i = 0; i < renderQueue.size(); ++i) delete renderQueue.at(i);*/
	Frame *frame;
	foreach(frame, renderQueue) delete frame;
	renderQueue.clear();
	renderQueueMutex.unlock();
	//this->setFocus();//Чтобы было более заметно, кто обновляется
}
