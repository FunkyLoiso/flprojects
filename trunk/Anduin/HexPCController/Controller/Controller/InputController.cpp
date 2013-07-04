#include "InputController.h"

#include "Hex_Globals.h"

InputController  g_InputController;

static short g_BodyYOffset = 65;  // 0 - Devon wanted...
static short g_BodyYShift = 0;

void     InputController::Init(void)
{
};

void     InputController::ControlInput(void)
{
	bool WalkMethod = false;
	bool DoubleTravelOn = false;

	short rx = 0;
	short ry = 0;
	short lx = 0;
	short ly = 100;

	if (WalkMethod)  //(Walk Methode) 
		g_InControlState.TravelLength.z = ry; //Right Stick Up/Down  

	else {
		g_InControlState.TravelLength.x = -lx;
		g_InControlState.TravelLength.z = ly;
	}

	if (!DoubleTravelOn) {  //(Double travel length)
		g_InControlState.TravelLength.x = g_InControlState.TravelLength.x/2;
		g_InControlState.TravelLength.z = g_InControlState.TravelLength.z/2;
	}

	g_InControlState.TravelLength.y = -(rx)/4; //Right Stick Left/Right 

	//Calculate walking time delay
	g_InControlState.InputTimeDelay = 128 - max(max(abs(lx), abs(ly)), abs(rx));

	//Calculate g_InControlState.BodyPos.y
	g_InControlState.BodyPos.y = max(g_BodyYOffset + g_BodyYShift, 0);
};
void     InputController::AllowControllerInterrupts(bool fAllow)
{

};
