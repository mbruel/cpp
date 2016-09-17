#ifndef COMPLEXITEM_H
#define COMPLEXITEM_H

#include "GraphicItem.h"
#include <QGraphicsItem>



class ComplexItem : public QGraphicsItem, public GraphicItem
{
public:
    ComplexItem(Element *elem);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPointF centerPoint() const;

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // COMPLEXITEM_H
