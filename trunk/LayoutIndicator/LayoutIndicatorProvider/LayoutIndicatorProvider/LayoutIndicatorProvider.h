#pragma once
#include <QObject>

#include "serial.h"

class LayoutIndicatorProvider :	public QObject
{
public:
	LayoutIndicatorProvider(QObject *parent = 0);
	~LayoutIndicatorProvider(void);

private:
	void timerEvent(QTimerEvent *);

	Serial ser;

	int timeId;
	bool confirmed;
	LANGID lastLangId;
};
