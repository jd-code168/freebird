 #ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsObject>


class Button:public QGraphicsObject
{
    Q_OBJECT
public:
    Button(const QPixmap& pixmap,QGraphicsScene *scene);
    ~Button();

protected:
    QRectF boundingRect() const; //重写图形项的绘图区域

private:
    //鼠标按下事件
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    //重写绘图
    void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

signals:
    void clicked(); //点击信号

private:
    QPixmap m_pix;
};

#endif // BUTTON_H
