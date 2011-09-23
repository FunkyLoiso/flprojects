#ifndef Relative_h__
#define Relative_h__

#include "..\RelationsWidget.h"

namespace pres
{
class Person;
/**	Ссылка на родственника */
class Relative
{
public:
	Relative(Person *person);
	virtual ~Relative(void);

	const Person* GetPerson() const {return m_person;};

	/**	Отрисовать связь на виджете.
	 *	@todo реализовать отрисовку */
	virtual void Draw(RelationsWidget*) = 0;

protected:
	Person *m_person;
};

/**	Ребёнок */
class Child : public Relative
{
public:
	Child(Person* person, bool isAdopted = false);
	virtual void Draw(RelationsWidget*);

	bool IsAdopted() const { return m_isAdopted; }

private:
	bool m_isAdopted;///< Ребёнок усыновлён
};

/**	Родитель */
class Parent : public Relative
{
public:
	Parent(Person* person, bool isAdopter = false);
	virtual void Draw(RelationsWidget*);

	bool IsAdopter() const { return m_isAdopter; }

private:
	bool m_isAdopter;///< Является усыновителем
};

/**	Супруг[а] */
class Spouse : public Relative
{
public:
	Spouse(Person* person);
	virtual void Draw(RelationsWidget*);
};

}// namespace pres

#endif // Relative_h__