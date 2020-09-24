#include "birditem.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QPainter>
#include <QKeyEvent>
#include <QList>
#include "gamewindow.h"
#include <QDebug>

#define FLY_BIRD_SIZE 45

BirdItem::BirdItem(QGraphicsScene *scene)
    :m_curflyStatus(0)
    ,m_IsLandFall(true)
    ,m_IsRaise(true)
{
    scene->addItem(this);   //将当前的小鸟对象添加到游戏场景中去
    m_scene=scene;

    m_birdRefreshTimer=new QTimer(this);    //初始化计时器
    connect(m_birdRefreshTimer,SIGNAL(timeout()),
            this,SLOT(onRefreshBird()));
    m_birdRefreshTimer->setInterval(75);
    m_birdRefreshTimer->start();

    m_flyAnimation=new QPropertyAnimation(this,"pos");
    connect(m_flyAnimation,SIGNAL(finished()),
            this,SLOT(onFlyRasieAnimationFinished()));

    for (int i=0;i<3;i++){
        m_imageList.insert(i,QImage(QString(":/BirdGame/Resources/image/bird%1.png").arg((i+1))));
    }

}

BirdItem::~BirdItem()
{
    qDebug()<<"birdItem delete!";
}

//重写绘图区域
QRectF BirdItem::boundingRect() const
{
    return QRectF(60,FLY_BIRD_SIZE*5,FLY_BIRD_SIZE,FLY_BIRD_SIZE);
}

void BirdItem::flyRaiseAnimation()
{
    if(m_IsRaise){
        m_IsRaise=false;
        m_flyAnimation->stop();

        int nMove=pos().y();  //鸟儿移动的Y值

        if(nMove>-180){
            m_flyAnimation->setDuration(300); //动画时长
            m_flyAnimation->setEndValue(QPoint(pos().x(),pos().y()-FLY_BIRD_SIZE));
        }else{
             m_flyAnimation->setDuration(300); //动画时长
             m_flyAnimation->setEndValue(QPoint(pos().x(),-225));
        }
        m_flyAnimation->setEasingCurve(QEasingCurve::Linear);
        m_flyAnimation->start();
    }
}

void BirdItem::flyLandfallAnimation()
{
    if(m_IsLandFall){
        m_flyAnimation->stop();
        m_flyAnimation->setDuration(1000);
        m_flyAnimation->setEndValue(QPoint(pos().x(),m_scene->height()));
        m_flyAnimation->setEasingCurve(QEasingCurve::InQuad);
        m_flyAnimation->start();
        m_IsLandFall=false;
    }
}

bool BirdItem::checkIsCollided()
{
    //collidingItems返回与当前游戏项
    if(collidingItems().isEmpty())
        return false;
    else
        m_flyAnimation->stop();
        //this->setPos(60,225);
        //this->setProperty("pos",QPoint(60,-100));
        //collidingItems().removeAll(collidingItems().at(0));
//        m_flyAnimation->setEndValue(QPoint(60,0));
//        m_flyAnimation->setEasingCurve(QEasingCurve::InQuad);
//        m_flyAnimation->setDuration(1);
//        m_flyAnimation->start();
        return true;

        //
}

void BirdItem::onRefreshBird()
{
    update();   //刷新小鸟
}

void BirdItem::onFlyRasieAnimationFinished()
{
    m_IsLandFall=true;
    flyLandfallAnimation();
}

void BirdItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if(m_curflyStatus==1) { //翅膀往上
        m_curflyStatus++;
        painter->drawImage(QRectF(60,FLY_BIRD_SIZE*5,FLY_BIRD_SIZE,FLY_BIRD_SIZE),m_imageList.at(0));
    }else if(m_curflyStatus==2){
        m_curflyStatus++;
        painter->drawImage(QRectF(60,FLY_BIRD_SIZE*5,FLY_BIRD_SIZE,FLY_BIRD_SIZE),m_imageList.at(1));
    }else if(m_curflyStatus==3){
        m_curflyStatus++;
        painter->drawImage(QRectF(60,FLY_BIRD_SIZE*5,FLY_BIRD_SIZE,FLY_BIRD_SIZE),m_imageList.at(2));
    }else{
        m_curflyStatus=1;
         painter->drawImage(QRectF(60,FLY_BIRD_SIZE*5,FLY_BIRD_SIZE,FLY_BIRD_SIZE),m_imageList.at(0));
    }

}

void BirdItem::keyPressEvent(QKeyEvent *event)
{
    //空格键
    if(event->key()==Qt::Key_Space){
        m_IsRaise=true;
        flyRaiseAnimation();
    }
}
