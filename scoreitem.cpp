#include "scoreitem.h"
#include <QFont>

scoreItem::scoreItem(QGraphicsScene *scene)
{
    scene->addItem(this);
}

scoreItem::~scoreItem()
{

}

void scoreItem::refreshScore(QString &score)
{
    QFont scoreFont("宋体",20,60);
    scoreFont.setBold(isEnabled());
    this->setFont(scoreFont);
    this->setDefaultTextColor(QColor("#194819"));
    this->setPos(450,30);
    this->setZValue(6);
    this->setPlainText("Score:"+score);

    update();
}
