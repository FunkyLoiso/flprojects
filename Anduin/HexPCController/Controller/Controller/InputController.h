//==============================================================================
// InputController.h - This is is the class definition for the abstraction of
//     which input controller is used to control the Hex robot.  There will be
//     several implementations of this class, which include:
//         PS2 - 
//         XBEE
//         Serial
//         Autonomous
//==============================================================================
#ifndef _INPUT_CONTROLLER_h_
#define _INPUT_CONTROLLER_h_

#include "Hex_Cfg.h"  // make sure we know what options are enabled...
//#if ARDUINO>99
//#include <Arduino.h> // Arduino 1.0
//#else
//#include <Wprogram.h> // Arduino 0022
//#endif
#include <stdint.h>

class InputController {
  public:
    void     Init(void);
    void     ControlInput(void);
    void     AllowControllerInterrupts(bool fAllow);

	void	setLY(short val);

  private:
	  bool WalkMethod;
	  bool DoubleTravelOn;

	  short rx;
	  short ry;
	  short lx;
	  short ly;
} ;   

typedef struct _Coord3D {
    long      x;
    long      y;
    long      z;
} COORD3D;


//==============================================================================
// class ControlState: This is the main structure of data that the Control 
//      manipulates and is used by the main Phoenix Code to make it do what is
//      requested.
//==============================================================================
typedef struct _InControlState {
     bool		fHexOn;				//Switch to turn on Phoenix
     bool		fPrev_HexOn;			//Previous loop state 
//Body position
     COORD3D        BodyPos;

//Body Inverse Kinematics
     COORD3D        BodyRot1;               // X -Pitch, Y-Rotation, Z-Roll

//[gait]
     uint8_t			GaitType;			//Gait type

     short			LegLiftHeight;		//Current Travel height
     COORD3D        TravelLength;            // X-Z or Length, Y is rotation.

//[Single Leg Control]
     uint8_t			SelectedLeg;
     COORD3D        SLLeg;                // 
     bool		fSLHold;		 	//Single leg control mode


//[Balance]
     bool        BalanceMode;

//[TIMING]
     uint8_t			InputTimeDelay;	//Delay that depends on the input to get the "sneaking" effect
     unsigned int			SpeedControl;	//Adjustible Delay

//[]Foot sensors
	uint16_t sensorValue[6];
} INCONTROLSTATE;


#endif //_INPUT_CONTROLLER_h_
