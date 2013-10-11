#include "FieldController.h"

#include <QSet>

FieldController::FieldController()
: m_spareFiguresFieldWidth(0), m_spareFiguresFieldHeight(0)
, m_fieldWidget(NULL), m_spareFiguresWidget(NULL)
, m_fieldFigureSelected(NULL), m_spareFigureSelected(NULL)
, m_fieldWidth(0), m_fieldHeight(0)
{

}

void FieldController::addSpareFigure(Figure spareFigure)
{
	m_spareFigures.append(spareFigure);
	updateSpareFiguresList();
}

void FieldController::addFieldFigure(const Figure& fieldFigure)
{
	m_fieldFigures.append(fieldFigure);
}

void FieldController::setFieldConfiguration(const QString& configuration)
{
	m_fieldWidth = m_fieldHeight = 0;
	QStringList lines = configuration.split('\n');
	for(int y = 0; y < lines.count(); ++y)
	{
		const QString& line = lines.at(y);
		for(int x = 0; x < line.length(); ++x)
		{
			if(line.at(x) == 'O') m_fieldConf.append(FieldPlace(x, y));
			if(m_fieldWidth < x+1) m_fieldWidth = x+1;
			if(m_fieldHeight < y+1) m_fieldHeight = y+1;
		}
	}

	if(m_fieldWidget != NULL)
	{
		m_fieldWidget->setFieldWidth(m_fieldWidth);
		m_fieldWidget->setFieldHeight(m_fieldHeight);
	}
}

void FieldController::connectSpareFiguresWidget(FieldWidget* spareFiguresWidget)
{
	disconnectSpareFiguresWidget();
	m_spareFiguresWidget = spareFiguresWidget;
	connect(m_spareFiguresWidget, SIGNAL(lmbClicked(FieldPlace)), this, SLOT(onSpareFiguresLmbClicked(FieldPlace)));
	connect(m_spareFiguresWidget, SIGNAL(rmbClicked()), this, SLOT(onSpareFiguresRmbClicked()));

	m_spareFiguresWidget->setFigures(&m_spareFigures);
	m_spareFiguresWidget->setFieldWidth(m_spareFiguresFieldWidth);
	m_spareFiguresWidget->setFieldHeight(m_spareFiguresFieldHeight);
}

void FieldController::disconnectSpareFiguresWidget()
{
	if(m_spareFiguresWidget != NULL)
	{
		m_spareFiguresWidget->disconnect(this);
		m_spareFiguresWidget->setFigures(NULL);
		m_fieldWidget->setFieldWidth(0);
		m_fieldWidget->setFieldHeight(0);
		m_spareFiguresWidget = NULL;
	}
}

void FieldController::connectFieldWidget(FieldWidget* fieldWidget)
{
	disconnectFieldWidget();
	m_fieldWidget = fieldWidget;
	m_fieldWidget->setFieldConfiguration(&m_fieldConf);
	m_fieldWidget->setFieldWidth(m_fieldWidth);
	m_fieldWidget->setFieldHeight(m_fieldHeight);
	m_fieldWidget->setFigures(&m_fieldFigures);
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
		m_fieldWidget->setFieldWidth(0);
		m_fieldWidget->setFieldHeight(0);
		m_fieldWidget = NULL;
	}
}

void FieldController::onSpareFiguresLmbClicked(FieldPlace place)
{
	if(m_fieldFigureSelected != NULL)
	{//помещаем фигуру с поля обратно к неиспользованным
		onFieldRemoved();//то же самое, что удаление фигуры с поля
	}
	else
	{//выбираем новую фигуру из поля с неиспользованными
		int lowerBound = 0;
		for(Figure::list::Iterator i = m_spareFigures.begin(); i != m_spareFigures.end(); ++i)
		{
			lowerBound += i->height()+1;
			if(lowerBound > place.y())
			{
				m_spareFigureSelected = &*i;

				if(m_fieldWidget != NULL) m_fieldWidget->setTrackingFigure(*m_spareFigureSelected);
				break;
			}
		}
	}
}

void FieldController::onSpareFiguresRmbClicked()
{
	clearFieldSelection();
	clearSpareSelection();
	if(m_fieldWidget != NULL) m_fieldWidget->update();
}

void FieldController::onFieldLmbClicked(FieldPlace place)
{
	if(m_fieldFigureSelected != NULL)
	{//попытка перенести фигуру с одного места на другое
		if(canBePlaced(m_fieldFigureSelected, place))
		{
			m_fieldFigureSelected->setCenter(place);
			clearFieldSelection();
			if(m_fieldWidget != NULL) m_fieldWidget->update();
		}
	}
	else if(m_spareFigureSelected != NULL)
	{//попытка перенести фигуру из неиспользованных на поле
		if(canBePlaced(m_spareFigureSelected, place))
		{
			Figure figureToInsert(*m_spareFigureSelected);
			m_spareFigures.removeAll(figureToInsert);
			clearSpareSelection();
			figureToInsert.setCenter(place);
			m_fieldFigures.append(figureToInsert);
			clearFieldSelection();

			updateSpareFiguresList();
			if(m_fieldWidget != NULL) m_fieldWidget->update();
			if(m_spareFiguresWidget != NULL) m_spareFiguresWidget->update();
		}
	}
	else
	{//выбор фигуры на поле
		Figure::list::Iterator i = getFigureInPlace(place);
		if(i != m_fieldFigures.end())
		{
			m_fieldFigureSelected = &*i;
			if(m_fieldWidget != NULL)
			{
				m_fieldWidget->setOverlayFigure(*i);
				m_fieldWidget->setTrackingFigure(*i);
				m_fieldWidget->update();
			}
		}
	}
}

void FieldController::onFieldRmbClicked()
{
	onSpareFiguresRmbClicked();
}

void FieldController::onFieldRotated(bool clockwise)
{
	if(m_fieldFigureSelected != NULL)
	{
		clockwise ? m_fieldFigureSelected->rotateCW() : m_fieldFigureSelected->rotateCCW();
		if(m_fieldWidget != NULL)
		{
			m_fieldWidget->setOverlayFigure(*m_fieldFigureSelected);
			m_fieldWidget->setTrackingFigure(*m_fieldFigureSelected);
			m_fieldWidget->update();
		}
	}
}

void FieldController::onFieldRemoved()
{
	Figure figure = *m_fieldFigureSelected;
	m_fieldFigures.removeAll(figure);
	addSpareFigure(figure);

	clearFieldSelection();

	updateSpareFiguresList();

	if(m_fieldWidget != NULL) m_fieldWidget->update();
	if(m_spareFiguresWidget != NULL) m_spareFiguresWidget->update();
}

//может ли фигура, на которую указывает figureToPlace быть расположена с центром в place
bool FieldController::canBePlaced(Figure* figureToPlace, FieldPlace newPlace) const
{
	Figure potentialFigure(*figureToPlace);
	potentialFigure.setCenter(newPlace);
	FieldPlace::list elemnets(potentialFigure.elements());//список элементов новый фигуры

	for(Figure::list::ConstIterator f = m_fieldFigures.begin(); f != m_fieldFigures.end(); ++f)
	{//для каждой фигуры на поле
		if(*figureToPlace != *f)
		{//если она не та же самая, которую мы собираемся размещать

			//если у текущей фигуры есть пересечения в вставляемой, то вставка невозможна
			if( !f->elements().toSet().intersect(elemnets.toSet()).isEmpty() ) return false;
		}
	}
	Q_FOREACH(FieldPlace newPlace, elemnets)
	{//для каждого элемента вставляемой фигуры
		//если нет соответствующего элемента поля, то вставка невозможна
		if(!m_fieldConf.contains(newPlace)) return false;
	}

	return true;
}

void FieldController::clearFieldSelection()
{
	m_fieldFigureSelected = NULL;
	m_fieldWidget->removeOverlayFigure();
	m_fieldWidget->removeTrackingFigure();
}

void FieldController::clearSpareSelection()
{
	m_spareFigureSelected = NULL;
}

Figure::list::Iterator FieldController::getFigureInPlace(FieldPlace place)
{
	for(Figure::list::Iterator i = m_fieldFigures.begin(); i != m_fieldFigures.end(); ++i)
	{
		if(i->elements().contains(place)) return i;
	}
	return m_fieldFigures.end();
}

void FieldController::updateSpareFiguresList()
{
	m_spareFiguresFieldWidth = 0;
	m_spareFiguresFieldHeight = 0;

	for(Figure::list::Iterator i = m_spareFigures.begin(); i != m_spareFigures.end(); ++i)
	{
		i->setCenter(i->originalCenter());
		i->move(0, m_spareFiguresFieldHeight);
		m_spareFiguresFieldHeight += i->height()+1;
		if(m_spareFiguresFieldWidth < i->width()) m_spareFiguresFieldWidth = i->width();
	}

	if(m_spareFiguresWidget != NULL)
	{
		m_spareFiguresWidget->setFieldWidth(m_spareFiguresFieldWidth);
		m_spareFiguresWidget->setFieldHeight(m_spareFiguresFieldHeight);
	}
}
