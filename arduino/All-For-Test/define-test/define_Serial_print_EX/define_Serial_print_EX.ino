#define MySerial Serial
#define p(msg) MySerial.print(msg);
#define pl(msg) MySerial.println(msg);
//�[�����O���F���U��{���p�G�ѰO�[�����]���|�X�{���j���~
void setup() 
{
	MySerial.begin(115200);
}

void loop() 
{
	p("aaaaaa");
	pl("");
}
