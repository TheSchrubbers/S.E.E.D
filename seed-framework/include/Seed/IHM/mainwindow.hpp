#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets>

class MyGLWidget;
class UpToolbar;
class MainWindow : public QMainWindow
{
public:
	explicit MainWindow();
	explicit MainWindow(QRect r);

	void init();
private:
	MyGLWidget *m_myGLWidget;
	UpToolbar *m_uptoolbar;
};

#endif