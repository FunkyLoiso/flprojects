#include "ScriptSource.h"

#include "Exception/ScriptErrorException.h"

using namespace AutoBot;

void ScriptSource::ReadScript(const QString& scriptText) throw(ScriptErrorException)
{
	QScriptValue ret = evaluate(scriptText);
	if(hasUncaughtException()) throw ScriptErrorException(ret.toString(), uncaughtExceptionLineNumber());
	init();//Этот метод должен быть определён в наследниках
	m_inited = true;
}
