#include "Arduino.h"
#include "Scara_3Axis.h"
#include <math.h>

void Scara::delta_3axis();

/*
DeltaRobInverseKin::DeltaRobInverseKin(double L, double l, double wb, double wp, double up, double sp)
{
	
	_L = L;
	_l = l;
	_wb = wb;
	_wp = wp;
	_up = up;
	_sp = sp;

	maxArrIndex = 0;

	_rat = 18000 / PI;
	debugFlag = false;
}

void DeltaRobInverseKin::setGoalCoordinates(double x, double y, double z, int state)
{
	_x = x;
	_y = y;
	_z = z;
	_state = state;
	_computeAngles();
}

void DeltaRobInverseKin::_computeAngles()
{
	

	_computePara_abc();
	_computePara_EFG();
	_computePara_t();
	_computeGoalPos();
}
*/