#include "stdafx.h"
#include "MyForm.h"


using namespace System::Drawing;
ref struct Point3D
{
	double x;
	double y;
	double z;
};
ref struct tuple
{
	double t1;
	double t2;
	double t3;
	double t4;
	double t5;
	double t6;
};
public ref class Robot
{
	const double pi = 3.14159265359;
	const double dToR = 1.74532925199432E-02;

//private:double atan2(  double y, double x);

	   double h, a1, a2, a3,  a5,a6,d1, d4,  d6; //dimension of the robot a is x direction, d  z-direction
	double _t1, _t2, _t3, _t4, _t5, _t6; //dimension of the robot a is x direction, d  z-direction
public:RobotSimulator::MyForm^ f;
public:tuple^  MoveToMotorAngles(double t1, double t2, double  t3, double  t4, double  t5, double  t6);
public:tuple^ MoveToPositions(double x, double y, double  z, double  a, double  b, double  c);
public:tuple^ GetPosition();
	   System::Windows::Forms::PictureBox ^PicBox;
 
	System::Drawing::Bitmap^ part;
private:double round2(double x, int n);
private:tuple^ inverse(double x, double y, double z, double a, double b, double c);
private:Point pToChart(Point3D^ p);
private:Point pToChart(double _x, double _y, double _z);
private: Point3D^ Arm1(double t1, double _x, double _y, double _z);
private: void DrawArm1(Graphics^ g, Pen^ rPen, double t1);
private: Point3D^ Arm2(double t1, double t2, double _x, double _y, double _z);
private: void DrawArm2(Graphics^ g, Pen^ rPen, double t1, double t2);
private: Point3D^ Arm3(double t1, double t2, double t3, double _x, double _y, double _z);
private: void DrawArm3(Graphics^ g, Pen^ rPen, double t1, double t2, double t3);
private: Point3D^ Arm4(double t1, double t2, double t3, double t4, double _x, double _y, double _z);
private: void DrawArm4(Graphics^ g, Pen^ rPen, double t1, double t2, double t3, double t4);
private: Point3D^ Arm5(double t1, double t2, double t3, double t4, double t5, double _x, double _y, double _z);
private: void DrawArm5(Graphics^ g, Pen^ rPen, double t1, double t2, double t3, double t4, double t5);
private: Point3D^ Arm6(double t1, double t2, double t3, double t4, double t5, double t6, double _x, double _y, double _z);
private: void DrawArm6(Graphics^ g, Pen^ rPen, double t1, double t2, double t3, double t4, double t5, double t6);


public: double x, y;

public: Robot(double _x,double _y); //size of the image
public:	void draw( double t1, double  t2, double  t3, double  t4, double  t5, double  t6);
public:Bitmap^ GetImage();
public:void Resize(double _x, double _y);

	//	void getShape(double * b, double a, double d);
};