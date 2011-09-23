#pragma once
#include <QString>
#include <QVector>

struct Node
{
	Node(QString val = QString()) : m_val(val) {};
	~Node(void);

	void AddChild(const QString& val);

	QVector<Node*> m_children;
	QString m_val;

};
