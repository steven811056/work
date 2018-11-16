// Robot Simulator.cpp : file di progetto principale.

#include "stdafx.h"
#include "Form1.h"

using namespace RobotSimulator;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Attivare gli effetti visivi di Windows XP prima di creare i controlli
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Creare la finestra principale ed eseguirla
	Application::Run(gcnew Form1());
	return 0;
}
