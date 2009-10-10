#include <QtGui/QApplication>
#include "WorldEditor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WorldEditor w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
