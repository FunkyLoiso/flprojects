#include <LedCube.h>
#include <LedCubeData.h>

const byte latchPin = 8;
const byte clockPin = 12;
const byte dataPin = 11;

LedCube cube(latchPin, clockPin, dataPin);
LedCubeData data;

template<int SIZE>
class FrameQueue
{
public:
	FrameQueue()
	{
		m_enqPtr = m_arr;
		m_deqPtr = m_arr;
		m_size = 0;
	}

	bool enqueue(byte data[8][8])
	{
		if(m_size == SIZE) return false;

		m_enqPtr->setData(data);
		incrementPtr(m_enqPtr);
		++m_size;

		return true;
	}

	bool dequeue(LedCubeData& data)
	{
		if(m_size == 0) return false;

		data = *m_deqPtr;
		incrementPtr(m_deqPtr);
		--m_size;

		return true;
	}

	byte getSize(){return m_size;}

private:
	LedCubeData m_arr[SIZE];
	byte m_size;
	LedCubeData* m_enqPtr;
	LedCubeData* m_deqPtr;

	void incrementPtr(LedCubeData*& ptr)
	{
		if(ptr != m_arr+SIZE-1) ++ptr;
		else ptr = m_arr;
	}

};

FrameQueue<10> queue;

void setup()
{
	Serial.begin(115200);
}

void loop()
{
	static const unsigned long m_frameInterval_mcs = 1000000/*16667*/;
	static unsigned long m_frameEnd_mcs = micros();

	while(micros() < m_frameEnd_mcs)
	{
		receiveData();
		cube.lightCube(data);
	}
	updateCurentFrame();
	m_frameEnd_mcs += m_frameInterval_mcs;//рассчитаем время окончания следующего кадра
	Serial.print(". ");
	Serial.print(queue.getSize());
	Serial.println(" in queue");
}
void receiveData()
{
	static const char MAGIC = 0x22;
	static byte data[8][8];
	static const byte* dataEnd = *data+64;
	static byte* dataPtr = data[0];
	static bool isReceivingFrame = false;

	if(!isReceivingFrame)
	{
		while(Serial.available())
		{
			if(Serial.read() == MAGIC)
			{
				//Serial.println("magic received");
				isReceivingFrame = true;
				break;
			}
			
		}
	}

	if(isReceivingFrame)
	{
		byte left = dataEnd - dataPtr;
		byte read = Serial.readBytes((char*)dataPtr, left);
		if( read  == left ) //frame is ready
		{
			//add frame to queue
			//Serial.println("\t\t\tframe received");
			queue.enqueue(data);
			isReceivingFrame = false;
			//return pointer to start
			dataPtr = data[0];
		}
		else dataPtr += read;
	}
}

void updateCurentFrame()
{
	queue.dequeue(data);
}
