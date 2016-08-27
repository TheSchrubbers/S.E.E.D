#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <glwidget.hpp>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Engine;
class Camera;
class Scene;

class MyGLWidget : public GLWidget
{
	Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    //CONTROLS
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
	Engine *m_engine;
	QPoint m_mousePosition;
    glm::vec3 m_initDirectionVectorCamera, m_initBVectorCamera, m_initUpVectorCamera;
    float m_initWAngle, m_initHAngle;
    glm::quat q;
};

#endif