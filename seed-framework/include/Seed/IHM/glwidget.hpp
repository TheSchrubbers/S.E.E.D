#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <QOpenGLWidget>
#include <QWidget>
#include <QWindow>
#include <QTimer>
#include <QMessageBox>
#include <QObject>
#include <QKeyEvent>

#include "Seed/Graphics/definitions.hpp"

class GLWidget : public QOpenGLWidget, public openGLFunctions
{
	Q_OBJECT
public:
	explicit GLWidget(int framesPerSecond = 0, QWidget *parent = 0);
	~GLWidget();

	virtual void initializeGL() = 0;
	virtual void resizeGL(int w, int h) = 0;
	virtual void paintGL() = 0;

	//CONTROLS
	virtual void keyPressEvent( QKeyEvent *keyEvent ) = 0;
	virtual void mousePressEvent(QMouseEvent *event) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *event) = 0;
        
public slots:
    virtual void timeOutSlot();

//METHODS
private:
    QTimer *t_Timer;
};

#endif