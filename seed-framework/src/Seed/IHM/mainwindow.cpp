#include <mainwindow.hpp>
#include <myglwidget.hpp>
#include <QGraphicsScene>
#include <toolbars/uptoolbar.hpp>
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
	QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

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