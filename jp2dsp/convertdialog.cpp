#include <QFileDialog>

#include "convertdialog.h"

#include "converters/spoi_to_msv.h"
#include "converters/spoi_to_avi.h"

ConvertDialog::ConvertDialog(QWidget *parent) : settings("OKB_Sukhogo", "spoi_video"), QDialog(parent)
{
	ui.setupUi(this);
	ui.progressConvertion->setValue(0);
	connect(ui.bOpenSrc, SIGNAL(clicked()), this, SLOT(openSrc()));
	connect(ui.bOpenDst, SIGNAL(clicked()), this, SLOT(openDst()));
	connect(ui.bStart, SIGNAL(clicked()), this, SLOT(go()));

	ui.lineSrc->setText(settings.value("lastSPOI", "").toString());
	ui.lineDst->setText(settings.value("lastAVI", "").toString());
}

void ConvertDialog::openSrc()
{
	src = QFileDialog::getExistingDirectory(this, "Укажите директорию с данными", settings.value("lastSPOI", "C:\\").toString(), QFileDialog::ShowDirsOnly);
	ui.lineSrc->setText(src);
}

void ConvertDialog::openDst()
{
	//dst = QFileDialog::getOpenFileName(this, tr("Выберите файл"), settings.value("lastMSV", "C:\\").toString(), "Поддерживаемые видеофайлы (*.msv)");
	dst = QFileDialog::getExistingDirectory(this, tr("Выберите директорию для avi-файлов"), settings.value("lastAVI", "C:\\").toString(), QFileDialog::ShowDirsOnly);
	ui.lineDst->setText(dst);
}

void ConvertDialog::go()
{
	//SPOI_to_MSV *conv = new SPOI_to_MSV();
	SPOI_to_AVI *conv = new SPOI_to_AVI();
	src = ui.lineSrc->text();
	dst = ui.lineDst->text();
	//src = "E:\\SPOI_video\\2008-08-27-15-21-22-484";
	//dst = "D:\\AVI";
	if(!conv->OpenFolder(src.toAscii().data())) return;
	if(!conv->SetDestenation(dst.toAscii().data())) return;
	settings.setValue("lastSPOI", src.section('/', 0, -1));	
	settings.setValue("lastAVI", dst.section('/', 0, -1));	
	ui.progressConvertion->setMaximum(conv->getMaxTime_ms());
	connect(conv, SIGNAL(Progress(int)), ui.progressConvertion, SLOT(setValue(int)));
	conv->Convert();
	printf("****************************");
	fflush(stdout);
	setWindowTitle("DONE!");
	delete conv;
}
