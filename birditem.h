#ifndef BIRDITEM_H
#define BIRDITEM_H
#include <QGraphicsObject>
#include <QGraphicsScene>
class QPropertyAnimation;


//游戏中的鸟儿对象（图形项），自定义图形项时，需重写paint,boundingRect两个虚函数
class BirdItem:public QGraphicsObject
{
    Q_OBJECT
public:
    BirdItem(QGraphicsScene *scene);
    ~BirdItem();

public:
    QRectF boundingRect() const;    //重写绘图区域

    void flyRaiseAnimation();   //上升动画
    void flyLandfallAnimation();    //下降动画
    bool checkIsCollided(); //碰撞检测

private slots:
    void onRefreshBird();   //定时刷新小鸟
    void onFlyRasieAnimationFinished(); //上升动画接收

private:
    //重写绘图事件
    void paint(QPainter* ,const QStyleOptionGraphicsItem*,QWidget*);
public:
    //重写键盘事件，空格键控制小鸟
    void keyPressEvent(QKeyEvent* event);

private:
    QTimer* m_birdRefreshTimer; //刷新小鸟的计时器
    QPropertyAnimation* m_flyAnimation; //飞行动画
    QGraphicsScene* m_scene;    //所在场景
    int m_curflyStatus; //当前飞行状态
    bool m_IsLandFall;  //下降标志
    bool m_IsRaise; //上升标志
    QList<QImage> m_imageList;
};

#endif // BIRDITEM_H
