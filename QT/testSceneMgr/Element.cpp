#include "DeleteElementAction.h"
#include "Element.h"
#include "LinkElement.h"
#include <QMenu>
#include <graphics/ComplexItem.h>


Element::~Element()
{
    for (LinkElement * link : _links)
    {
        link->removeElement(this);
    }
    delete _item;
}

QGraphicsItem *Element::createGraphicsItem()
{
    _item = new ComplexItem(this);
    _item->setFlag(QGraphicsItem::ItemIsMovable, true);
    _item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    return _item;
}

void Element::showMenu()
{
    QMenu menu;
    QAction *qAction = menu.addAction("Delete");
    qAction->setEnabled(true);
    DeleteElementAction *delAction = new DeleteElementAction(this);
    QObject::connect(qAction, SIGNAL(triggered()), delAction, SLOT(remove()));

    menu.exec(QCursor::pos());
}
