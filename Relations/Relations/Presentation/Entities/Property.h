#ifndef Property_h__
#define Property_h__

#include <QVariant>
#include <QDate>

#include "Data/Entities/PropertyData.h"

namespace pres
{
/**	������� ����� ��� ������� ��������. */
class Property : public QObject
{
	Q_OBJECT
public:
	Property(QString name);
	virtual ~Property(void);
	static Property* Create(const data::Property& prop);
	/**	��������� ����� ��������� ������������ ��������, ����� ������� ����� ��������������� �������� ��������. */
	QWidget* GetEditor(QWidget* parent = NULL) const;

	QString GetName() const { return m_name; }

	QVariant GetValue() const { return m_value; }

	virtual QString ToString() const { return m_value.toString(); }

public slots:
	void SetValue(const QVariant& val) { m_value = val; }

protected:
	QString m_name;
	QVariant m_value;
	/**	������� ����������� ������� */
	virtual QWidget* createEditor(QWidget* parent = NULL) const = 0;
	/**	��������� ����������� ������� �� ������ SetValue */
	virtual void connectEditor(QWidget* editor) const = 0;

};

/**	�������� - ����� ����� */
class IntProperty : public Property
{
public:
	IntProperty(QString name, int value = 0) : Property(name){m_value = value;}

private:
	virtual QWidget* createEditor(QWidget* parent = NULL) const;
	virtual void connectEditor(QWidget* editor) const;
};

/**	�������� - ������ */
class StringProperty : public Property
{
public:
	StringProperty(QString name, const QString& value = QString()) : Property(name) {m_value = value;}

private:
	virtual QWidget* createEditor(QWidget* parent = NULL) const;
	virtual void connectEditor(QWidget* editor) const;
};

/**	�������� - ���� */
class DateProperty : public Property
{
public:
	DateProperty(QString name, QDate value = QDate()) : Property(name) {m_value = value;}

private:
	virtual QWidget* createEditor(QWidget* parent = NULL) const;
	virtual void connectEditor(QWidget* editor) const;
};

}// namespace pres

#endif // Property_h__