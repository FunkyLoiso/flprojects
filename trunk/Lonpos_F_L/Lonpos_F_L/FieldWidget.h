#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>

#include "Figure.h"

/**	���������� ��������� ������, �������������� ������, � ��� �� ������� ��������� ������ � ��������� � �������.
  *	�� ������������ ������� ��������.		*/
class FieldWidget : public QWidget
{
	Q_OBJECT

public:
	enum LayoutMode
	{
		MODE_VERTICAL,	///< ���� �������� ��� ������ �������, ����� ��������� ������������ ������ ���������
		MODE_FIT		///< ���� ������� ���������� � ������, ������ ��������� �� ����� ���������
	};
	FieldWidget(QWidget *parent = NULL);
	~FieldWidget();

	void setFieldWidth(int width);
	int fieldWidth() const;
	void setFieldHeight(int height);
	int fieldHeight() const;

	void setLayoutMode(LayoutMode mode);
	void setFigures(Figure::list* figures);	///< ������ ������ ������������ �����
	void setFieldConfiguration(FieldPlace::list* fieldConfiguration); ///< ������ ������������ ���� ��� ����������� ������ ����� �� ����

	/**	������ �������������� ������, ������� ����� ������������ ������ ������.
	  *	��� ���������� ����������� ������ �� ����� ������� ����� ��� ��. */
	void setOverlayFigure(const Figure& overlayFigure);
	void removeOverlayFigure();

	/**	������ ������, ����������� ������� ����� ��������� �� ��������. */
	void setTrackingFigure(const Figure&);
	void removeTrackingFigure();

signals:
	void lmbClicked(FieldPlace place);	///< ������������ ������� ����� ������� �� �������� ���� place
	void rmbClicked();	///< ������������ ������� ������ �������
	void rotated(bool clockwise);	///< ������������ ����������� �������
	void removed();	///< ������������ ����������� �������� ������ � ����

private:
	LayoutMode m_layoutMode;
	Figure::list* m_figures;
	Figure m_overlayFigure;
	Figure m_trackingFigure;
	FieldPlace::list* m_fieldConf;
	QPoint m_mouseTrackingPos;
	double m_cellSize;
	int m_fieldWidth;
	int m_fieldHeight;

	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void keyPressEvent(QKeyEvent *);
	virtual void paintEvent(QPaintEvent *);
	virtual QSize sizeHint() const;
};

#endif // FIELDWIDGET_H
