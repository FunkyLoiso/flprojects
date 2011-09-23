#ifndef RelationshipData_h__
#define RelationshipData_h__

#include <QUuid>
#include <QDate>

#include "EntityBase.h"

namespace data
{
/**	Отношение между людьми */
class Relationship : public EntityBase
{
public:
	enum Type
	{
		TypeMarriage,
		TypeParenthood,
		TypeAdoption,
		TypeInvalid
	};
	Relationship();//invalid relationship constructor
	Relationship(Type type, QUuid master = QUuid(), QUuid slave = QUuid());
	//virtual ~Relationship(void);
	static Relationship Invalid(){return Relationship();}
	
	Relationship::Type GetType() const { return m_type; }

	QUuid GetMaster() const { return m_master; }
	void SetMaster(QUuid val) { m_master = val; }

	QUuid GetSlave() const { return m_slave; }
	void SetSlave(QUuid val) { m_slave = val; }

	QDate GetBegining() const { return m_begining; }
	void SetBegining(QDate val) { m_begining = val; }

	QDate GetEnd() const { return m_end; }
	void SetEnd(QDate val) { m_end = val; }

private:
	Type m_type;
	
	QUuid m_master;
	QUuid m_slave;

	QDate m_begining;
	QDate m_end;
};

}// namespace data

#endif // RelationshipData_h__
