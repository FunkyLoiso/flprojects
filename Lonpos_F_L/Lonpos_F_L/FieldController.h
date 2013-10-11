#ifndef FieldController_h__
#define FieldController_h__

#include <QObject>
#include "Figure.h"
#include "FieldWidget.h"

/**	���������� ����.
  *	�������� ��� ������, ��� ����������� �� ����, ��� � ��������� ��� �������. ��������� ������������
  *	������� ����� � ����, ��������� ������������ ������ � �������� ������� �������. */
class FieldController : QObject
{
	Q_OBJECT
public:
	FieldController();

	void addSpareFigure(Figure spareFigure);	///< �������� ������ � ������ ��������� � ��������� �� ����
	void addFieldFigure(const Figure& fieldFigure);	///< �������� ������ � ������ ������������� �� ����

	/**	������ ������������ ���� ����� ������. ������������ ������ ������������ �������� '_', ���������� - 'O'.
	  *	������ ��������� �������� '\n'. ������:
	  *	QString field =	"__O\n"
						"_OOO\n"
						"OOOO\n"
						"OOO\n"
						"_O"			*/
	void setFieldConfiguration(const QString& configuration);

	void connectSpareFiguresWidget(FieldWidget* spareFiguresWidget);///< ��������� � �������� ��������� ��� ���������� �����
	void disconnectSpareFiguresWidget();
	void connectFieldWidget(FieldWidget* fieldWidget);	///< ��������� � �������� ����
	void disconnectFieldWidget();

public slots:
	void onSpareFiguresLmbClicked(FieldPlace place);	///< ������ ����� ������� �� �������� place ����
	void onFieldLmbClicked(FieldPlace place);			///< ������ ����� ������� �� �������� place ������ ���������������� �����
	void onCancelSelection();
	void onFieldRotated(bool clockwise);	///< ������� ��������� ��������� ������
	void onFieldRemoved();	///< ������� ������� ��������� ������

private:
	Figure::list m_fieldFigures;
	Figure::list m_spareFigures;
	int m_spareFiguresFieldWidth, m_spareFiguresFieldHeight;
	FieldPlace::list m_fieldConf;
	int m_fieldWidth, m_fieldHeight;

	FieldWidget* m_spareFiguresWidget;
	FieldWidget* m_fieldWidget;

	Figure m_fieldFigureSelected;
	Figure m_spareFigureSelected;

	void clearFieldSelection();
	void clearSpareSelection();

	bool canBePlaced(const Figure& figureToPlace, FieldPlace newPlace) const;
	Figure::list::Iterator getFigureInPlace(FieldPlace place);
	void updateSpareFiguresList();
};
#endif // FieldController_h__
