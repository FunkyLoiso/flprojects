#ifndef FRAME_CONTROLLER_H
#define FRAME_CONTROLLER_H

#include "LedCube.h"
#include "LedCubeData.h"

/*
 *	Класс для контроля частоты кадров.
 *	Использование:
 *		void setup()
 *		{
 *			controller.setCube(cube);
 *			controller.setData(data);
 *			controller.setFps(60);
 *		}
 *		void loop()
 *		{
 *			controller.begin();
 *			//вычислить новый кадр, изменить data
 *			controller.end();
 *		}
 *
 *	Продолжительнгость кадра и частота кадров связанны обратным соотношением, так что задавать нужно только один из этих параметров
 */
class FrameController
{
public:
	FrameController();
	FrameController(LedCube* cube, LedCubeData* data);
	
	void setCube(LedCube* cube);		// установить куб
	void setData(LedCubeData* data);	// установить данные
	
	void setFrameInterval_mcs(unsigned long time);	// задвать продолжительность кадра
	unsigned long getFrameInterval_mcs() const;		// получить заданную продолжительность кадра
	void setFPS(unsigned int fps);	// задать частоту кадров
	unsigned int getFPS() const;	// получить заданную частоту кадров
	
	void begin();	// поместите этот вызов перед расчётом кадра
	void end();		// поместите этот вызов после расчёта кадра
	
private:
	LedCube* m_cube;
	LedCubeData* m_data;
	unsigned long m_frameInterval_mcs;
	unsigned long m_frameEnd_mcs;
};

#endif FRAME_CONTROLLER_H