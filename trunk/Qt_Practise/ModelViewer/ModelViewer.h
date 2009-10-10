#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QtGui/QMainWindow>
#include "ui_ModelViewer.h"

class ModelViewer : public QMainWindow
{
	Q_OBJECT

public:
	ModelViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ModelViewer();

private:
	Ui::ModelViewerClass ui;
};

#endif // MODELVIEWER_H
