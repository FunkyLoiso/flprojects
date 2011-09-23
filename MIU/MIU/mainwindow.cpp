#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_rules.push_back( new RuleI() );
	m_rules.push_back( new RuleII() );
	m_rules.push_back( new RuleIII() );
	m_rules.push_back( new RuleIV() );

	m_head = new Node("MI");

	doJob(m_head);

	QTreeWidgetItem* root = new QTreeWidgetItem(QStringList("Root"));
	ui.treeWidget->addTopLevelItem(root);
	display(m_head, root);
	ui.treeWidget->expandAll();

	QString title("Max depth: %1. Total theorems: %2. \"MU\" found: %3.");
	title = title.arg(maxLength).arg(m_theorems.count()).arg(m_theorems.contains("MU"));

	setWindowTitle(title);
}

MainWindow::~MainWindow()
{
	for each(Rule* r in m_rules) delete r;
	delete m_head;
}

void MainWindow::doJob( Node* node )
{
	QString& val = node->m_val;
	//проверим условия выхода
	if(m_theorems.contains(val)) return;
	if(val.length() > maxLength) return;
	//добавим эту теорему в список теорем
	m_theorems.insert(val);
	//Выполним каждое правило и добавим детей
	for each(Rule* r in m_rules)
	{
		if(r->IsApplicable(val))
		{
			node->AddChild(r->Apply(val));
		}
	}
	//Выполним рекурсивно для детей
	for each(Node* n in node->m_children)
	{
		doJob(n);
	}
}

void MainWindow::display( Node* node, QTreeWidgetItem* parent )
{
	QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(node->m_val));
	parent->addChild(newItem);
	for each(Node* n in node->m_children)
	{
		display(n, newItem);
	}
}
