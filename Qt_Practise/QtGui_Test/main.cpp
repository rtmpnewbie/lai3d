#include <QtCore/QCoreApplication>


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CStringA strMFC("canvas.xml");

	const char* pszStr = strMFC.GetBuffer();

	::printf_s("%s\n", pszStr);

	QString qStr = pszStr;
	return a.exec();
}
