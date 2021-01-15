#include "myqgraphicsview.h"

MyQGraphicsView::MyQGraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{

}

MyQGraphicsView::MyQGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void MyQGraphicsView::enterEvent(QEvent *event)
{
    QCursor cursorQGraphicView = QCursor(QPixmap(":/cursors/cursor/cursor.png"));
    viewport()->setCursor(cursorQGraphicView);
}

void MyQGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QCursor cursorQGraphicView = QCursor(QPixmap(":/cursors/cursor/cursor.png"));
    viewport()->setCursor(cursorQGraphicView);

    qDebug() << "Hello";
}

void MyQGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QCursor cursorQGraphicView = QCursor(QPixmap(":/cursors/cursor/cursor.png"));
    viewport()->setCursor(cursorQGraphicView);
}
