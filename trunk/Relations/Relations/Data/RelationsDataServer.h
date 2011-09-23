#ifndef RelationsDataServer_h__
#define RelationsDataServer_h__

#include <QVector>
#include "./Entities/PersonData.h"
#include "./Entities/RelationshipData.h"

namespace data
{

class RelationsDataServer
{
public:
	RelationsDataServer(void);
	~RelationsDataServer(void);

	QVector<Person> GetAllPeople();
	QVector<Relationship> GetAllRelationships();

private:
	QVector<Person> m_testPeople;
	QVector<Relationship> m_testRelations;
};

}// namespace data

#endif // RelationsDataServer_h__
