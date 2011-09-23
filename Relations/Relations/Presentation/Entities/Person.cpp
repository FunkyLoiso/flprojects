#include "./Person.h"
#include "./Property.h"
#include "./Relative.h"

using namespace pres;

Person::Person(const data::Person& person) : m_guid(person.GetGuid())
{
	for each(const data::Property& p in person.GetProperties())
	{
		Property* prop = Property::Create(p);
		m_properties.push_back(prop);
	}
}

Person::~Person(void)
{
	for each(Property* p in m_properties)
	{
		delete p;
	}
	for each(Relative* r in m_relatives)
	{
		delete r;
	}
}

void pres::Person::AddProperty(Property* prop)
{
	m_properties.push_back(prop);
}

void pres::Person::AddRelative(Relative* rel)
{
	m_relatives.push_back(rel);
}
