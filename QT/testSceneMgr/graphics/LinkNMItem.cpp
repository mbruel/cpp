#include "LinkNMItem.h"
#include <QPainter>
#include <QRectF>

void LinkNMItem::removeItem(QGraphicsItem *item)
{
    if (_items.remove(item))
        prepareGeometryChange();
}

QRectF LinkNMItem::boundingRect() const
{
    QPolygonF polygon;
    polygon << _movablePoint;
//    for (QGraphicsItem * item : _items)
//        polygon << item->sceneBoundingRect().center();

    return polygon.boundingRect().adjusted(-10,-10,10,10);
}

void LinkNMItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    _movableRect = QRectF(_movablePoint, QSize(0,0));
    _movableRect.adjust(-5,-5,5,5);
    painter->setPen(QPen(Qt::red, 2));
    painter->drawEllipse(_movableRect);

    painter->setPen(QPen(Qt::black,2));
    for (QGraphicsItem * item : _items)
        painter->drawLine(_movablePoint, item->sceneBoundingRect().center());

//    painter->setPen(QPen(Qt::blue,2));
//    painter->drawRect(boundingRect());
}
