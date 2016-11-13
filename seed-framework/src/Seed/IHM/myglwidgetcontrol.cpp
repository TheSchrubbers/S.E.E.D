#include "Seed/Graphics/engine/engine.hpp"
#include "Seed/IHM/myglwidget.hpp"
#include <QObject>
#include <QKeyEvent>
#include <glm/gtc/quaternion.hpp>


using namespace glm;

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = event->globalPos();
    m_Vmat = m_engine->getScene()->getCamera()->getViewMatrix();
    m_uVec = normalize(vec3(m_Vmat[1][0], m_Vmat[1][1], m_Vmat[1][2]));
    m_rVec = normalize(vec3(m_Vmat[0][0], m_Vmat[0][1], m_Vmat[0][2]));
    m_speed = m_engine->getScene()->getCamera()->getMouseSpeed();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    double deltaX = (-m_mousePosition.x() + event->globalX()) * m_speed;
    double deltaY = (-m_mousePosition.y() + event->globalY()) * m_speed;
    glm::mat4 m = mat4_cast(rotate(quat_cast(m_Vmat), (float)deltaX, m_uVec));// * rotate(quat_cast(m_Vmat), (float)deltaY, m_rVec));
    m_engine->getScene()->getCamera()->setViewMatrix(m);
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