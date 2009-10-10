#ifndef QD3DWIDGET_H
#define QD3DWIDGET_H

#include <QWidget>

class QD3DWidget : public QWidget
{
	Q_OBJECT

public:
	QD3DWidget(QWidget *parent = 0);
	~QD3DWidget();

	//QSize minimumSizeHint() const;
	//QSize sizeHint() const;

	QPaintEngine *paintEngine() const;

	void rotateBy(float yAngle);
public Q_SLOTS:
		virtual void updateD3D();
protected:
	virtual void initializeD3D();
	virtual void paintD3D();

	void paintEvent(QPaintEvent*);

	virtual void d3dInit();
	virtual void d3dDraw();

	bool initialized() const{ return m_bInit; }
private:
	bool m_bInit;
};

#endif // QD3DWIDGET_H
