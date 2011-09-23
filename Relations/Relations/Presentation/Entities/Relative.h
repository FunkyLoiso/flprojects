#ifndef Relative_h__
#define Relative_h__

#include "..\RelationsWidget.h"

namespace pres
{
class Person;
/**	������ �� ������������ */
class Relative
{
public:
	Relative(Person *person);
	virtual ~Relative(void);

	const Person* GetPerson() const {return m_person;};

	/**	���������� ����� �� �������.
	 *	@todo ����������� ��������� */
	virtual void Draw(RelationsWidget*) = 0;

protected:
	Person *m_person;
};

/**	������ */
class Child : public Relative
{
public:
	Child(Person* person, bool isAdopted = false);
	virtual void Draw(RelationsWidget*);

	bool IsAdopted() const { return m_isAdopted; }

private:
	bool m_isAdopted;///< ������ ��������
};

/**	�������� */
class Parent : public Relative
{
public:
	Parent(Person* person, bool isAdopter = false);
	virtual void Draw(RelationsWidget*);

	bool IsAdopter() const { return m_isAdopter; }

private:
	bool m_isAdopter;///< �������� ������������
};

/**	������[�] */
class Spouse : public Relative
{
public:
	Spouse(Person* person);
	virtual void Draw(RelationsWidget*);
};

}// namespace pres

#endif // Relative_h__