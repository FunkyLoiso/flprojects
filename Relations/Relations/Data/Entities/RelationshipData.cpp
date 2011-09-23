#include "RelationshipData.h"

using namespace data;

Relationship::Relationship() : m_type(TypeInvalid)
{}


Relationship::Relationship( Type type, QUuid master, QUuid slave)
: m_type(type), m_master(master), m_slave(slave)
{
	
}

