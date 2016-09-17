#ifndef DELETEELEMENTACTION_H
#define DELETEELEMENTACTION_H

#include <QObject>
#include "mainwindow.h"

class Element;

class DeleteElementAction : public QObject
{
    Q_OBJECT

public:
    DeleteElementAction(Element *elem):_element(elem){}

public slots:
    void remove() {MainWindow::getInstance()->removeElement(_element);deleteLater();}

private:
    Element *_element;
};

#endif // DELETEELEMENTACTION_H
