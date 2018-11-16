String str;
char R;
//---josn
char json[200];
String jsonS;
char jsonC;
int jsonNumber = 0;
//---

void setup() {
	Serial.begin(9600);
}

void loop() 
{
	For_Uart();
}

void For_Uart()
{
	jsonC = ' ';
	jsonS = "";
	int a = 0;
	while (1)
	{
		while (Serial.available())
		{
			jsonC = Serial.read();
			jsonS = jsonS + jsonC;
			if (jsonC == '{')
			{
				a++;
			}
			else if (jsonC == '}')
			{
				a++;
			}
			else;
		}
		if (a == 2)
		{
			Serial.println(jsonS);
			a = 0;
			break;
		}
	}
}