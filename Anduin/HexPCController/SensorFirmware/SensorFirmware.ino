#define BAUD_RATE 57600

void setup()
{
	Serial.begin(57600);
	
}

#define MSG_SIZE 6
uint8_t msg[MSG_SIZE];
static const int avgSize = 5;
int avg[6] = {0};

bool started = false;

void loop()
{
	if(!started)
	{
		while(Serial.read() != 97) delay(1);
		started = true;
	}

	int counter = avgSize;
	while(--counter >= 0)
	{
		for(int i = 0; i < 6; ++i) avg[i] += analogRead(A0+i);
	}
	for(int i = 0; i < 6; ++i)
	{
		msg[i] = avg[i]/avgSize/4;//we send value/4!
		avg[i] = 0;
	}

	Serial.write(msg, MSG_SIZE);

	if(Serial.read() == 99) started = false;
}
