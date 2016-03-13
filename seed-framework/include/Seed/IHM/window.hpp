#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtCore>
#include <QWidget>
#include <QMainWindow>
#include <QString>
#include <iostream>

class Window : public QMainWindow
{
	Q_OBJECT
public:
	Window(QString name);
	~Window();
};

#endif