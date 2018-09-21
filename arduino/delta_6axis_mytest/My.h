#ifndef My_h
#define My_h

#include "arduino.h"

class MyDelta
{
public:
	//MyDelta(double tt);
	double _hh;
	void return1();
	double Mylocation[2][3];
	void LocationCompare();	
	const int MstepPin[3] = { 2,4,6 };
	const int MdirPin[3] = { 3,5,7 };
	const int Msenser[3] = { 8,9,10 };
	const int Mdelaytime = 1;
	MyDelta(double L, double l, double wb, double wp, double up, double sp);
	void setGoalCoordinates(double x, double y, double z, int state);
	void resetArr();
	bool debugFlag;
	int mygoalPos[3];
	int goalPos[3];
	//int posArr[100][4];
	int maxArrIndex;
	//private:
	void _computePara_abc();
	void _computePara_EFG();
	void _computePara_t();
	void _computeAngles();
	void _computeGoalPos();
	int _state;
	double _rat;
	double _L, _l, _wb, _wp, _up, _sp;
	double _a, _b, _c;
	double _x, _y, _z;
	double _E[3], _F[3], _G[3];
	double _t1Num[3], _t1Den[3], _t2Num[3], _t2Den[3];
	double _theta1[3], _theta2[3];	 //馬達轉動角度

};

#endif

