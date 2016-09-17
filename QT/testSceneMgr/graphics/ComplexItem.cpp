#include "ComplexItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>
#include "Element.h"

ComplexItem::ComplexItem(Element *elem):QGraphicsItem(), GraphicItem(elem)
{setPos(_element->pos());}

QRectF ComplexItem::boundingRect() const
{
    return QRectF(QPointF(0,0), _element->size());
}

void ComplexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::green, 2));
    painter->drawRect(boundingRect());
}

QPointF ComplexItem::centerPoint() const
{
    return sceneBoundingRect().center();
}

void ComplexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void ComplexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void ComplexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void ComplexItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (_element)
    {
        _element->showMenu();
    }
    event->accept();
}
