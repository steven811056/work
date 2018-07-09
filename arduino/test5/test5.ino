
const int stepPin = 4;
const int dirPin = 5;

int reset1 = 9;

void setup()
{	
	Serial.begin(9600);
	Serial.println("setup--start");
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(reset1, INPUT);
	pinMode(10, OUTPUT);
}
void loop()
{  
	digitalWrite(10, HIGH);
	Serial.println("start");
	if (digitalRead(reset1) != 1)
	{
		Serial.println(digitalRead(reset1));
		digitalWrite(dirPin, LOW);
		delay(30);
		for (int i = 0; i<18; i++)
		{ /*  往極限開關反方向轉18圈，大約2.025度，如果精度是1/16，
		  為了讓手臂放開極限開關  */
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(500);
			digitalWrite(stepPin, LOW);
			delayMicroseconds(500);
		}
		delay(1000);
		
	}
	if (digitalRead(reset1) == 1)
	{
		Serial.println(digitalRead(reset1));
		digitalWrite(dirPin, HIGH);
		delay(30);
		for (int i = 0; i<18; i++)
		{ /*  往極限開關反方向轉18圈，大約2.025度，如果精度是1/16，
		  為了讓手臂放開極限開關  */
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(500);
			digitalWrite(stepPin, LOW);
			delayMicroseconds(500);
		}
		delay(1000);
	}
	
	//digitalWrite(dirPin, HIGH);
	//delay(30);
	//for (int i = 0; i<18; i++)
	//{ /*  往極限開關反方向轉18圈，大約2.025度，如果精度是1/16，
	//  為了讓手臂放開極限開關  */
	//	digitalWrite(stepPin, HIGH);
	//	delayMicroseconds(500);
	//	digitalWrite(stepPin, LOW);
	//	delayMicroseconds(500);
	//}
	//delay(2000);
	
		
}
