#include "FrameController.h"

FrameController::FrameController()
: m_frameEnd_mcs(micros())
{
	setCube(NULL);
	setData(NULL);
	setFrameInterval_mcs(0);
}

FrameController::FrameController(LedCube* cube, LedCubeData* data)
: m_frameEnd_mcs(micros())
{
	setCube(cube);
	setData(data);
	setFrameInterval_mcs(0);
}
	
void FrameController::setCube(LedCube* cube)
{
	m_cube = cube;
}
void FrameController::setData(LedCubeData* data)
{
	m_data = data;
}
	
void FrameController::setFrameInterval_mcs(unsigned long time)
{
	m_frameInterval_mcs = time;
}
unsigned long FrameController::getFrameInterval_mcs() const
{
	return m_frameInterval_mcs;
}

void FrameController::setFPS(unsigned int fps)
{
	m_frameInterval_mcs = 1000000 / fps;
}

unsigned int FrameController::getFPS() const
{
	return 1000000 / m_frameInterval_mcs;
}
	
void FrameController::begin()
{//выключим куб на время рассчёта, что бы никой уровень не светился долше других
	m_cube->turnOut();
}
void FrameController::end()
{//пока не закончится время кадра - перерисовывам куб с одними и теми же данными снова и смнова
	while(micros() < m_frameEnd_mcs)
	{
		m_cube->lightCube(*m_data);
	}
	m_frameEnd_mcs += m_frameInterval_mcs;//рассчитаем время окончания следующего кадра
}