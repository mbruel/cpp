#ifndef ELEMENT_H
#define ELEMENT_H

#include <QColor>
#include <QPointF>
#include <QSet>
#include <QSize>


class QGraphicsItem;
class LinkElement;

class Element
{
public:
    Element(const QPointF & pos = QPointF(0,0), const QSize & size = QSize(50,50)):
    _scenePos(pos), _size(size){}

    virtual ~Element();

    virtual QGraphicsItem *createGraphicsItem();
    void deleteItem(){delete _item;_item = Q_NULLPTR;}

    void setPos(const QPointF& pos){_scenePos = pos;}
    QGraphicsItem *item() {return _item;}

    void addLink(LinkElement *link) {_links.insert(link);}
    void removeLink(LinkElement *link){_links.remove(link);}

    void showMenu();

    QPointF pos()  const {return _scenePos;}
    QSize   size() const {return _size;}
protected:
    QPointF _scenePos;
    QSize   _size;
    QColor  _color;

    QGraphicsItem      *_item;
    QSet<LinkElement *> _links;
};

#endif // ELEMENT_H
