#include <QtGui/QApplication>
#include "helloworld.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HelloWorld w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
