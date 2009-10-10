#include <QtGui/QApplication>
#include "ModelViewer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ModelViewer w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
