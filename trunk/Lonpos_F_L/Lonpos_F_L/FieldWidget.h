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
	FieldWidget(QWidget *parent);
	~FieldWidget();

	void setWidth(int width);
	int width() const;
	void setHeight(int height);
	int height() const;

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
};

#endif // FIELDWIDGET_H
