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
 */
class FrameController
{
public:
	FrameController();
	FrameController(LedCube* cube, LedCubeData* data);
	
	void setCube(LedCube* cube);
	void setData(LedCubeData* data);
	
	void setFrameInterval_mcs(unsigned long time);
	unsigned long getFrameInterval_mcs() const;
	void setFPS(unsigned int fps);
	unsigned int getFPS() const;
	
	void begin();
	void end();
	
private:
	LedCube* m_cube;
	LedCubeData* m_data;
	unsigned long m_frameInterval_mcs;
	unsigned long m_frameEnd_mcs;
};

#endif FRAME_CONTROLLER_H