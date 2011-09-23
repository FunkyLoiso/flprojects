#ifndef PropertyData_h__
#define PropertyData_h__

#include <QString>
#include <QVariant>
#include "EntityBase.h"

namespace data
{

/**	Информационное поле человека */
class Property : public EntityBase
{
public:
	Property();//invalid property constructor
	Property(QString name, QVariant value = QVariant());
	static Property Invalid() {return Property();}
	//~Property(void);

	QString GetName() const { return m_name; }

	QVariant GetValue() const { return m_value; }
	void SetValue(QVariant val) { m_value = val; }

	QVariant::Type GetType() const { return m_value.type(); }

private:

	QString m_name;
	QVariant m_value;
};

}// namespace data

#endif // PropertyData_h__
