#include "pipeitem.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <QDebug>
#include "gamewindow.h"

#define PIPE_WIDTH 60

int PipeItem::m_score=0;

PipeItem::PipeItem(QGraphicsScene* scene)
    :m_scene(scene)
{
    m_scene->addItem(this);
    createPipeHeight();
    startMove();
    setScore();
}

PipeItem::~PipeItem()
{

}

QRectF PipeItem::boundingRect() const
{
    return QRectF(m_scene->width(),0,PIPE_WIDTH,m_scene->height());
}

void PipeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawImage(QRectF(m_scene->width(),0,
                              PIPE_WIDTH,m_upPipeHeight)
                              ,QImage(":/BirdGame/Resources/image/tubeup.png"));
    painter->drawImage(QRectF(m_scene->width(),m_upPipeHeight+116,
                              PIPE_WIDTH,m_downPipeHeight)
                       ,QImage(":/BirdGame/Resources/image/tubedown.png"));
}

QPainterPath PipeItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(m_scene->width(),0,
                        PIPE_WIDTH,m_upPipeHeight));
    path.addRect(QRectF(m_scene->width(),m_upPipeHeight+140,
                        PIPE_WIDTH,m_downPipeHeight));

    return path;
}

int PipeItem::getScore()
{
    return m_score;
}

void PipeItem::clearScore()
{
    m_score=0;
}


void PipeItem::startMove()
{
    QPropertyAnimation* moveAnimation=new QPropertyAnimation(this,"pos");
    moveAnimation->setLoopCount(-1);
    moveAnimation->setDuration(3000);
    moveAnimation->setStartValue(QPoint(0,pos().y()));
    moveAnimation->setEndValue(QPoint(0-m_scene->width()-PIPE_WIDTH,pos().y()));
    moveAnimation->start();

    //动画循环次数改变时则创建管道
    connect(moveAnimation,&QPropertyAnimation::currentLoopChanged,[this](){
        if(GameWindow::getGameStatus()==true)
            m_score++;

        qDebug()<< m_score;
        createPipeHeight();
    });

}

//创建管道高度
void PipeItem::createPipeHeight()
{
   m_upPipeHeight= qrand()%100+80; //上管道的高度
   m_downPipeHeight=m_scene->height()-m_upPipeHeight-178;   //下管道的高度
}

void PipeItem::setScore()
{
//    QTimer* refreshScore=new QTimer(this);
//    connect(refreshScore,&QTimer::timeout,[this](){
//        if(this->pos().x()==0){
//            m_score++;
//            qDebug()<< m_score;
//        }
//    });
//    refreshScore->start(50);
}
