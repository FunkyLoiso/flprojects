void setup()
{
	Serial.begin(115200);
        while(Serial.read() != 97) delay(100);
}

uint8_t msg[4];

void loop()
{
	int v0 = analogRead(A0);
	msg[0] = v0;      // first 8 bits to msg[0]
	msg[1] = v0 >> 8; // last 2 bits to msg[1]
	int v3 = analogRead(A3);
	msg[1] |= v3 << 2;// first 6 bits to msg[1]
	msg[2] = v3 >> 6; // last 4 bits to msg[2] 
	int v5 = analogRead(A5);
	msg[2] |= v5 << 4;// first 4 bits tp msg[2]
	msg[3] = v5 >> 4; // last 6 bits to msg[3]

	Serial.write(msg, 4);
}
