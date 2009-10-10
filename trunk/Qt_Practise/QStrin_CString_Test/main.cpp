#include <QtCore/QCoreApplication>

#include <afxwin.h>         // MFC 核心组件和标准组件


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CStringA strMFC("canvas.xml 测试中文");

	const char* pszStr = strMFC.GetBuffer();

	::printf_s("%s\n", pszStr);

	QString qStr = pszStr;
	::printf_s("%s\n", qStr.toStdString().c_str());

	return a.exec();
}
