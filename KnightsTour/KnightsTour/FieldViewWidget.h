#ifndef FIELDVIEWWIDGET_H
#define FIELDVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMutex>
#include <QQueue>

#include "./common.h"

/**	������ ��� ����������� ���� � ������������������ �����. */
class  FieldViewWidget : public QWidget
{
    Q_OBJECT
public:
    FieldViewWidget(QWidget *parent = 0, int width = 8, int height = 8);

	QString GetFieldName(QPoint field);	///< �������� ��� ���� � ������������ field

public slots:
	void SetFieldSize(int height, int width);	///< �������� ����� ������ �� ����
	void ShowField(const FieldState &field);	///< �������� ����� ���� � ���������� ���

signals:
	void fieldClicked(QPoint field);	///< ������������ ������� �� ����

private:

    int m_width;	///< ����� ������ �� �����������
	int m_height;	///< ����� ������ �� ���������
    QColor backgrowndColor, usedColor, lineColor, blackFieldColor; //����� ������ ����
	float m_fieldSize;	///< ������ ������ � ��������
    FieldState m_field;	///< ������� ������� ����� �� ����
    QMutex fieldMutex;	///< ������� ��������� ��������� ����
	QMutex fieldDimsMutex;	///< ������� ��������� �������� ����

	QPixmap m_knightImage;	///< �������� � ����

    QPointF fieldPoint(unsigned int locX, unsigned int locY);	///< �������� ���������� ����������� ������ �����
    void paintEvent(QPaintEvent *);	///< ����������� �������
	void mouseReleaseEvent(QMouseEvent *);	///< ����������, ��� �� ��� ������ �� ����, � ������� ������ fieldClicked, ���� ���
};

#endif