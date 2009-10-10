#ifndef CTRYCUSTOMWIDGETS_H
#define CTRYCUSTOMWIDGETS_H

#include <QWidget>
#include "ui_CTryCustomWidgets.h"

class CTryCustomWidgets : public QWidget
{
	Q_OBJECT

public:
	CTryCustomWidgets(QWidget *parent = 0);
	~CTryCustomWidgets();

private:
	Ui::CTryCustomWidgetsClass ui;
};

#endif // CTRYCUSTOMWIDGETS_H
