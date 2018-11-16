#include "stdafx.h"
#include "Robot.h"
#include "Math.h"
#include "MyForm.h"



//Creator: in input size of the image
 Robot::Robot(double _x,double _y)
{
	//'the dimension of th robot
	h = 20;// 'base to First Joint
	a1 = 60;// 'First Joint to second Joint x-move
	d1 = 45; //'First Joint to second Joint z-move

	a2 = 120;// 'second Joint to third Joint x-move
	a3 = 20;//
	d4 = 100;
	a5 = 20;
	a6 = 10;
	d6 = 30;
	
  
	
	x = _x;
	y = _y;
	draw(_t1, _t2, _t3, _t4, _t5, _t6);
	
//f = gcnew RobotSimulator::MyForm;
//	f->Show();


}
 //It draws the Robot at the given angles.
 void  Robot::draw(  double t1, double  t2, double  t3, double  t4, double  t5, double  t6)
 {
	 
	 part = gcnew System::Drawing::Bitmap(x, y);
	 System::Drawing::Graphics^ g;
	 g = System::Drawing::Graphics::FromImage(part);
	 g->FillRectangle(System::Drawing::Brushes::Black, 0, 0, x, y);
	 Pen^ AxPen;
	 AxPen = gcnew Pen(Color::Blue , 3);
	 Pen^ rPen;
	 rPen = gcnew Pen(Color::Red, 3);
	 //Point 3d is my struct for contain a point in the space
	 Point3D^ pi;
	 Point3D^ pf;
	 pi = gcnew Point3D;
	 pf = gcnew Point3D;
	 //these are the main axes
	 pi->z = 0;
	 pf->z = 0;
	 pi->x = 0;
	 pi->y = 0;
	 pf->x =  x ;
	 pf->y = 0;
	 
	 g->DrawLine(AxPen,
		 pToChart(pi),
		 pToChart(pf));
	 
	 pi->x = 0;
	 pi->y = 0;
	 pf->x = 0 ;
	 pf->y = y;
	 g->DrawLine(AxPen,
		 pToChart(pi),
		 pToChart(pf));
	 pi->x = 0;
	 pi->y = 0;
	 pf->x = 0;
	 pf->y = 0;
	 pf->z = y;

	 g->DrawLine(AxPen,
		 pToChart(pi),
		 pToChart(pf));
   

	 //Draws the base
	 for (int i = 0; i < 360; i++)
	 {
		 g->DrawLine(rPen,
			 pToChart(20 * cos(i*dToR), 20 * sin(i*dToR), 0),
			 pToChart(20 * cos((i+1)*dToR), 20 * sin((i+1)*dToR), 0)
			 );
		 g->DrawLine(rPen,
			 pToChart(20 * cos(i*dToR), 20 * sin(i*dToR), h),
			 pToChart(20 * cos((i +1 )*dToR), 20 * sin((i + 1)*dToR), h)
			 );
	 }

	 	 g->DrawLine(rPen,
		 pToChart(0, 20, 0),
		 pToChart(0, 20, h)
		 );
	 	 g->DrawLine(rPen,
		 pToChart(0, -20, h),
		 pToChart(0, -20, 0)
		 );
	
		 //Draw the armes
	 DrawArm1(g, rPen, t1);
	 DrawArm2(g, rPen, t1, t2 );
	 DrawArm3(g, rPen, t1, t2, t3);
	 DrawArm4(g, rPen, t1, t2, t3,t4);
	 DrawArm5(g, rPen, t1, t2, t3, t4,t5);
	 DrawArm6(g, rPen, t1, t2, t3, t4, t5,t6);
 }
 //transforms a 3d Point in a 2D with assonemtrie
 Point Robot::pToChart(Point3D^ p)
 {
	 return pToChart(p->x, p->y, p->z);

 }
 //transforms a 3d Point in a 2D with assonemtrie
 Point Robot::pToChart(double _x, double _y, double _z)
 {
	 Point pr;
	 pr.X = x / 2 + _y - _x * 0.35355339059;
	 pr.Y = y / 2 - _z + _x * 0.35355339059;
	 return pr;

 }
 //Return the image
 Bitmap^ Robot::GetImage()
 {
	 return part;
 }
 //Resize the image
 void Robot::Resize(double _x, double _y)
 {
	 x = _x;
	 y = _y;
	 draw(_t1, _t2, _t3, _t4, _t5, _t6);
 }
 //The equation of the First arm.  Convert the _x,_y,_z, in the coordinate system of the arm. 
 Point3D^  Robot::Arm1(double t1, double _x, double _y,  double _z)
 {
	Point3D^ p;
	p = gcnew Point3D;
	p->x = cos(t1)*_x - sin(t1)*_y;
	p->y = cos(t1)*_y + sin(t1)*_x;
	p->z = h + _z;// +d1;
	return p;

 }
 //If contains the graphical array of points or equation of the arm
 void Robot::DrawArm1(Graphics^ g, Pen^ rPen, double t1)
{
	double parm[5][3] = { { 0,0, 0 },
	{ 0,0, d1 },
	{ a1,0, d1 },
	{ a1,0, 0 },
	{ 0,0, 0 } };

	for (int i = 0; i < 4; i++)
	{
		
		g->DrawLine(rPen,
			pToChart(Arm1(t1, parm[i][0], parm[i][1], parm[i][2])),
			pToChart(Arm1(t1, parm[i + 1][0], parm[i + 1][1], parm[i+1][2]))
			);
	}

}
 //see arm1
Point3D^  Robot::Arm2(double t1, double t2, double _x, double _y, double _z)
{
	Point3D^ p;
	p = gcnew Point3D;
	double s1, c1, s2, c2;
	s1 = sin(t1);
	s2 = sin(t2);
	c1 = cos(t1);
	c2 = cos(t2);

	p->x = s1*_z - c1*s2*_y + c1*c2*_x + a1*c1;
	p->y = -c1*_z - s1*s2*_y + s1*c2*_x + a1*s1;
	p->z = h + c2*_y + s2*_x + d1;
	return p;

}
//see arm1
void Robot::DrawArm2(Graphics^ g, Pen^ rPen, double t1, double t2)
{
	double parm[5][3] = { { 0, 0,-10 },
	{ a2,0, -10 },
	{ a2,0, 10 }, 
	{ 0,0, 10 },
	{ 0,0, -10 }
	};
	for (int i = 0; i < 4; i++)
	{
		g->DrawLine(rPen,
			pToChart(Arm2(t1, t2, parm[i][0], parm[i][1], parm[i][2])),
			pToChart(Arm2(t1, t2, parm[i + 1][0], parm[i + 1][1],  parm[i+1][2]))
			);
	}
		
}
//see arm1
Point3D^  Robot::Arm3(double t1, double t2, double t3, double _x, double _y, double _z)
{
	Point3D^ p;
	p = gcnew Point3D;
	double s1, c1, s2, c2, s3, c3, s23, c23, s4, c4;
	s1 = sin(t1);
	s2 = sin(t2);
	s23 = sin(t2 + t3);
	c1 = cos(t1);
	c2 = cos(t2);
	c23 = cos(t2 + t3);
	
	p->x = s1*_z - c1*s23*_y + c1*c23*_x + a2*c1*c2 + a1*c1;
	p->y = -c1*_z - s1*s23*_y + s1*c23*_x + a2*s1*c2 + a1*s1;
	p->z = h + c23*_y + s23*_x + a2*s2 + d1;
	return p;

}
//see arm1
void Robot::DrawArm3(Graphics^ g, Pen^ rPen, double t1, double t2, double t3  )
{
	double parm[5][3] = { { 0, 0, -10 },
	{ a3, 0, -10 },
	{ a3, 0, 10 },
	{ 0, 0, 10 },
	{ 0, 0, -10 }
	};
	for (int i = 0; i < 4; i++)
	{
		g->DrawLine(rPen,
			pToChart(Arm3(t1, t2,t3, parm[i][0], parm[i][1], parm[i][2])),
			pToChart(Arm3(t1, t2,t3, parm[i + 1][0], parm[i + 1][1], parm[i + 1][2]))
			);
	}
}
//see arm1
Point3D^  Robot::Arm4(double t1, double t2, double t3, double t4, double _x,double _y, double _z)
{
	Point3D^ p;
	p = gcnew Point3D;
	double s1, c1, s2, c2, s3, c3, s23, c23, s4, c4  ;
	s1 = sin(t1);
	s2 = sin(t2);
 
	s23 = sin(t2 + t3);
	s4 = sin(t4);
	c1 = cos(t1);
	c2 = cos(t2);
	 
	c23 = cos(t2 + t3);
	c4 = cos(t4);
	
	p->x = c1*c23*_z + (s1*s4 - c1*s23*c4)*_y + (-c1*s23*s4 - s1*c4)*_x + a3*c1*c23 + a2*c1*c2 + a1*c1;
	p->y = s1*c23*_z + (-c1*s4 - s1*s23*c4)*_y + (c1*c4 - s1*s23*s4)*_x + a3*s1*c23 + a2*s1*c2 + a1*s1;
	p->z = h + s23*_z + c23*c4*_y + c23*s4*_x + a3*s23 + a2*s2 + d1;

	return p;

}
//see arm1
void Robot::DrawArm4(Graphics^ g, Pen^ rPen, double t1, double t2, double t3, double t4 )
{
	double parm[5][3] = { { -10, 0, 0 },
	{ -10, 0,d4 },
	{ 10, 0, d4 },
	{ 10, 0, 0 },
	{ -10, 0, 0 }
	};
	for (int i = 0; i < 4; i++)
	{
		g->DrawLine(rPen,
			pToChart(Arm4(t1, t2, t3,t4, parm[i][0], parm[i][1], parm[i][2])),
			pToChart(Arm4(t1, t2, t3, t4, parm[i + 1][0], parm[i + 1][1], parm[i + 1][2]))
			);
	}

}
//see arm1
Point3D^  Robot::Arm5(double t1, double t2, double t3, double t4, double t5, double _x, double _y, double _z)
{
	Point3D^ p;
	p = gcnew Point3D;
	double s1, c1, s2, c2, s3, c3, s23,c23,s4, c4, s5, c5;
	s1 = sin(t1); 
	s2 = sin(t2);
	s3 = sin(t3);
	s23 = sin(t2 + t3);
	s4 = sin(t4);
	s5 = sin(t5);
	c1 = cos(t1);
	c2 = cos(t2);
	c3 = cos(t3);
	c23 = cos(t2 + t3);
	c4 = cos(t4);
	c5 = cos(t5);
	
	p->x = (c1*s23*s4 + s1*c4)*_z + ((s1*s4 - c1*s23*c4)*c5 - c1*c23*s5)*_y + ((s1*s4 - c1*s23*c4)*s5 + c1*c23*c5)*_x + 
		d4*c1*c23 + a3*c1*c23 + a2*c1*c2 + a1*c1;

	p->y = (s1*s23*s4 - c1*c4)*_z + ((-c1*s4 - s1*s23*c4)*c5 - s1*c23*s5)*_y + ((-c1*s4 - s1*s23*c4)*s5 + s1*c23*c5)*_x +
		d4*s1*c23 + a3*s1*c23 + a2*s1*c2 + a1*s1;
	
	p->z = h -c23*s4*_z + (c23*c4*c5 - s23*s5)*_y + (c23*c4*s5 + s23*c5)*_x + d4*s23 + a3*s23 + a2*s2 + d1;

	return p;

}
//see arm1
void Robot::DrawArm5(Graphics^ g, Pen^ rPen, double t1, double t2, double t3, double t4, double t5)
{
	double parm[5][3] = { { 0, 0, -10 },
	{ a5, 0, -10 },
	{ a5, 0, 10 },
	{ 0, 0, 10 },
	{ 0, 0, -10 }
	};
	for (int i = 0; i < 4; i++)
	{
		g->DrawLine(rPen,
			pToChart(Arm5(t1, t2, t3, t4,t5, parm[i][0], parm[i][1], parm[i][2])),
			pToChart(Arm5(t1, t2, t3, t4, t5, parm[i + 1][0], parm[i + 1][1], parm[i + 1][2]))
			);
	}


}
//see arm1
Point3D^  Robot::Arm6(double t1, double t2, double t3, double t4, double t5, double t6, double _x, double _y, double _z)
{
	Point3D^ p;
	p = gcnew Point3D;
	double s1, c1, s2, c2, s3, c3, s23, c23, s4, c4, s5, c5,s6,c6;
	s1 = sin(t1);
	s2 = sin(t2);
	s3 = sin(t3);
	s23 = sin(t2 + t3);
	s4 = sin(t4);
	s5 = sin(t5);
	s6 = sin(t6);
	c1 = cos(t1);
	c2 = cos(t2);
	c3 = cos(t3);
	c23 = cos(t2 + t3);
	c4 = cos(t4);
	c5 = cos(t5);
	c6 = cos(t6);

	p->x = ((s1*s4 - c1*s23*c4)*s5 + c1*c23*c5)*_z + (((s1*s4 - c1*s23*c4)*c5 - c1*c23*s5)*c6 - (-c1*s23*s4 - s1*c4)*s6)*_y + 
		(((s1*s4 - c1*s23*c4)*c5 - c1*c23*s5)*s6 + (-c1*s23*s4 - s1*c4)*c6)*_x
		+ a5*((s1*s4 - c1*s23*c4)*s5 + c1*c23*c5) + d4*c1*c23 + a3*c1*c23 + a2*c1*c2 + a1*c1;

	p->y = ((-c1*s4 - s1*s23*c4)*s5 + s1*c23*c5)*_z + (((-c1*s4 - s1*s23*c4)*c5 - s1*c23*s5)*c6 - (c1*c4 - s1*s23*s4)*s6)*_y + 
		(((-c1*s4 - s1*s23*c4)*c5 - s1*c23*s5)*s6 + (c1*c4 - s1*s23*s4)*c6)*_x
		+ a5*((-c1*s4 - s1*s23*c4)*s5 + s1*c23*c5) + d4*s1*c23 + a3*s1*c23 + a2*s1*c2 + a1*s1;

	p->z =h + (c23*c4*s5 + s23*c5)*_z + ((c23*c4*c5 - s23*s5)*c6 - c23*s4*s6)*_y + ((c23*c4*c5 - s23*s5)*s6 + c23*s4*c6)*_x + 
		a5*(c23*c4*s5 + s23*c5) + d4*s23 + a3*s23 + a2*s2 + d1;

	return p;

}
//see arm1
void Robot::DrawArm6(Graphics^ g, Pen^ rPen, double t1, double t2, double t3, double t4, double t5, double t6)
{
	double parm[5][3] = { { -20, 0, 0 },
	{ -20, 0, d6 },
	{ 20, 0, d6 },
	{ 20, 0, 0 },
	{ -20, 0, 0 }
	};
	for (int i = 0; i < 4; i++)
	{
		g->DrawLine(rPen,
			pToChart(Arm6(t1, t2, t3, t4, t5, t6, parm[i][0], parm[i][1], parm[i][2])),
			pToChart(Arm6(t1, t2, t3, t4, t5, t6, parm[i + 1][0], parm[i + 1][1], parm[i + 1][2]))
			);
	}

}
//Given the angles of the motor, draw the robot consequently
tuple^  Robot::MoveToMotorAngles(double t1, double t2, double  t3, double  t4, double  t5, double  t6)
{
	_t1 = t1;
	_t2 = t2;
	_t3 = t3;
	_t4 = t4;
	_t5 = t5;
	_t6 = t6;
	draw(t1, t2, t3, t4, t5, t6);
	return GetPosition();

}
//personal function to round at given n places
double Robot::round2(double x, int n)
	 {
				 long v;
				 v =  pow((double)10, n);
				 return round(x*v) / v;

	 }
//Return the actual position and orientation of the top of the arm6
tuple^ Robot::GetPosition()
{
		double nx, ny, nz, sx, sy, sz, ax, ay, az;
		double par = 30 * pi / 180;
		double s1, c1, s2, c2, s3, c3, s23, c23, s4, c4, s5, c5, s6, c6;
		s1 = sin(_t1);
		s2 = sin(_t2);
		s3 = sin(_t3);
		s23 = sin(_t2 + _t3);
		s4 = sin(_t4);
		s5 = sin(_t5);
		s6 = sin(_t6);
		c1 = cos(_t1);
		c2 = cos(_t2);
		c3 = cos(_t3);
		c23 = cos(_t2 + _t3);
		c4 = cos(_t4);
		c5 = cos(_t5);
		c6 = cos(_t6);
	//nx = ((s1*s4 - c1*s23*c4)*c5 - c1*c23*s5)*s6 + (-c1*s23*s4 - s1*c4)*c6;
	//ny = ((-c1*s4 - s1*s23*c4)*c5 - s1*c23*s5)*s6 + (c1*c4 - s1*s23*s4)*c6;
	nz = (c23*c4*c5 - s23*s5)*s6 + c23*s4*c6;
	//sx = ((s1*s4 - c1*s23*c4)*c5 - c1*c23*s5)*c6 - (-c1*s23*s4 - s1*c4)*s6;
	//sy = ((-c1*s4 - s1*s23*c4)*c5 - s1*c23*s5)*c6 - (c1*c4 - s1*s23*s4)*s6;
	sz = (c23*c4*c5 - s23*s5)*c6 - c23*s4*s6;
	ax = (s1*s4 - c1*s23*c4)*s5 + c1*c23*c5;
	ay = (-c1*s4 - s1*s23*c4)*s5 + s1*c23*c5;
	az = c23*c4*s5 + s23*c5;


	//nx = round2(nx, 6);
	//ny = round2(ny, 6);
	nz = round2(nz, 6);
	//sy = round2(sy, 6);
	sz = round2(sz, 6);
	az = round2(az, 6);
	ay = round2(ay, 6);
	ax = round2(ax, 6);
	Point3D^ p = Arm6(_t1, _t2, _t3, _t4, _t5, _t6, 0, 0, d6);
	tuple^t = gcnew tuple;


	t->t1 = p->x;
	t->t2 = p->y;
	t->t3 = p->z;
	//I calculate this value with brute force, see below
	t->t5 = atan2(-az, sqrt(ax*ax + ay*ay)); // it works also with atan2(-az, sqrt(sz*sz + nz*nz)); 
	t->t4 = atan2(ay / cos(t->t5), ax / cos(t->t5))  ;
	t->t6 = atan2(sz / cos(t->t5),  nz / cos(t->t5))-pi/2   ;

	
	//This quoted procedure needs to find, brute force, the right configuration of parameters... there is to go crazy
	//Remember to initiate the class MyForm in Robot::Robot

	//double m[9];
	//System::Text::StringBuilder^ st = gcnew System::Text::StringBuilder("");
	//m[0] = round2(nx,6);
	//m[1] = round2(ny, 6);
	//m[2] = round2(nz, 6);
	//m[3] = round2(sx, 6);
	//m[4] = round2(sy, 6);
	//m[5] = round2(sz, 6);
	//m[6] = round2(ax, 6);
	//m[7] = round2(ay, 6);
	//m[8] = round2(az, 6);

	//for (int i = 0; i < 9;i++)
	//{
	//	for (int j = 0; j < 9; j++)
	//	{

	//		/*for (int k = 0; k < 9; k++)
	//		{
	//		*/	st->Append(i);
	//			st->Append(j);
	//		// st->Append(k);
	//			st->Append("=");
	//			st->Append(atan2(m[i] / cos(t->t5), m[j] / cos(t->t5)) / dToR);
	//			// st->Append(atan2(m[i],sqrt(m[j]*m[j]+m[k]*m[k]))/dToR);
	//			st->Append("\r\n");
 //	/*}*/

	//	}


	//}


	//f->textBox1->Text = st->ToString();




	return t;

}
//Given position and orientation, calculate the angles of the motors
tuple^ Robot::inverse(double x, double y, double z, double a, double b, double c)
{
	tuple^ p;
	p = gcnew tuple;
	double pWx, pWy, pWz, px, py, pz, s3, c3, s2, c2,  xi, fi;
 
	int sgn;
	pWx = x  - cos(a)*cos(b)*(d6 + a5);
	pWy = y - sin(a)*cos(b)*(d6 + a5);
	pWz = z + sin(b)*(d6 + a5);
	double u;
	int sg=1;
	p->t1 = atan2(pWy, pWx);

	
	
	
	if (pWx*pWx + pWy*pWy - a1*a1 < 0)
		sg = -1;
	//distance of the wirst from the Motor 2
	px = pWx -a1 * cos(p->t1);
	py = pWy - a1 * sin(p->t1);
	pz = pWz - d1 - h;
 

 
	
	c3 = (px*px + py*py + pz*pz - pow(a3 + d4, 2) - a2*a2) / (2 * (a3 + d4) * a2);
	s3 =- sqrt(1 - c3*c3);

	p->t3 = atan2(s3, c3);
	
//the follow code show in the MyForm f, some parameters. MyForm get shown in the creator
	//f->textBox1->Text = System::String::Concat("c3num =", (px*px + py*py + pz*pz - pow((a3 + d4), 2) ), "\r\n"
		//	, "mod =", px*px + py*py + pz*pz, "\r\n"
		//	, "pWx =", pWx, "\r\n"
		//	, "pWy =", pWy, "\r\n"
		//	, "pWz =", pWz, "\r\n"
		//	, "px =", px, "\r\n"
		//	, "py =", py, "\r\n"
		//	, "pz =", pz, "\r\n"
		//	, "c3 =", c3, "\r\n"
		//	, "s3 =", s3, "\r\n"
		//	, "t3 =", p->t3, "\r\n"
		//	);
	// http://www.rob.uni-luebeck.de/Lehre/2008w/Robotik/Vorlesung/Robotik1VL5_1_vers1.pdf  pag. 50
		xi = atan2(pz, sg*sqrt(px *px + py*py));// sg is my addition
		fi = atan2((a3 + d4) * s3, ( a2 + (a3 + d4) * c3));

		p->t2 = xi -  fi;
		
		double nx, ny, nz, sx, ax, sa, sb, sc, s1, ca,cb,cc, c1,  c23, s23; 
		sa = sin(a);
		sb = sin(b);
		sc = sin(c);
		s1 = sin(p->t1);
		s23 = sin(p->t3 + p->t2);
		
		ca = cos(a);
		cb = cos(b);
		cc = cos(c);
		c1 = cos(p->t1);
		c23 = cos(p->t3 + p->t2);

		nx = round2(-sb*s23 + cb*c23* cos(a-p->t1),6); 
		ny = round2(-cb*s23*cos(a - p->t1) - sb*c23,6);
		nz = round2(cb * sin(p->t1 - a), 6);
		sx = round2(cb*sc*s23 + (sa*sb*sc + ca*cc)*s1*c23 + (ca*sb*sc - sa*cc)*c1*c23, 6);
		ax = round2(cb*cc*s23 + (sa*sb*cc - ca*sc)*s1*c23 + (sa*sc + ca*sb*cc)*c1*c23, 6);
		
		p->t5 = atan2(sqrt(nz *nz + ny *ny), (nx));
		if (p->t5 < 0)
		{
			p->t4 = atan2((-nz), (-ny));

				p->t5 = atan2(-sqrt(nz *nz+ ny *ny), (nx));
 
		}
		else
		{
			//p->t4 = atan2((nz), (ny));

			//p->t5 = atan2(sqrt(nz *nz + ny *ny), (nx));
			p->t4 = atan2((nz), (ny));
 
		}
		p->t6 = atan2(-sx, -ax);
		if (sg < 0)
		{
			p->t6 = atan2(sx, ax);

			if (p->t6 <0)
				p->t6 = atan2(-sx,- ax);
			else
				p->t6 = atan2(sx, ax);
		}
		
	
			return p;
 
}
//Given position and orientation, calculate the angles of the motors and draw robot consequently
tuple^ Robot::MoveToPositions(double x, double y, double  z, double  a, double  b, double  c)
{
	tuple^ r = inverse(x, y, z, a, b, c);
	draw(r->t1, r->t2, r->t3, r->t4, r->t5, r->t6);
	_t1 = r->t1;
	_t2 = r->t2;
	_t3 = r->t3;
	_t4 = r->t4;
	_t5 = r->t5;
	_t6 = r->t6;
	return r;

}
//double Robot::atan2(double y, double  x)
//{
//
//	if (x > 0)
//		return atan(y / x);
//	else
//	{
//		if (x < 0)
//		{
//			if (y >= 0)
//				return atan(y / x) + pi;
//			else
//				return atan(y / x) - pi;
//		}
//		else
//		{
//			if (y>0) return pi / 2;
//			if (y<0) return -pi / 2;
//			return 0;
//				
//		}
//	}	
//
//
//}
//

