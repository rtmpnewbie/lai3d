#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include <QtGui/QMainWindow>
//#include "ui_WorldEditor.h"

class QD3DWidget;

class WorldEditor : public QMainWindow
{
	Q_OBJECT

public:
	WorldEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~WorldEditor();

private slots:
	void rotateOneStep();

private:
	//Ui::WorldEditorClass ui;

	QD3DWidget* m_pD3DWidget;
};

#endif // WORLDEDITOR_H
