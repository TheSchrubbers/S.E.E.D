#include <QtCore>
#include <QApplication>
#include <QWidget>
#include <window.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Window window(QString("Seed-engine"));
	window.showMaximized();
	window.show();

	return app.exec();
}