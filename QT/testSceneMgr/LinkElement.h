#ifndef LINKELEMENT_H
#define LINKELEMENT_H

#include "Element.h"

#include <QSet>



class LinkElement : public Element
{
public:
    LinkElement(const QSet<Element*> &elems, const QPointF & pos = QPointF(0,0)):
        Element(pos), _elements(elems){}
    ~LinkElement();

    QGraphicsItem *createGraphicsItem();

    void removeElement(Element* elem);

private:
    QSet<Element*> _elements;
};

#endif // LINKELEMENT_H
