#ifndef PlayerImp_h__
#define PlayerImp_h__

namespace AutoBot
{
class RulesManager;
class Field;
class Move;
class TypeMismatchException;
class NotInitializedException;

/**	���������� ������ */
class PlayerImp
{
public:
	PlayerImp(void)
	{
	}

	virtual ~PlayerImp(void)
	{
	}

	/**	������ ��������� ������.
	  *	@exception TypeMismatchException, ���� ��� �������� ������ �� ������ ����. */
	virtual void SetRulesManager(const RulesManager*) throw(TypeMismatchException) = 0;
	/**	�������� ��������� ��� �� �������� ����. */
	virtual Move GetMove(const Field&) throw(NotInitializedException) = 0;
};

}//namespace AutoBot

#endif // PlayerImp_h__
