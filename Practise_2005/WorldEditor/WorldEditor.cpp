// WorldEditor.cpp: ����Ŀ�ļ���

#include "stdafx.h"
#include "MainForm.h"

using namespace WorldEditor;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// �ڴ����κοؼ�֮ǰ���� Windows XP ���ӻ�Ч��
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// ���������ڲ�������
	Application::Run(gcnew MainForm());
	return 0;
}
