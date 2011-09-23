#ifndef RelationsPresServer_h__
#define RelationsPresServer_h__

#include <QVector>

#include "Data\RelationsDataServer.h"
#include "Entities\Person.h"

namespace pres
{
/**	Сервер уровня представления */
class RelationsPresServer
{
public:
	RelationsPresServer(void);
	~RelationsPresServer(void);

	void SetDataSource(data::RelationsDataServer* srv);

	/**	Test. Получить всех людей. */
	QVector<Person*> GetAllPeople();

private:
	data::RelationsDataServer* m_srv;

	QVector<Person*> m_people;

	/**	Получить человека по идентификатору.
	 *	@return NULL, если такого нет */
	Person* getPersonByGuid(QUuid id);

	struct SameGuid
	{
		SameGuid(QUuid id) : m_id(id) {}
		bool operator()(const Person* other)
		{
			return other->GetGuid() == m_id;
		}
		QUuid m_id;
	};
};

}// namespace pres

#endif // RelationsPresServer_h__