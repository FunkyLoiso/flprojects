#define BAUD_RATE 57600

void setup()
{
	Serial.begin(57600);
}

#define MSG_SIZE 2
uint8_t msg[MSG_SIZE];
static const int avgSize = 10;
int avg = 0;

void loop()
{
	int counter = avgSize;
	while(--counter >= 0)
	{
		avg += analogRead(A2);
	}
	avg /= avgSize;

	msg[0] = avg;      // first 8 bits to msg[0]
	msg[1] = avg >> 8; // last 2 bits to msg[1]
	//msg[2] = time >> 6;
	//msg[3] = time >> 14;
	//msg[4] = time >> 22;
	Serial.write(msg, MSG_SIZE);
}
