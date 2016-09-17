#ifndef LINKNMITEM_H
#define LINKNMITEM_H

#include "LinkItem.h"

#include <QSet>



class LinkNMItem : public LinkItem
{
public:
    LinkNMItem(Element *link, const QPointF &movePoint, const QSet<QGraphicsItem*> &items):
        LinkItem(link, movePoint), _items(items){}

    void removeItem(QGraphicsItem *item);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QSet<QGraphicsItem*> _items;
};

#endif // LINKNMITEM_H
