#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Element.h"
#include "LinkElement.h"
#include <graphics/LinkItem.h>

MainWindow *MainWindow::_mainWin = Q_NULLPTR;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), _scene(Q_NULLPTR)
{
    ui->setupUi(this);
    ui->graphicsView->setOptimizationFlag(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setInteractive(true);
    ui->graphicsView->setMouseTracking(false);
//    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFrameStyle(QFrame::NoFrame);
//    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);


    _scene = new QGraphicsScene();
    ui->graphicsView->setScene(_scene);


    addElement();
    addElement(QPointF(100,0));
    addElement(QPoint(50,100));

    LinkElement *link = new LinkElement(_elements);
    _links.insert(link);
    _scene->addItem(link->createGraphicsItem());
    _scene->update();
}

MainWindow::~MainWindow()
{     
    delete _scene;
    delete ui;
}

void MainWindow::addElement(const QPointF &pos, const QSize &size)
{
    Element *elem = new Element(pos, size);
    _scene->addItem(elem->createGraphicsItem());
    _elements.insert(elem);
}

void MainWindow::removeElement(Element *elem)
{

    if (_elements.remove(elem) || _links.remove(elem))
    {
        _scene->removeItem(elem->item());
        _scene->update();
        delete elem;
    }
}

MainWindow *MainWindow::getInstance()
{
    if (!MainWindow::_mainWin)
        _mainWin = new MainWindow();
    return _mainWin;
}
