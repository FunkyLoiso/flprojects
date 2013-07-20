#ifndef CONVERTDIALOG_H
#define CONVERTDIALOG_H

#include <QSettings>

#include "ui_ConvertDialog.h"

class ConvertDialog : public QDialog
{
	Q_OBJECT
public:
	ConvertDialog(QWidget *parent = 0);

	Ui::Dialog ui;
	QSettings settings;
	QString src, dst;

private slots:
	void openSrc();
	void openDst();
	void go();
};

#endif /* CONVERTDIALOG_H */
