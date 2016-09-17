#include "LinkElement.h"
#include <QSet>
#include <graphics/LinkNMItem.h>


LinkElement::~LinkElement()
{
    for (Element *elem : _elements)
        elem->removeLink(this);
}

QGraphicsItem *LinkElement::createGraphicsItem()
{
    QSet<QGraphicsItem *> items;
    for (Element *elem : _elements)
    {
        elem->addLink(this);
        items.insert(elem->item());
    }
    _item = new LinkNMItem(this, _scenePos, items);
    _item->setFlag(QGraphicsItem::ItemIsMovable, true);
    _item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    return _item;
}

void LinkElement::removeElement(Element *elem)
{
    static_cast<LinkNMItem*>(_item)->removeItem(elem->item());
    _elements.remove(elem);
}
