#ifndef My_h
#define My_h

#include "arduino.h"


class MyDelta
{
public:
	MyDelta();
	MyDelta(double _Mycoordinate1,double  _Mycoordinate2,double  _Mycoordinate3);
	double _hh;
	void return1();
	void PtoP();
	double Mycoordinate[3];
	double Mylocation[2][3];
	double degreeTurn[1][3];
	void LocationCompare();
	void AfterCompare();


	const int MstepPin[3] = { 2,4,6 };
	const int MdirPin[3] = { 3,5,7 };
	const int Msenser[3] = { 8,9,10 };
	const int Mdelaytime = 1;
	

};

extern MyDelta Mdelta;
#endif

