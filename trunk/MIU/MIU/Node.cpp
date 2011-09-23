#include "Node.h"

Node::~Node(void)
{
	for each(Node* n in m_children)
	{
		delete n;
	}
}

void Node::AddChild( const QString& val )
{
	m_children.push_back(new Node(val));
}
