#include "RelationsDataServer.h"

using namespace data;

RelationsDataServer::RelationsDataServer(void)
{
	m_testPeople.resize(15);
	for each(const Person& p in m_testPeople)
	{
		Relationship r = Relationship(Relationship::TypeAdoption, p.GetGuid(), m_testPeople[qrand()%m_testPeople.size()].GetGuid());
		m_testRelations.push_back(r);
	}
}

RelationsDataServer::~RelationsDataServer(void)
{
}

QVector<Person> RelationsDataServer::GetAllPeople()
{
	return m_testPeople;
}

QVector<Relationship> RelationsDataServer::GetAllRelationships()
{
	return m_testRelations;
}
