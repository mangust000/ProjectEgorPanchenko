#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace FORMCALC;

[STAThread]

int main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	FORMCALC::MyForm form;
	Application::Run(% form);
	return 0;
}