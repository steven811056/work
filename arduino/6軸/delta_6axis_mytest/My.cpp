#include "Arduino.h"
#include "My.h"
#include <math.h>

MyDelta::MyDelta(double L, double l, double wb, double wp, double up, double sp)
{
		// The parameters here match up with the onces mentioned
		// in Robert L. William's Delta Robot Publication page 5 & 6 (see README.md)
		// Units: [m]
		// DelataRobInverseKin��_L...���Oprivate���A�]���ݭn�A�إߤ@��DelataRobInverseKin�̭���
		// �ݩʡA���٬O���W��DelataRobInverseKin�A��s��DelataRobInverseKin�̭���L���
		// private��_L�̭��A���쥻DelataRobInverseKin�̭�����L�ݩʥi�H�B��C
		_L = L;
		_l = l;
		_wb = wb;
		_wp = wp;
		_up = up;
		_sp = sp;

		maxArrIndex = 0;

		_rat = 180 / PI;

		debugFlag = false;
	}


	void MyDelta::setGoalCoordinates(double x, double y, double z, int state)
	{
		_x = x;
		_y = y;
		_z = z;
		_state = state;
		_computeAngles();
	}
	
	void MyDelta::_computeAngles()
	{
		// The parameters here match up with the onces mentioned
		// in Robert L. William's Delta Robot Publication page 12 (see README.md)
		// Units: [m], [rad]

		_computePara_abc();
		_computePara_EFG();
		_computePara_t();
		_computeGoalPos();
	}

	void MyDelta::_computePara_abc()
	{		
		_a = _wb - _up;
		_b = _sp / 2.0 - sqrt(3.0) / 2.0 * _wb;
		_c = _wp - _wb / 2.0;

		if (debugFlag) {
			Serial.print("a: "); Serial.println(_a);
			Serial.print("b: "); Serial.println(_b);
			Serial.print("c: "); Serial.println(_c);
		}
	}


	// - - - - - - - - - - - - - - - - - - - - - - -
	// - - - DeltaRobInverseKin COMPUTEPARA_EFG  - -
	// - - - - - - - - - - - - - - - - - - - - - - -
	void MyDelta::_computePara_EFG()
	{
		// The parameters here match up with the onces mentioned
		// in Robert L. William's Delta Robot Publication page 12 (see README.md)
		// Units: [m]

		_E[0] = 2 * _L * (_y + _a);
		_F[0] = 2 * _z * _L;
		_G[0] = pow(_x, 2) + pow(_y, 2) + pow(_z, 2) + pow(_a, 2) + pow(_L, 2) + 2 * _y * _a - pow(_l, 2);

		_E[1] = -_L * (sqrt(3.0)*(_x + _b) + _y + _c);
		_F[1] = 2 * _z * _L;
		_G[1] = pow(_x, 2) + pow(_y, 2) + pow(_z, 2) + pow(_b, 2) + pow(_c, 2) + pow(_L, 2)
			+ 2 * (_x * _b + _y * _c) - pow(_l, 2);

		_E[2] = _L * (sqrt(3.0)*(_x - _b) - _y - _c);
		_F[2] = 2 * _z * _L;
		_G[2] = pow(_x, 2) + pow(_y, 2) + pow(_z, 2) + pow(_b, 2) + pow(_c, 2) + pow(_L, 2)
			+ 2 * (-_x * _b + _y * _c) - pow(_l, 2);

		if (debugFlag) {
			for (int i = 0; i < 3; i++) {
				Serial.print("i: ");    Serial.println(i);
				Serial.print("E[i]: "); Serial.println(_E[i]);
				Serial.print("F[i]: "); Serial.println(_F[i]);
				Serial.print("G[i]: "); Serial.println(_G[i]);
			}
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - -
	// - - - DeltaRobInverseKin COMPUTEPARA_t  - - -
	// - - - - - - - - - - - - - - - - - - - - - - -
	void MyDelta::_computePara_t()
	{
		// The parameters here match up with the onces mentioned
		// in Robert L. William's Delta Robot Publication page 12 (see README.md)
		// Units: [m]

		for (int i = 0; i < 3; i++) {
			//the first root (+ in the equation)
			_t1Num[i] = -_F[i] + sqrt(pow(_E[i], 2) + pow(_F[i], 2) - pow(_G[i], 2));
			_t1Den[i] = _G[i] - _E[i];

			//the second root (- in the equation)
			_t2Num[i] = -_F[i] - sqrt(pow(_E[i], 2) + pow(_F[i], 2) - pow(_G[i], 2));
			_t2Den[i] = _G[i] - _E[i];

			_theta1[i] = 2 * atan(_t1Num[i] / _t1Den[i]); //�ϥ�����
			_theta2[i] = 2 * atan(_t2Num[i] / _t2Den[i]);
		}

		if (debugFlag) {
			for (int i = 0; i < 3; i++) {
				Serial.print("i: ");    Serial.println(i);
				Serial.print("t1[i]: "); Serial.println(_t1Num[i] / _t1Den[i]);
				Serial.print("t2[i]: "); Serial.println(_t2Num[i] / _t2Den[i]);
				Serial.print("theta1[i]: "); Serial.println(_theta1[i]);
				Serial.print("theta2[i]: "); Serial.println(_theta2[i]);
			}
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - -
	// - - - DeltaRobInverseKin COMPUTEGOALPOS - - -
	// - - - - - - - - - - - - - - - - - - - - - - -
	void MyDelta::_computeGoalPos()
	{
		for (int i = 0; i < 3; i++) {
			//posArr[maxArrIndex][i] = (int)(_theta2[i] * _rat);
			goalPos[i] = (int)(_theta2[i] * _rat);
			mygoalPos[i] = _t2Den[i];
		}
		//posArr[maxArrIndex][3] = _state;

		maxArrIndex++;

		if (debugFlag) {
			for (int i = 0; i < 4; i++) {
				Serial.print("i: ");
				Serial.println(i);
				Serial.print("goalPos[i]: ");
				Serial.println(goalPos[i]);
			}
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - -
	// - - - DeltaRobInverseKin RESET ARR  - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - -
	void MyDelta::resetArr()
	{
		for (int i = 0; i <= 499; i++) {
			for (int j = 0; j <= 3; j++) {
				//posArr[i][j] = 0;
			}
		}
		maxArrIndex = 0;
	}
