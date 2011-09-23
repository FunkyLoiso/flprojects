#ifndef Exception_h__
#define Exception_h__

#include <typeinfo>
#include <QString>

namespace AutoBot
{

/**	Базовый класс для исключений */
class Exception
{
public:
	Exception(const QString& desc = "", const QString& caption = "Exception")
		: m_caption(caption), m_description(desc){}

	virtual ~Exception(void)
	{
	}

	QString GetCaption() const { return m_caption; }
	void SetCaption(QString val) { m_caption = val; }

	QString GetDescription() const { return m_description; }
	void SetDescription(QString val) { m_description = val; }

	QString GetFullMessage() const
	{
		return QString("Exception of type {0}\nCaption: \"{1}\"\nDesc: \"{2}\"").arg(typeid(*this).name()).arg(GetCaption()).arg(GetDescription());
	}

private:
	QString m_caption;		///< Заголовок сообщения
	QString m_description;	///< Тело сообщения

};

}
#endif // Exception_h__
