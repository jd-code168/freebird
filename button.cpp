#include "button.h"
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Button::Button(const QPixmap& pixmap,QGraphicsScene *scene)
    :m_pix(pixmap)
{
    scene->addItem(this);
    this->setCursor(QCursor(Qt::PointingHandCursor)); //鼠标为手形状
}

Button::~Button()
{

}

QRectF Button::boundingRect() const
{
    return QRectF(QPointF(0,0),m_pix.size());
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //按下为左键则发射点击信号
    if(event->button()==Qt::LeftButton){
        emit clicked();
    }
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(boundingRect().toRect(),m_pix);
}
