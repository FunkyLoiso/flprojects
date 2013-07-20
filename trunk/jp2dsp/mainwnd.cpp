#include <QFile>
#include <QSlider>
#include <QButtonGroup>
#include <QPushButton>
#include <QFileDialog>

#include "mainwnd.h"
#include "renderers/jp2dspwidget.h"
#include "renderers/sounddevice.h"
#include "J2K-Codec/j2k-codec.h"
#include "convertdialog.h"

MainWnd::MainWnd(QWidget *parent) : QMainWindow(parent), settings("OKB_Sukhogo", "spoi_video"), player(0)
{
	ui.setupUi(this);
	actionPlay = ui.toolBar->addAction("play", this, SLOT(play()));
//	actionNext = ui.toolBar->addAction("next", player, SLOT(NextFrame()));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	connect(ui.actionFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionSPOIFolderOpen, SIGNAL(triggered()), this, SLOT(openSPOIFolder()));
	connect(ui.actionSPOIDeviceOpen, SIGNAL(triggered()), this, SLOT(openSPOIDevice()));
	connect(ui.actionConvert, SIGNAL(triggered()), this, SLOT(convert()));
	connect(ui.actionAviFolderOpen, SIGNAL(triggered()), this, SLOT(openAVIFolder()));
	slider = new QSlider(Qt::Horizontal, this);
//	slider->setTracking(false);
	connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
	ui.toolBar->addWidget(slider);

	setCentralWidget(&mdiArea);
	mdiArea.tileSubWindows();

	fmt.setSwapInterval(0);

	video0 = new JP2DspWidget(fmt, this);
	video1 = new JP2DspWidget(fmt, this);
	video2 = new JP2DspWidget(fmt, this);
	video3 = new JP2DspWidget(fmt, this);
	video4 = new JP2DspWidget(fmt, 0);
	video5 = new JP2DspWidget(fmt, 0);
	snd = new SoundDevice();

	mdiArea.addSubWindow(video0);
	mdiArea.addSubWindow(video1);
	mdiArea.addSubWindow(video2);
	mdiArea.addSubWindow(video3);
	//mdiArea.addSubWindow(video4);
	//mdiArea.addSubWindow(video5);

	video0->stream = VideoDataChannel_0;
	video1->stream = VideoDataChannel_1;
	video2->stream = VideoDataChannel_2;
	video3->stream = VideoDataChannel_3;
	video4->stream = VideoDataChannel_4;
	video4->stream = VideoDataChannel_5;


}

MainWnd::~MainWnd()
{
	player->TerminateDispatchers();//Не освобождает буферы, чтобы JP2DspWidget-ы спокойно закрылись

	if(video0) delete video0;	//Мы можем так проверять потому, что используется QPointer
	if(video1) delete video1;	//Рендереров нужно удалить до удаления плеера, ибо они используют буфер из него
	if(video2) delete video2;
	if(video3) delete video3;
	if(video4) delete video4;
	if(video5) delete video5;
	delete snd;
	delete player;//Буферы освобождаются здесь
}

void MainWnd::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите файл"), settings.value("lastMSV", "C:\\").toString(), "Поддерживаемые видеофайлы (*.msv)");
	if(fileName.isEmpty()) return;

	ui.statusBar->showMessage("Файл обрабатывается...");
	qApp->setOverrideCursor(Qt::WaitCursor);

	createPlayer(MSV_Mode);

	//player->OpenFile(fileName.toAscii().data());
	player->OpenFile("D:\\AVI\\");
	slider->setMaximum(player->GetTotalPlaytime_ms());

	ui.statusBar->showMessage("Готово", 2000);
	qApp->restoreOverrideCursor();
	settings.setValue("lastMSV", fileName.section('/', 0, -2));
}

void MainWnd::openSPOIFolder()
{
//	QString folderName = QFileDialog::getOpenFileName(this, tr("Выберите файл"), settings.value("lastDir", "C:\\").toString(), "Поддерживаемые видеофайлы (*.msv)");
	QString folderName = QFileDialog::getExistingDirectory(this, "Укажите директорию с данными", settings.value("lastSPOI", "C:\\").toString(), QFileDialog::ShowDirsOnly);
	if(!folderName.isEmpty())
	{
		ui.statusBar->showMessage("Данные обрабатываются...");
		qApp->setOverrideCursor(Qt::WaitCursor);

		createPlayer(SPOI_Mode);

		player->OpenFile(folderName.toAscii().data());
		slider->setMaximum(player->GetTotalPlaytime_ms());

		ui.statusBar->showMessage("Готово", 2000);
		qApp->restoreOverrideCursor();
		settings.setValue("lastSPOI", folderName.section('/', 0, -1));
	}
}

void MainWnd::openSPOIDevice()
{
	ui.statusBar->showMessage("Данные обрабатываются...");
	qApp->setOverrideCursor(Qt::WaitCursor);

	createPlayer(SPOI_Mode);

	player->OpenFile(0);
	slider->setMaximum(player->GetTotalPlaytime_ms());

	ui.statusBar->showMessage("Готово", 2000);
	qApp->restoreOverrideCursor();
}

void MainWnd::openAVIFolder()
{
	QString folderName = QFileDialog::getExistingDirectory(this, "Укажите директорию с данными", settings.value("lastAVI", "C:\\").toString(), QFileDialog::ShowDirsOnly);
	if(!folderName.isEmpty())
	{
		ui.statusBar->showMessage("Данные обрабатываются...");
		qApp->setOverrideCursor(Qt::WaitCursor);

		createPlayer(AVI_Mode);

		player->OpenFile(folderName.toAscii().data());
		slider->setMaximum(player->GetTotalPlaytime_ms());

		ui.statusBar->showMessage("Готово", 2000);
		qApp->restoreOverrideCursor();
		settings.setValue("lastAVI", folderName.section('/', 0, -1));	
	}
}

void MainWnd::convert()
{
	ConvertDialog(this).exec();
}

void MainWnd::play()
{
	static bool pause = true;
	pause = !pause;
	player->Pause(pause);
	ui.toolBar->actions()[0]->setText(pause ? "play" : "pause");
	//QTimer::singleShot(pause ? 5000 : 300, this, SLOT(startClick()));
}

void MainWnd::sliderReleased()
{
	player->Seek(slider->value());
}

void MainWnd::ProgressValChanged(int val)
{
	if(!slider->isSliderDown()) slider->setValue(val);
}

//void MainWnd::StreamSwitchClicked(QAbstractButton *button)
//{
//	StreamType stream = (StreamType)button->text().toInt();
//	if(button->isChecked())
//	{//Создаём рендерер
//		renderers[stream] = new JP2DspWidget(fmt);
//		renderers[stream]->stream = stream;
//		mdiArea.addSubWindow(renderers[stream]);
//		mdiArea.cascadeSubWindows();
//		player->RegisterRenderer(renderers[stream], stream);
//		connect(renderers[stream], SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)));
//	}
//	else
//	{//уничтожаем
//		if(renderers[stream]) delete renderers[stream];
//	}
//}

void MainWnd::createPlayer(PlayerMode mode)
{
	if(player) delete player;
	player = new Player(mode);

	player->RegisterRenderer(video0, VideoDataChannel_0);
	player->RegisterRenderer(video1, VideoDataChannel_1);
	player->RegisterRenderer(video2, VideoDataChannel_2);
	player->RegisterRenderer(video3, VideoDataChannel_3);
	player->RegisterRenderer(video4, VideoDataChannel_4);
	player->RegisterRenderer(video4, VideoDataChannel_5);
	player->RegisterRenderer(snd, SoundDataID);

	connect(video0, SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)), Qt::DirectConnection);
	connect(video1, SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)), Qt::DirectConnection);
	connect(video2, SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)), Qt::DirectConnection);
	connect(video3, SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)), Qt::DirectConnection);
	connect(video4, SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)), Qt::DirectConnection);
	connect(video5, SIGNAL(Destroing(StreamType)), player, SLOT(TerminateDispatcher(StreamType)), Qt::DirectConnection);

	connect(player, SIGNAL(SetProgressVal(int)), this, SLOT(ProgressValChanged(int)));

	switch(mode)
	{
	case MSV_Mode: setWindowTitle("player in MSV_mode"); break;
	case SPOI_Mode: setWindowTitle("player in SPOI_mode"); break;
	case AVI_Mode: setWindowTitle("player in AVI_mode"); break;
	}
}

