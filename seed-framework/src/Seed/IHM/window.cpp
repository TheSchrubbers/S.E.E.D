#include "window.hpp"
#include "glwidget.hpp"

Window::Window(QString name)
{
	this->setWindowTitle(name);
	this->glwidget = new GlWidget();
	this->setCentralWidget(this->glwidget);
}

Window::~Window()
{
	delete glwidget;
}

