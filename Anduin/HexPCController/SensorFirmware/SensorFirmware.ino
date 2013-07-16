#include <SPI.h>

#define BAUD_RATE 57600

//Assign the Chip Select signal to pin 10.
int CS=10;

//This is a list of some of the registers available on the ADXL345.
//see at https://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
byte POWER_CTL = 0x2D;	//Power Control Register
byte DATA_FORMAT = 0x31;
byte DATAX0 = 0x32;	//X-Axis Data 0
byte DATAX1 = 0x33;	//X-Axis Data 1
byte DATAY0 = 0x34;	//Y-Axis Data 0
byte DATAY1 = 0x35;	//Y-Axis Data 1
byte DATAZ0 = 0x36;	//Z-Axis Data 0
byte DATAZ1 = 0x37;	//Z-Axis Data 1

//This buffer will hold values read from the ADXL345 registers.
byte values[10];
//These variables will be used to hold the x,y and z axis accelerometer values.
int x,y,z;
//deltas for accel callibration
int dx = 0, dy = 0, dz = 0;

// 
//#define MSG_SIZE 6
//uint8_t msg[MSG_SIZE];
static const double alpha = 0.3;
//int filtered[6] = {0};

void MSound(uint8_t _pin, byte cNotes, ...);

void setup()
{
	Serial.begin(BAUD_RATE);
	Serial.flush();

	//Initiate an SPI communication instance.
	SPI.begin();
	//Configure the SPI connection for the ADXL345.
	SPI.setDataMode(SPI_MODE3);

	//Set up the Chip Select pin to be an output from the Arduino.
	pinMode(CS, OUTPUT);
	//Before communication starts, the Chip Select pin needs to be set high.
	digitalWrite(CS, HIGH);

	//Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
	//writeRegister(DATA_FORMAT, 0x01);
	writeRegister(DATA_FORMAT, 0x00);//0x00 - +-2g mode
	//Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
	writeRegister(POWER_CTL, 0x08);  //Measurement mode  

	delay(10);
	for(int i = 0; i < 100; ++i)
	{
		readRegister(DATAX0, 6, values);

		//The ADXL345 gives 10-bit acceleration values, but they are stored as bytes (8-bits). To get the full value, two bytes must be combined for each axis.
		//The X value is stored in values[0] and values[1].
		x = ((int)values[1]<<8)|(int)values[0];
		//The Y value is stored in values[2] and values[3].
		y = ((int)values[3]<<8)|(int)values[2];
		//The Z value is stored in values[4] and values[5].
		z = ((int)values[5]<<8)|(int)values[4];

		dx += (0-x);
		dy += (0-y);
		dz += (256-z);

		delay(20);
	}
	dx /= 100;
	dy /= 100;
	dz /= 100;

	MSound(5, 3, 200, 1000, 100, 400, 100, 400);
}

bool started = false;

void loop()
{
	if(!started)
	{
		while(Serial.read() != 97) delay(1);
		started = true;
	}

	for(int i = 0; i < 6; ++i)
	{
		//send pressure sensors unfiltered. We send value/4 to fit 10 bit into 8!
		Serial.write((analogRead(A0+i))/4);
	}

	readRegister(DATAX0, 6, values);
	//we filter accel data
	x = filter( (((int)values[1]<<8)|(int)values[0]) + dx, x, alpha );
	y = filter( (((int)values[3]<<8)|(int)values[2]) + dy, y, alpha );
	z = filter( (((int)values[5]<<8)|(int)values[4]) + dz, z, alpha );

	//x = ((int)values[1]<<8)|(int)values[0] + dx;
	//y = ((int)values[3]<<8)|(int)values[2] + dy;
	//z = ((int)values[5]<<8)|(int)values[4] + dz;

	double angleX = atan2(x, z) * RAD_TO_DEG;
	double angleY = atan2(y, z) * RAD_TO_DEG;

	//we send angel in degrees multiplied by 10 as one byte

	Serial.write(char(angleX*10));
	Serial.write(char(angleY*10));


	if(Serial.read() == 99) started = false;
}

int filter(int val, int prevFiltered, double alpha)
{
	return prevFiltered  + alpha * (val - prevFiltered);
}

//This function will write a value to a register on the ADXL345.
//Parameters:
//  char registerAddress - The register to write a value to
//  char value - The value to be written to the specified register.
void writeRegister(byte registerAddress, byte value){
	//Set Chip Select pin low to signal the beginning of an SPI packet.
	digitalWrite(CS, LOW);
	//Transfer the register address over SPI.
	SPI.transfer(registerAddress);
	//Transfer the desired register value over SPI.
	SPI.transfer(value);
	//Set the Chip Select pin high to signal the end of an SPI packet.
	digitalWrite(CS, HIGH);
}

//This function will read a certain number of registers starting from a specified address and store their values in a buffer.
//Parameters:
//  char registerAddress - The register addresse to start the read sequence from.
//  int numBytes - The number of registers that should be read.
//  char * values - A pointer to a buffer where the results of the operation should be stored.
void readRegister(byte registerAddress, int numBytes, byte* values){
	//Since we're performing a read operation, the most significant bit of the register address should be set.
	char address = 0x80 | registerAddress;
	//If we're doing a multi-byte read, bit 6 needs to be set as well.
	if(numBytes > 1)address = address | 0x40;

	//Set the Chip select pin low to start an SPI packet.
	digitalWrite(CS, LOW);
	//Transfer the starting register address that needs to be read.
	SPI.transfer(address);
	//Continue to read registers until we've read the number specified, storing the results to the input buffer.
	for(int i=0; i<numBytes; i++){
		values[i] = SPI.transfer(0x00);
	}
	//Set the Chips Select pin high to end the SPI packet.
	digitalWrite(CS, HIGH);
}

//==============================================================================
//    SoundNoTimer - Quick and dirty tone function to try to output a frequency
//            to a speaker for some simple sounds.
//==============================================================================
void SoundNoTimer(uint8_t _pin, unsigned long duration,  unsigned int frequency)
{
#ifdef __AVR__
	volatile uint8_t *pin_port;
	volatile uint8_t pin_mask;
#else
	volatile uint32_t *pin_port;
	volatile uint16_t pin_mask;
#endif
	long toggle_count = 0;
	long lusDelayPerHalfCycle;

	// Set the pinMode as OUTPUT
	pinMode(_pin, OUTPUT);

	pin_port = portOutputRegister(digitalPinToPort(_pin));
	pin_mask = digitalPinToBitMask(_pin);

	toggle_count = 2 * frequency * duration / 1000;
	lusDelayPerHalfCycle = 1000000L/(frequency * 2);

	// if we are using an 8 bit timer, scan through prescalars to find the best fit
	while (toggle_count--) {
		// toggle the pin
		*pin_port ^= pin_mask;

		// delay a half cycle
		delayMicroseconds(lusDelayPerHalfCycle);
	}    
	*pin_port &= ~(pin_mask);  // keep pin low after stop

}

void MSound(uint8_t _pin, byte cNotes, ...)
{
	va_list ap;
	unsigned int uDur;
	unsigned int uFreq;
	va_start(ap, cNotes);

	while (cNotes > 0) {
		uDur = va_arg(ap, unsigned int);
		uFreq = va_arg(ap, unsigned int);
		SoundNoTimer(_pin, uDur, uFreq);
		cNotes--;
	}
	va_end(ap);
}
