#include <QSpinBox>
#include <QLineEdit>
#include <QCalendarWidget>

#include "./Property.h"

using namespace pres;

Property::Property(QString name) : m_name(name)
{
}

Property::~Property(void)
{
}

Property* Property::Create( const data::Property& prop )
{
	switch(prop.GetType())
	{
	case QVariant::Int:		return new IntProperty(prop.GetName(), prop.GetValue().toInt());
	case QVariant::String:	return new StringProperty(prop.GetName(), prop.GetValue().toString());
	case QVariant::Date:	return new DateProperty(prop.GetName(), prop.GetValue().toDate());
	default: throw -1;
	}
}

QWidget* Property::GetEditor( QWidget* parent) const
{
	QWidget* ed = createEditor(parent);
	connectEditor(ed);
	return ed;
}

//////////////////////////////////////////////////////////////////////////
QWidget* IntProperty::createEditor(QWidget* parent) const
{
	QSpinBox* sb = new QSpinBox(parent);
	sb->setValue(m_value.toInt());

	return sb;
}

void IntProperty::connectEditor( QWidget* editor ) const
{
	QSpinBox* sb = dynamic_cast<QSpinBox*>(editor);
	if (NULL == sb) throw -1;

	connect(sb, SIGNAL(valueChanged(int)), this, SLOT(SetValue(const QVariant&)));

}

//////////////////////////////////////////////////////////////////////////
QWidget* StringProperty::createEditor( QWidget* parent) const
{
	QLineEdit* le = new QLineEdit(parent);
	le->setText(m_value.toString());
	return le;
}
void pres::StringProperty::connectEditor( QWidget* editor ) const
{
	QLineEdit* le = dynamic_cast<QLineEdit*>(editor);
	if (NULL == le) throw -1;
	
	connect(le, SIGNAL(textChanged(const QString&)), this, SLOT(SetValue(const QVariant&)));
}

//////////////////////////////////////////////////////////////////////////
QWidget* DateProperty::createEditor( QWidget* parent) const
{
	QCalendarWidget* cw = new QCalendarWidget(parent);
	cw->setSelectedDate(m_value.toDate());
	return cw;
}

void DateProperty::connectEditor( QWidget* editor ) const
{
	QCalendarWidget* cw = dynamic_cast<QCalendarWidget*>(editor);
	if (NULL == cw) throw -1;
	
	connect(cw, SIGNAL(clicked(const QDate&)), this, SLOT(SetValue(const QVariant&)));
}
