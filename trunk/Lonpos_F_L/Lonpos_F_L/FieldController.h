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

	void addSpareFigure(const Figure& spareFigure);	///< �������� ������ � ������ ��������� � ��������� �� ����
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

private slots:
	void onSpareFiguresLmbClicked(FieldPlace place);
	void onSpareFiguresRmbClicked();
	void onFieldLmbClicked(FieldPlace place);
	void onFieldRmbClicked();
	void onFieldRotated();	///< ������� ��������� ��������� ������
	void onFieldRemoved();	///< ������� ������� ��������� ������
};
#endif // FieldController_h__
