#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class Element;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();

    void addElement(const QPointF & pos=QPointF(0,0), const QSize &size = QSize(50,50));
    void removeElement(Element *elem);

    static MainWindow *getInstance();

private:
    explicit MainWindow(QWidget *parent = 0);

private:
    static MainWindow *_mainWin;

    Ui::MainWindow *ui;

    QGraphicsScene *_scene;

    QSet<Element *> _elements;
    QSet<Element *> _links;
};

#endif // MAINWINDOW_H
