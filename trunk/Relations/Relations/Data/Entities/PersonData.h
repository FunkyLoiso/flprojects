#ifndef PersonData_h__
#define PersonData_h__

#include <QUuid>
#include <QMap>

#include "EntityBase.h"
#include "PropertyData.h"

namespace data
{

class Person : public EntityBase
{
public:
	Person(QUuid guid = QUuid::createUuid());
	virtual ~Person(void);

	QUuid GetGuid() const { return m_guid; }

	QList<Property> GetProperties() const { return m_properties.values(); }
	Property GetProperty(const QString& name);
	void SetProperty(const Property& value);

private:
	QUuid m_guid;
	QMap<QString, Property> m_properties;
	
};

}//namespace data

#endif // PersonData_h__
