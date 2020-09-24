#ifndef SCOREITEM_H
#define SCOREITEM_H
#include <QGraphicsTextItem>
#include <QGraphicsScene>>

class scoreItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    scoreItem(QGraphicsScene *scene);
    ~scoreItem();

    void refreshScore(QString &score);
};

#endif // SCOREITEM_H
