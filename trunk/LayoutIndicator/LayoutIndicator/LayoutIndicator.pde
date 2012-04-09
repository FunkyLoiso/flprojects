#define LINE_1 3 // F
#define LINE_2 5 // |
#define LINE_3 6 // _

#define ANALOG 0

//int lastMs = 0;
char dspItem = '\0';

static int intensivity = 255;

void DisplayR()
{
	 analogWrite(LINE_1, intensivity);
	 analogWrite(LINE_2, intensivity);
	 analogWrite(LINE_3, 0);
}

void DisplayE()
{
	analogWrite(LINE_1, intensivity);
	analogWrite(LINE_2, 0);
	analogWrite(LINE_3, intensivity);
}

void DisplayNothing()
{
	analogWrite(LINE_1, 0);
	analogWrite(LINE_2, 0);
	analogWrite(LINE_3, 0);
}

void setup()
{
	 pinMode(LINE_1, OUTPUT);
	 pinMode(LINE_2, OUTPUT);
	 pinMode(LINE_3, OUTPUT);

	 Serial.begin(38400);
}

void loop()
{
	// 1. Process commands
	while(Serial.available())
	{
		char data = (char)Serial.read();
		//Serial.print("Got ");
		//Serial.println(data);
		switch(data)
		{
		case 'i': // id command
			Serial.println("I understand you!");
			break;
		default:
			dspItem = data;
			break;
		}
	}
	// 2. Update intensivity
	intensivity = analogRead(ANALOG) / 4;

	// 3. Switch the indication

	if(dspItem == 'e')		DisplayE();
	else if(dspItem == 'r')	DisplayR();
	else					DisplayNothing();
}
