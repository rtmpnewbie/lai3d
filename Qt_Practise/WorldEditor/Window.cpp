#include "Window.h"

Window::Window(QWidget *parent)
	: QWidget(parent)
{
	m_pD3DWidget = new QD3DWidget(this);
	setWindowTitle(tr("»ùÓÚQtµÄWorldEditor"));
}

Window::~Window()
{

}
