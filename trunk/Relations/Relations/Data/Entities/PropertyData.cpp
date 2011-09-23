#include "PropertyData.h"

using namespace data;

data::Property::Property()
{
#ifdef _DEBUG
	m_name = "# invalid #";
#endif
}

data::Property::Property( QString name, QVariant value)
: m_name(name), m_value(value)
{}
