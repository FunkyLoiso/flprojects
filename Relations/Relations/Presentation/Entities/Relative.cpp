#include "Relative.h"
#include "Person.h"

using namespace pres;

Relative::Relative(Person* person) : m_person(person) 
{}

Relative::~Relative(void)
{
}

//////////////////////////////////////////////////////////////////////////
Child::Child( Person* person, bool isAdopted)
: Relative(person), m_isAdopted(isAdopted)
{
}

void Child::Draw( RelationsWidget* )
{
	throw std::exception("The method or operation is not implemented.");
}

//////////////////////////////////////////////////////////////////////////
Parent::Parent( Person* person, bool isAdopter)
: Relative(person), m_isAdopter(isAdopter)
{
}

void Parent::Draw( RelationsWidget* )
{
	throw std::exception("The method or operation is not implemented.");
}

//////////////////////////////////////////////////////////////////////////
Spouse::Spouse( Person* person ) : Relative(person)
{
}

void Spouse::Draw( RelationsWidget* )
{
	throw std::exception("The method or operation is not implemented.");
}
