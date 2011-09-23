#ifndef ScriptSource_h__
#define ScriptSource_h__

#include <QtScript>

namespace AutoBot
{
class ScriptErrorException;

class ScriptSource : protected QScriptEngine
{
public:
	ScriptSource() : m_inited(false) {};
	~ScriptSource(void) {};

	/**	��������� ������. */
	void ReadScript(const QString& scriptText) throw(ScriptErrorException);

	bool IsInited() {return m_inited;}

protected:
	/**	���������, �������� �� ������, � ��������� �������������. ���������� � ReadScript */
	virtual void init() throw(ScriptErrorException) = 0;

private:
	bool m_inited;
};

}//namespace AutoBot

#endif // ScriptSource_h__
