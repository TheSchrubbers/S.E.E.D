#include <toolbars/uptoolbar.hpp>
#include <QMenu>

UpToolbar::UpToolbar()
{
    
    this->buttonsBar->addAction(this->importerAction);

    menuImport->addAction(this->enregistrerAction);
    this->buttonsBar->addAction(this->enregistrerAction);

    this->addToolBar(Qt::TopToolBarArea,  this->buttonsBar);
}

