//==============================================================================
// GLOBALS - The main global definitions for the CPhenix program - still needs
//		to be cleaned up.
// This program assumes that the main files were compiled as C files
//==============================================================================
#ifndef _HEX_GLOBALS_H_
#define _HEX_GLOBALS_H_
#include <stdarg.h>
//#include <SoftwareSerial.h>        // Beta 11 newsoftware serial...
#include "SerialWrapper.h"

#include "Hex_Cfg.h"
#include "ServoDriver.h"
#include "InputController.h"
//=============================================================================
//[CONSTANTS]
//=============================================================================
#define BUTTON_DOWN 0
#define BUTTON_UP 	1

#define	c1DEC		10
#define	c2DEC		100
#define	c4DEC		10000
#define	c6DEC		1000000

#define	cRR			0
#define	cRM			1
#define	cRF			2
#define	cLR			3
#define	cLM			4
#define	cLF			5

#define	WTIMERTICSPERMSMUL  	64	// BAP28 is 16mhz need a multiplyer and divider to make the conversion with /8192
#define WTIMERTICSPERMSDIV  	125 // 
#define USEINT_TIMERAV

//////////////////////////////////////////////////////////////////////////
// special for arduino -> PC
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

typedef uint16_t word;
typedef uint8_t byte;
//////////////////////////////////////////////////////////////////////////




#define NUM_GAITS    5
extern void GaitSelect(void);
extern short SmoothControl (short CtrlMoveInp, short CtrlMoveOut, uint8_t CtrlDivider);


//-----------------------------------------------------------------------------
// Define global class objects
//-----------------------------------------------------------------------------
extern ServoDriver      g_ServoDriver;           // our global servo driver class
extern InputController  g_InputController;       // Our Input controller 
extern INCONTROLSTATE   g_InControlState;		 // State information that controller changes

//-----------------------------------------------------------------------------
// Define Global variables
//-----------------------------------------------------------------------------
extern boolean          g_fDebugOutput;

#if 0
extern bool		g_fHexOn;				//Switch to turn on Phoenix
extern bool		g_fPrev_HexOn;			//Previous loop state 
//Body position
extern long		BodyPosX; 		//Global Input for the position of the body
extern long		BodyPosY; 
extern long		BodyPosZ; 

//Body Inverse Kinematics
extern long		BodyRotX1; 		//Global Input pitch of the body
extern long		BodyRotY1;		//Global Input rotation of the body
extern long		BodyRotZ1; 		//Global Input roll of the body


//[gait]
extern uint8_t		GaitType;			//Gait type
extern short		NomGaitSpeed;		//Nominal speed of the gait

extern short		LegLiftHeight;		//Current Travel height
extern long		TravelLengthX;		//Current Travel length X
extern long		TravelLengthZ; 		//Current Travel length Z
extern long		TravelRotationY; 	//Current Travel Rotation Y

//[Single Leg Control]
extern uint8_t		SelectedLeg;
extern short		SLLegX;
extern short		SLLegY;
extern short		SLLegZ;
extern bool		fSLHold;		 	//Single leg control mode


//[Balance]
extern bool BalanceMode;

//[TIMING]
extern uint8_t			InputTimeDelay;	//Delay that depends on the input to get the "sneaking" effect
extern unsigned int			SpeedControl;	//Adjustible Delay
#endif


extern void MSound(uint8_t _pin, uint8_t cNotes, ...);
//extern int DBGPrintf(const char *format, ...);
//extern int SSCPrintf(const char *format, ...);


// The defined controller must provide the following
extern void InitController(void);
extern void	ControlInput(void);
extern void	AllowControllerInterrupts(bool fAllow);


extern bool CheckVoltage(void);

// debug handler...
extern bool g_fDBGHandleError;


//#ifdef __AVR__
//#if not defined(UBRR1H)
////extern NewSoftSerial SSCSerial;
//extern SoftwareSerial SSCSerial;
//#endif
extern SerialWrapper SSCSerial;

//#endif
#endif


