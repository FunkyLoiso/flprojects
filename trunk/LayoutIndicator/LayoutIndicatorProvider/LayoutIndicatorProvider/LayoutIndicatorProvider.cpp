#include "LayoutIndicatorProvider.h"
#include <QDebug>

#include <windows.h>

LayoutIndicatorProvider::LayoutIndicatorProvider(QObject *parent) : QObject(parent)
{
	timeId = startTimer(100);
	ser.Set_baud(38400);
	confirmed = false;
	lastLangId = -1;
}

LayoutIndicatorProvider::~LayoutIndicatorProvider(void)
{
	ser.Write("-", 1);
	ser.Close();
}

void LayoutIndicatorProvider::timerEvent( QTimerEvent * )
{
	if(!confirmed)
	{
		QStringList ports = ser.port_list();
		for each(QString port in ports)
		{
			ser.Close();
			if( int ret = ser.Open(port) )
			{
				qDebug() << "Error opening port" << port << ":" << ret;
				ser.Close();
				continue;
			}
	
			ser.Input_discard();
			qDebug() << "Sending 'i' to" << port;
			ser.Write("i", 1);
			char resp[1024];
			Sleep(10);
			
			int respSize = ser.Read(resp, sizeof(resp));
			if( respSize )
			{
				if(memcmp(resp, "I understand you!", respSize)) confirmed = true;
			}
			
	
			if(confirmed)
			{
				qDebug() << "Comunication established on" << port;
				//killTimer(timeId);
			}
			else qDebug() << "Unknown device on" << port;
		}
	}
	else
	{
		DWORD threadId = GetWindowThreadProcessId(GetForegroundWindow(), 0);
		HKL id = GetKeyboardLayout(threadId);

		//GUITHREADINFO Gti;
		//::ZeroMemory ( &Gti,sizeof(GUITHREADINFO));
		//Gti.cbSize = sizeof( GUITHREADINFO );

		//::GetGUIThreadInfo(0,&Gti);
		//DWORD dwThread = ::GetWindowThreadProcessId(Gti.hwndActive,0);

		//HKL id = ::GetKeyboardLayout(dwThread);

		LANGID langId = LOWORD(id);
		if(lastLangId != langId)
		{
			if(langId == 1049) ser.Write("r", 1);
			else if(langId == 1033) ser.Write("e", 1);
			else ser.Write("-", 1);
			lastLangId = langId;
		}
	}
}
