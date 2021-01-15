#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QEvent>
#include <QDebug>

class MyQGraphicsView : public QGraphicsView
{
public:
    MyQGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    MyQGraphicsView(QWidget *parent = nullptr);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MYQGRAPHICSVIEW_H
