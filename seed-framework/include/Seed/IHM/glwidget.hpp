#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

//#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL/QGLWidget>
#include <QWidget>
#include <iostream>
#include <memory>

class Engine;
class Scene;
class Camera;

class GlWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit GlWidget(int framePerSecond = 0, QWidget *parent = 0);
	~GlWidget();

	void initializeGL() = 0;
	void paintGL() = 0;
	void resizeGL(int width, int height) = 0;
	void keyPressEvent( QKeyEvent *keyEvent );

	public slots:
	void timeOutSlot();

private:
	QTimer *t_timer;
	Engine *engine;
	std::shared_ptr<Scene> scene;
	Camera *camera;

};

#endif