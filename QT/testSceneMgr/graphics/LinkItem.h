#ifndef LINKITEM_H
#define LINKITEM_H

#include "GraphicItem.h"

#include <QGraphicsItem>



class LinkItem : public QGraphicsItem, public GraphicItem
{
public:
    LinkItem(Element *elem, const QPointF &movePoint);


    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);


protected:
    QPointF _movablePoint;
    QRectF  _movableRect;
    bool    _isMoving;

    static const ushort SHAPE_MOVABLE_RECT = 10;
};

#endif // LINKITEM_H
