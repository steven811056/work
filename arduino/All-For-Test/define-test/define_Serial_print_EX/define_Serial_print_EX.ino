#define MySerial Serial
#define p(msg) MySerial.print(msg);
#define pl(msg) MySerial.println(msg);
//加分號是為了讓下方程式如果忘記加分號也不會出現重大錯誤
void setup() 
{
	MySerial.begin(115200);
}

void loop() 
{
	p("aaaaaa");
	pl("");
}
