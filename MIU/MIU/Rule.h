#pragma once
#include <QString>

class Rule
{
public:
	Rule(void);
	~Rule(void);

	virtual bool IsApplicable(const QString& str) = 0;
	virtual QString Apply(const QString& str) = 0;
};
/*	xI -> xIU	*/
class RuleI : public Rule
{
public:
	bool IsApplicable(const QString& str)
	{
		return str.size() > 0 && str[str.size()-1] == 'I';
	}
	QString Apply(const QString& str)
	{
		return str + "U";
	}
};
/*	Mx -> Mxx	*/
class RuleII : public Rule
{
public:
	bool IsApplicable(const QString& str)
	{
		return str.size() > 0 && str[0] == 'M';
	}
	QString Apply(const QString& str)
	{
		return str + str.right(str.size() - 1);
	}
};
/*	xIIIy -> xUy	*/
class RuleIII : public Rule
{
public:
	bool IsApplicable(const QString& str)
	{
		return str.contains("III");
	}
	QString Apply(const QString& str)
	{
		QString out = str;
		return out.replace(out.indexOf("III"), 3, "U");
	}
};
/*	xUUy -> xy	*/
class RuleIV : public Rule
{
public:
	bool IsApplicable(const QString& str)
	{
		return str.contains("UU");
	}
	QString Apply(const QString& str)
	{
		QString out = str;
		return out.remove(out.indexOf("UU"), 2);
	}
};