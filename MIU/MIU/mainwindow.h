#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "Node.h"
#include "Rule.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

	static const int maxLength = 20;

	Node *m_head;
	QVector<Rule*> m_rules;
	QSet<QString> m_theorems;

	void doJob(Node* node);
	void display(Node* node, QTreeWidgetItem* item);
};

#endif // MAINWINDOW_H
