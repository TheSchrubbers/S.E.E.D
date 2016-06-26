#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtCore>
//#include <QWidget>
#include <QMainWindow>
#include <QString>
#include <iostream>

class GlWidget;

class Window : public QMainWindow
{
	Q_OBJECT
public:
	Window(QString name);
	~Window();
private:
	GlWidget *glwidget;
};

#endif