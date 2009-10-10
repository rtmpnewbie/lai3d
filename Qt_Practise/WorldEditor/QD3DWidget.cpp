#include "QD3DWidget.h"

#include "Matrices.h"

QD3DWidget::QD3DWidget(QWidget *parent)
	: QWidget(parent)
	, m_bInit(false)
{
	resize(QSize(400, 300));
	setAttribute(Qt::WA_PaintOnScreen, true);
}

QD3DWidget::~QD3DWidget()
{

}

void QD3DWidget::initializeD3D()
{
	InitD3D(/*this->topLevelWidget()->*/winId());
	InitGeometry();

	m_bInit = true;
}

void QD3DWidget::paintD3D()
{
	// lyl: 真正要画的东西放这儿
	Render();
}

void QD3DWidget::paintEvent( QPaintEvent* )
{
	if (updatesEnabled()) 
	{
		d3dDraw();
	}
}

void QD3DWidget::d3dInit()
{
	initializeD3D();
}

void QD3DWidget::d3dDraw()
{
	if (!initialized())
	{
		d3dInit();
	}
	paintD3D();
}

//QSize QD3DWidget::minimumSizeHint() const
//{
//	return QSize(50, 50);
//}
//
//QSize QD3DWidget::sizeHint() const
//{
//    return QSize(200, 200);
//}

QPaintEngine * QD3DWidget::paintEngine() const
{
	return NULL;
}

void QD3DWidget::rotateBy( float yAngle )
{
	g_fYAngle += yAngle;
	if (g_fYAngle > (2.0f * D3DX_PI))
	{
		g_fYAngle -= (2.0f * D3DX_PI);
	}
	updateD3D();
}

void QD3DWidget::updateD3D()
{
	if (updatesEnabled())
		d3dDraw();
}