#ifndef Person_h__
#define Person_h__

#include <QVector>

#include "Data/Entities/PersonData.h"

namespace pres
{
	class Property;
	class Relative;
/**	ќписание человека уровн€ представлени€.
  *	 ласс ответственен за удаление своих свойств в деструкторе. */
class Person
{
public:
	Person(const data::Person&);
	~Person(void);

	void AddProperty(Property*);
	QVector<Property*> Properties() const { return m_properties; }

	void AddRelative(Relative*);
	QVector<Relative*> Relatives() const { return m_relatives; }

	QUuid GetGuid() const { return m_guid; }

private:
	QVector<Property*> m_properties;
	QVector<Relative*> m_relatives;

	QUuid m_guid;
};

}// namespace pres

#endif // Person_h__
