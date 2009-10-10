#include "WorldEditor.h"


#include <QtGui>
#include "QD3DWidget.h"
#include "Matrices.h"

WorldEditor::WorldEditor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	resize(QSize(600, 400));
	QGridLayout *mainLayout = new QGridLayout;

	m_pD3DWidget = new QD3DWidget(this);
	mainLayout->addWidget(m_pD3DWidget, 0, 0);

	setLayout(mainLayout);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
	timer->start(20);

	setWindowTitle(tr("WorldEditor based Qt"));
}

WorldEditor::~WorldEditor()
{

}

void WorldEditor::rotateOneStep()
{
	if (m_pD3DWidget)
	{
		m_pD3DWidget->rotateBy( 20 * (2.0f * D3DX_PI)/1000.0f );
	}
}