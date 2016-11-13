#include "Seed/IHM/glwidget.hpp"

GLWidget::GLWidget(int framesPerSecond, QWidget *parent):QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
	if(framesPerSecond == 0)
        t_Timer = NULL;
    else
    {
        int seconde = 1000; // 1 seconde = 1000 ms
        int timerInterval = seconde / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );
    }
}

GLWidget::~GLWidget()
{
}

void GLWidget::timeOutSlot()
{
    update();
}