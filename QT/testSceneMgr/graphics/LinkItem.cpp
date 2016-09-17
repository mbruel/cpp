#include "LinkItem.h"

#include <QGraphicsSceneMouseEvent>
#include "Element.h"

LinkItem::LinkItem(Element *elem, const QPointF &movePoint) :
    QGraphicsItem(), GraphicItem(elem), _movablePoint(movePoint),
    _movableRect(_movablePoint, QSize(0,0)), _isMoving(false)
{
    _movableRect.adjust(-5,-5,5,5);
}

void LinkItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    QPointF pos = event->scenePos();
    if (_movableRect.contains(pos))
        _isMoving = true;
}

void LinkItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isMoving)
    {
        _movablePoint = event->scenePos();
        update();
    }
}

void LinkItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isMoving)
    {
        QGraphicsItem::mouseReleaseEvent(event);
        _movablePoint = event->scenePos();
        _isMoving = false;
        _element->setPos(_movablePoint);
        prepareGeometryChange();
    }
}

void LinkItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (_element)
    {
        _element->showMenu();
    }
    event->accept();
}
