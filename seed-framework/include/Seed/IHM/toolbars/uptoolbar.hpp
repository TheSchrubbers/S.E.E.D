#ifndef UPTOOLBAR_HPP
#define UPTOOLBAR_HPP

#include <QToolBar>

class QMenu;
class UpToolbar : public QToolBar
{
public:
	UpToolbar();
private:
	QMenu *m_menuFile;
};
#endif