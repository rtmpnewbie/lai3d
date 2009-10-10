#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <QtGui/QMainWindow>
#include "CTryCustomWidgets.h"

class HelloWorld : public QMainWindow
{
	Q_OBJECT

public:
	HelloWorld(QWidget *parent = 0, Qt::WFlags flags = 0);
	~HelloWorld();

private:
	Ui::CTryCustomWidgetsClass ui;
};

#endif // HELLOWORLD_H
