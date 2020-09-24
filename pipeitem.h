#ifndef PIPEITEM_H
#define PIPEITEM_H
#include <QGraphicsObject>
#include <QGraphicsScene>


class PipeItem : public QGraphicsObject
{
    Q_OBJECT
public:
    PipeItem(QGraphicsScene* scene);
    ~PipeItem();

protected:
    //重写图形项的绘图区域
    QRectF boundingRect() const;

    //重写绘图事件
    void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

    QPainterPath shape() const;

public:
    static int getScore();  //获取分数
    static void clearScore();

private:
    void startMove();   //开始移动管道
    void createPipeHeight(); //创建管道高度
    void setScore();    //小鸟得分

private:
    QGraphicsScene* m_scene;
    int m_upPipeHeight; //上管道高度
    int m_downPipeHeight;   //下管道高度
    static int m_score;     //分数
};

#endif // PIPEITEM_H
