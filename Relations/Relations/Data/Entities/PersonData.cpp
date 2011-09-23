#include "PersonData.h"

using namespace data;

Person::Person(QUuid guid) : m_guid(guid)
{
}

Person::~Person(void)
{
}

Property Person::GetProperty( const QString& name )
{
	if(m_properties.contains(name)) return m_properties[name];
	else return Property::Invalid();
}

void Person::SetProperty( const Property& value )
{
	m_properties[value.GetName()] = value;
}
