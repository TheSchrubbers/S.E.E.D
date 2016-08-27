#include <Seed/Graphics/engine/engine.hpp>
#include <myglwidget.hpp>
#include <QObject>
#include <QKeyEvent>
#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>


using namespace glm;

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = event->globalPos();
    m_initDirectionVectorCamera = m_engine->getScene()->getCamera()->getDirection();
    m_initUpVectorCamera = m_engine->getScene()->getCamera()->getUp();
    m_initBVectorCamera = cross(m_initUpVectorCamera, m_initDirectionVectorCamera);
    printVec3(m_initDirectionVectorCamera);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	double speed = m_engine->getScene()->getCamera()->getMouseSpeed();
    double deltaX = (-m_mousePosition.x() + event->globalX()) * speed;
    double deltaY = (m_mousePosition.y() + event->globalY()) * speed;
    quat q;
    q = rotate(q, (float)deltaX, m_initUpVectorCamera);
    q = rotate(q, (float)deltaY, m_initBVectorCamera);
    vec3 dirVec = q * m_initDirectionVectorCamera;
    //q2 = rotate(q2, (float)deltaX, m_initBVectorCamera);
    //vec3 upVec = q2 * m_initUpVectorCamera;
    m_engine->getScene()->getCamera()->setViewMatrix(dirVec, m_initUpVectorCamera);
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

void MyGLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Escape:
            close();
            break;
    }
}