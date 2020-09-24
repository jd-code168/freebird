#include "roaditem.h"
#include <QPainter>
#include <QPropertyAnimation>

#define ROAD_ITEM_HEIGHT 64

RoadItem::RoadItem(QGraphicsScene* scene)
    :m_scene(scene)
{

    m_scene->addItem(this);

    startMove();
}

RoadItem::~RoadItem()
{

}

QRectF RoadItem::boundingRect() const
{
    return QRectF(0,m_scene->height()-ROAD_ITEM_HEIGHT,
                  m_scene->width()*2,ROAD_ITEM_HEIGHT);
}

void RoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawImage(QRectF(0,m_scene->height()-ROAD_ITEM_HEIGHT,
                              m_scene->width(),ROAD_ITEM_HEIGHT),QImage(":/BirdGame/Resources/image/road.png"));
    painter->drawImage(QRectF(m_scene->width(),m_scene->height()-ROAD_ITEM_HEIGHT,
                              m_scene->width(),ROAD_ITEM_HEIGHT),QImage(":/BirdGame/Resources/image/road.png"));
}


void RoadItem::startMove()
{
    QPropertyAnimation *moveAnimation=new QPropertyAnimation(this,"pos");
    moveAnimation->setLoopCount(-1);
    moveAnimation->setDuration(6000);
    moveAnimation->setStartValue(QPoint(0,pos().y()));
    moveAnimation->setEndValue(QPoint(0-m_scene->width(),pos().y()));
    moveAnimation->setEasingCurve(QEasingCurve::Linear);
    moveAnimation->start();
}
