#ifndef MAINWND_H
#define MAINWND_H

#include <QtGui/QMainWindow>
#include <QMdiArea>
#include <QGLFormat>
#include <QGLContext>
#include <QPointer>
#include <QSettings>

#include "ui_mainwnd.h"
#include "commonclasses/player.h"

class JP2DspWidget;
class SoundDevice;
class QSlider;

class MainWnd : public QMainWindow
{
    Q_OBJECT

public:
    MainWnd(QWidget *parent = 0);
    ~MainWnd();

private:
	void AddStreamSwitches();		//Создаёт меню с кнопками для разных потоков данных

	QMdiArea mdiArea;
	QSlider *slider;
	QAction *actionPlay, *actionNext;
    Ui::MainWndClass ui;
    QSettings settings;

//    QPointer<JP2DspWidget> renderers[UndefinedDataID];//QPointer обнуляется, если объект удалился
    QPointer<JP2DspWidget> video0, video1, video2, video3, video4, video5;
    SoundDevice *snd;
    QGLFormat fmt;
    QGLContext *glContext;
    Player *player;

    void createPlayer(PlayerMode mode);

private slots:
	void openFile();
	void openSPOIFolder();
	void openSPOIDevice();
	void openAVIFolder();
	void convert();

   	void play();
   	void sliderReleased();
//   	void StreamSwitchClicked(QAbstractButton *);
   	void ProgressValChanged(int);
};

#endif // MAINWND_H
