#include "DeltaRobInverseKin.h"

#define L_UPPER 0.100
#define L_LOWER 0.220
#define WB 0.109
#define WP 0.040
#define UP 0.080
#define SP 0.138

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
int16_t x;
int16_t y;
int16_t z;

void setup()
{
	Serial.begin(115200);
	delta.debugFlag = false;
}

void loop()
{
	if (Serial.available()) {
		x = Serial.parseInt();
		y = Serial.parseInt();
		z = Serial.parseInt();

		Serial.print("input (");
		Serial.print(x);
		Serial.print(", ");
		Serial.print(y);
		Serial.print(", ");
		Serial.print(z);
		Serial.println(")");

		Serial.print("Start at ");
		Serial.println(micros());	//千分之一秒
		delta.setGoalCoordinates(x * 0.001, y * 0.001, z * 0.001, 0);
		Serial.print("End at ");
		Serial.println(micros());

		Serial.print("output (");
		Serial.print(delta.mygoalPos[0]);
		Serial.print(", ");
		Serial.print(delta.mygoalPos[1]);
		Serial.print(", ");
		Serial.print(delta.mygoalPos[2]);
		Serial.println(")");
		Serial.println();
	}
}
