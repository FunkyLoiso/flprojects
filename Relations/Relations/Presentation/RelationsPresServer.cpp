#include "RelationsPresServer.h"
#include "Entities\Person.h"

#include <algorithm>

using namespace pres;

RelationsPresServer::RelationsPresServer(void)
{
}

RelationsPresServer::~RelationsPresServer(void)
{
}


void pres::RelationsPresServer::SetDataSource( data::RelationsDataServer* srv )
{
	for each(Person* p in m_people)
	{
		delete p;
	}
	m_people.clear();

	m_srv = srv;

	QVector<data::Person> dataPeople = m_srv->GetAllPeople();
	QVector<data::Relationship> dataRelationships = m_srv->GetAllRelationships();
	//создадим людей
	for each(const data::Person& dataP in dataPeople)
	{
		m_people.push_back(new Person(dataP));
	}
	//добавим им отношения
	for each(const data::Relationship& dataR in dataRelationships)
	{
		Person* master = getPersonByGuid(dataR.GetMaster());
		if(master != NULL)
		{
			master->AddRelative(new Relative())
		}
	}
}


QVector<Person*> RelationsPresServer::GetAllPeople()
{
	return m_people;
}

Person* RelationsPresServer::getPersonByGuid( QUuid id )
{
	QVector<Person*>::iterator i = std::find_if(m_people.begin(), m_people.end(), SameGuid(id));
	if(i != m_people.end()) return *i;
	else return NULL;
}
