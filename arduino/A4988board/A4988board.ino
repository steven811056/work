#define EN           8       //步進電機使能端，低電平有效
#define X_DIR     5       //X軸 步進電機方向控制
#define Y_DIR     6       //y軸 步進電機方向控制
#define Z_DIR     7       //z軸 步進電機方向控制
#define X_STP     2       //x軸 步進控制
#define Y_STP     3       //y軸 步進控制
#define Z_STP     4       //z軸 步進控制

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
	digitalWrite(dirPin, dir);
	delay(50);
	for (int i = 0; i < steps; i++) {
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(800);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(800);
	}
}

void setup() {//將步進電機用到的IO管腳設置成輸出
	pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
	pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
	pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
	pinMode(EN, OUTPUT);
	digitalWrite(EN, LOW);
}
void loop() {
	step(false, X_DIR, X_STP, 200); //X軸電機 反轉1圈，200步為一圈
	step(false, Y_DIR, Y_STP, 200); //y軸電機 反轉1圈，200步為一圈
	step(false, Z_DIR, Z_STP, 200); //z軸電機 反轉1圈，200步為一圈
	delay(1000);
	step(true, X_DIR, X_STP, 200); //X軸電機 正轉1圈，200步為一圈
	step(true, Y_DIR, Y_STP, 200); //y軸電機 正轉1圈，200步為一圈
	step(true, Z_DIR, Z_STP, 200); //z軸電機 正轉1圈，200步為一圈
	delay(1000);
}