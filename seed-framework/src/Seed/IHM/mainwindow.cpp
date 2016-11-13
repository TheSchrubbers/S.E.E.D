#include "Seed/IHM/mainwindow.hpp"
#include "Seed/IHM/myglwidget.hpp"
#include <QGraphicsScene>
#include "Seed/IHM/toolbars/uptoolbar.hpp"
#include <QMenuBar>
#include <QMenu>


MainWindow::MainWindow()
{
	init();
}

MainWindow::MainWindow(QRect r)
{    
	init();
	setMaximumSize(r.width(), r.height());
	showMaximized();
}

void MainWindow::init()
{
    //QSurfaceFormat::setDefaultFormat(format);

    QStatusBar *qStatusBar = new QStatusBar();
    qStatusBar->showMessage("FPS : ");

    setStatusBar(qStatusBar);

    m_myGLWidget = new MyGLWidget(qStatusBar, this);
    setCentralWidget(m_myGLWidget);

    QMenu *fileMenu = new QMenu("&Fichier");

    QAction *importAction = new QAction(tr("Ouvrir"), this);
    importAction->setShortcut(QKeySequence("Ctrl+O"));
    importAction->setIcon(QIcon("icones/openbutton.png"));
    //QObject::connect(importerAction, SIGNAL(triggered()), this, SLOT(openMokaModel()));
    fileMenu->addAction(importAction);

    QAction *saveAction = new QAction(tr("Enregistrer"), this);
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    saveAction->setIcon(QIcon("icones/savebutton.png"));
    //QObject::connect(enregistrerAction, SIGNAL(triggered()), this, SLOT(saveModel()));
    fileMenu->addAction(saveAction);

    menuBar()->addMenu(fileMenu);

    QToolBar *uptoolbar = new QToolBar();
    uptoolbar->addAction(importAction);
    uptoolbar->addAction(saveAction);

    addToolBar(Qt::TopToolBarArea, uptoolbar);


    setWindowTitle(tr("Seed-Graphics"));
}