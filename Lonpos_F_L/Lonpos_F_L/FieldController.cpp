#include "FieldController.h"

#include <QSet>

FieldController::FieldController()
: m_spareFiguresFieldHeight(0)
, m_fieldWidget(NULL), m_spareFiguresWidget(NULL)
, m_fieldFigureSelected(NULL), m_spareFigureSelected(NULL)
{

}

void FieldController::addSpareFigure(Figure spareFigure)
{
	spareFigure.move(0, m_spareFiguresFieldHeight);
	m_spareFiguresFieldHeight += spareFigure.height()+1;
	m_spareFigures.append(spareFigure);
}

void FieldController::addFieldFigure(const Figure& fieldFigure)
{
	m_fieldFigures.append(fieldFigure);
}

void FieldController::setFieldConfiguration(const QString& configuration)
{
	QStringList lines = configuration.split('\n');
	for(int y = 0; y < lines.count(); ++y)
	{
		const QString& line = lines.at(y);
		for(int x = 0; x < line.length(); ++x)
		{
			if(line.at(x) == 'O') m_fieldConf.append(FieldPlace(x, y));
		}
	}
}

void FieldController::connectSpareFiguresWidget(FieldWidget* spareFiguresWidget)
{
	disconnectSpareFiguresWidget();
	m_spareFiguresWidget = spareFiguresWidget;
	connect(m_spareFiguresWidget, SIGNAL(lmbClicked(FieldPlace)), this, SLOT(onSpareFiguresLmbClicked(FieldPlace)));
	connect(m_spareFiguresWidget, SIGNAL(rmbClicked()), this, SLOT(onSpareFiguresRmbClicked()));

	m_spareFiguresWidget->setFigures(&m_spareFigures);
}

void FieldController::disconnectSpareFiguresWidget()
{
	if(m_spareFiguresWidget != NULL)
	{
		m_spareFiguresWidget->disconnect(this);
		m_spareFiguresWidget->setFigures(NULL);
		m_spareFiguresWidget = NULL;
	}
}

void FieldController::connectFieldWidget(FieldWidget* fieldWidget)
{
	disconnectFieldWidget();
	m_fieldWidget = fieldWidget;
	connect(m_fieldWidget, SIGNAL(lmbClicked(FieldPlace)), this, SLOT(onFieldLmbClicked(FieldPlace)));
	connect(m_fieldWidget, SIGNAL(rmbClicked()), this, SLOT(onFieldRmbClicked()));
	connect(m_fieldWidget, SIGNAL(rotated(bool)), this, SLOT(onFieldRotated(bool)));
	connect(m_fieldWidget, SIGNAL(removed()), this, SLOT(onFieldRemoved()));
}

void FieldController::disconnectFieldWidget()
{
	if(m_fieldWidget != NULL)
	{
		m_fieldWidget->disconnect(this);
		m_fieldWidget->setFigures(NULL);
		m_fieldWidget->setFieldConfiguration(NULL);
		m_fieldWidget = NULL;
	}
}

void FieldController::onSpareFiguresLmbClicked(FieldPlace place)
{
	if(m_fieldFigureSelected != m_fieldFigures.end())
	{//�������� ������ � ���� ������� � ����������������
		onFieldRemoved();//�� �� �����, ��� �������� ������ � ����
	}
	else
	{//�������� ����� ������ �� ���� � �����������������
		int lowerBound = 0;
		for(Figure::list::Iterator i = m_spareFigures.begin(); i != m_spareFigures.end(); ++i)
		{
			lowerBound += i->height()+1;
			if(lowerBound > place.y())
			{
				m_spareFigureSelected = i;

				m_fieldWidget->setTrackingFigure(*m_spareFigureSelected);
				break;
			}
		}
	}
}

void FieldController::onSpareFiguresRmbClicked()
{
	clearFieldSelection();
	clearSpareSelection();

}

void FieldController::onFieldLmbClicked(FieldPlace place)
{
	if(m_fieldFigureSelected != m_fieldFigures.end())
	{//������� ��������� ������ � ������ ����� �� ������
		if(canBePlaced(m_fieldFigureSelected, place))
		{
			m_fieldFigureSelected->setCenter(place);
			clearFieldSelection();
		}
	}
	else if(m_spareFigureSelected != m_spareFigures.end())
	{//������� ��������� ������ �� ���������������� �� ����
		if(canBePlaced(m_spareFigureSelected, place))
		{
			Figure figureToInsert(*m_spareFigureSelected);
			m_spareFigures.erase(m_spareFigureSelected);
			clearSpareSelection();
			figureToInsert.setCenter(place);
			m_fieldFigures.append(figureToInsert);
			clearFieldSelection();
		}
	}
	else
	{//����� ������ �� ����
		Figure::list::Iterator i = getFigureInPlace(place);
		if(i != m_fieldFigures.end())
		{
			m_fieldFigureSelected = i;
			m_fieldWidget->setOverlayFigure(*i);
			m_fieldWidget->setTrackingFigure(*i);
		}
	}
}

void FieldController::onFieldRmbClicked()
{
	onSpareFiguresRmbClicked();
}

void FieldController::onFieldRotated(bool clockwise)
{
	if(m_fieldFigureSelected != m_fieldFigures.end())
	{
		clockwise ? m_fieldFigureSelected->rotateCW() : m_fieldFigureSelected->rotateCCW();
		m_fieldWidget->setOverlayFigure(*m_fieldFigureSelected);
		m_fieldWidget->setTrackingFigure(*m_fieldFigureSelected);
	}
}

void FieldController::onFieldRemoved()
{
	Figure figure = *m_fieldFigureSelected;
	m_fieldFigures.erase(m_fieldFigureSelected);
	addSpareFigure(figure);

	clearFieldSelection();
}

//����� �� ������, �� ������� ��������� figureToPlace ���� ����������� � ������� � place
bool FieldController::canBePlaced(Figure::list::Iterator figureToPlace, FieldPlace newPlace) const
{
	Figure potentialFigure(*figureToPlace);
	potentialFigure.setCenter(newPlace);
	FieldPlace::list elemnets(potentialFigure.elements());//������ ��������� ����� ������

	for(Figure::list::ConstIterator i = m_fieldFigures.begin(); i != m_fieldFigures.end(); ++i)
	{//��� ������ ������ �� ����
		if(i != figureToPlace)
		{//���� ��� �� �� �� �����, ������� �� ���������� ���������

			//���� � ������� ������ ���� ����������� � �����������, �� ������� ����������
			if( !i->elements().toSet().intersect(elemnets.toSet()).isEmpty() ) return false;
		}
	}
	Q_FOREACH(FieldPlace place, elemnets)
	{//��� ������� �������� ����������� ������
		//���� ��� ���������������� �������� ����, �� ������� ����������
		if(!m_fieldConf.contains(place)) return false;
	}

	return true;
}

void FieldController::clearFieldSelection()
{
	m_fieldFigureSelected = m_fieldFigures.end();
	m_fieldWidget->removeOverlayFigure();
	m_fieldWidget->removeTrackingFigure();
}

void FieldController::clearSpareSelection()
{
	m_spareFigureSelected = m_spareFigures.end();
}

Figure::list::Iterator FieldController::getFigureInPlace(FieldPlace place)
{
	for(Figure::list::Iterator i = m_fieldFigures.begin(); i != m_fieldFigures.end(); ++i)
	{
		if(i->elements().contains(place)) return i;
	}
	return m_fieldFigures.end();
}
