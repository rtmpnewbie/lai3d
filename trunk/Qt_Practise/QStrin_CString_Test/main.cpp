#include <QtCore/QCoreApplication>

#include <afxwin.h>         // MFC ��������ͱ�׼���


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CStringA strMFC("canvas.xml ��������");

	const char* pszStr = strMFC.GetBuffer();

	::printf_s("%s\n", pszStr);

	QString qStr = pszStr;
	::printf_s("%s\n", qStr.toStdString().c_str());

	return a.exec();
}
