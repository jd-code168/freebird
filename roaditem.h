#ifndef ROADITEM_H
#define ROADITEM_H

#include <QGraphicsScene>
#include <QGraphicsObject>


class RoadItem : public QGraphicsObject
{
    Q_OBJECT
public:
    RoadItem(QGraphicsScene* scene);
    ~RoadItem();

protected:
    //重写图形项的绘图区域
    QRectF boundingRect() const;

    //重写图形项的绘图事件
    void paint(QPainter *,const QStyleOptionGraphicsItem *,QWidget *);

private:
    void startMove(); //开始移动

private:
    QGraphicsScene *m_scene;
};

#endif // ROADITEM_H
