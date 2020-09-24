#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QFile>
#include <QGraphicsView>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QTimer>
#include "roaditem.h"


bool GameWindow::m_startGame=false;

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
    /*,m_startGame(false)*/
    ,m_pipeNum(3)
{
    ui->setupUi(this);
    initControl();
    initBackgroundMusic();
    GameOverMusic();
    this->setWindowTitle("飞翔的小鸟");
    this->setWindowIcon(QIcon(":/BirdGame/Resources/image/bird1.png"));
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    //自定义paint事件
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_startGame) m_birdItem->keyPressEvent(event);
}

void GameWindow::initControl()
{
    loadStyleSheet("GameWindow");   //加载样式

    const QRectF gameRect=this->rect();
    m_scene=new GameScene(this,gameRect);   //初始化游戏场景
    QGraphicsView* view=new QGraphicsView(m_scene,this);
    //view->setScene(m_scene);
    view->setStyleSheet("border:none;background:transparent;");//设置视图无边框透明
    view->setCacheMode(QGraphicsView::CacheBackground); //设置缓存为背景模式
    startWelcome();
}

void GameWindow::initBackgroundMusic()
{
    m_welcomePlayer=new QMediaPlayer(this);
    m_welcomePlayerList=new QMediaPlaylist(this);

    m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
    //..添加曲目
    //..添加曲目

    //循环播放当前列表中的曲目
    m_welcomePlayerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_welcomePlayer->setPlaylist(m_welcomePlayerList); //设置播放列表
    m_welcomePlayer->play();
}

void GameWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/BirdGame/Resources/qss/"+sheetName+".css");
    if(file.open(QFile::ReadOnly)){
        QString strStyleSheet=this->styleSheet();
        strStyleSheet+=file.readAll();
        this->setStyleSheet(strStyleSheet);
        file.close();
    }
}

//启动欢迎
void GameWindow::startWelcome()
{
    //道路
    RoadItem *roadItem=new RoadItem(m_scene);
    //管道

   m_creatPipeTimer=new QTimer(this);

    static int pipeNum=0;
    //m_pipeItemList=new QList<PipeItem *>;
    connect(m_creatPipeTimer,&QTimer::timeout,[this](){
            PipeItem* pipeItem=new PipeItem(m_scene);
            m_pipeItemList.append(pipeItem);
            pipeNum++;
            if (!(m_pipeNum-pipeNum)){
                m_creatPipeTimer->stop();
                pipeNum=0;
            }

    });

    m_creatPipeTimer->start(1000);

    //小鸟
    m_birdItem=new BirdItem(m_scene);
    m_birdItem->setZValue(6);

    //游戏状态检测
    m_checkGameStatusTimer=new QTimer(this);
    connect(m_checkGameStatusTimer,SIGNAL(timeout()),
            this,SLOT(onCheckGmaeStatus()));

    //字母
    const int nletters=15;
    struct{
        char const *pix;
        qreal initX,initY;
        qreal destX,destY;
    }letterData[nletters]={
        {"飞",-1000,-1000,250,100},
        {"吧",-800,-1000,300,100},
        {"，",-600,-1000,350,100},
        {"像",-400,-1000,150,160},
        {"风",1000,2000,200,160},
        {"一",800,2000,250,160},
        {"样",600,2000,300,160},
        {"自",400,2000,350,160},
        {"由",200,2000,400,160},
        {"，",0,2000,450,160},
        {"无",0,2000,175,220},
        {"法",0,2000,225,220},
        {"挽",0,2000,275,220},
        {"留",0,2000,325,220},
        {"...",0,2000,375,220},
    };

    //连续动画组
    QSequentialAnimationGroup* letterGroupMoving=new QSequentialAnimationGroup(this);

    m_lettersGroupFading=new QParallelAnimationGroup(this);

    for(int i=0;i<nletters;i++){
        //文本图形项
        QString htmlText=QString("<span style=\"font-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819;\">%1</span>").arg(letterData[i].pix);
        QGraphicsTextItem *letter=new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(QPoint(letterData[i].initX,letterData[i].initY));
        letter->setZValue(6);
        //文本项属性动画
        QPropertyAnimation *moveAnimation=new QPropertyAnimation(letter,"pos",letterGroupMoving);
        moveAnimation->setEndValue(QPoint(letterData[i].destX,letterData[i].destY));
        moveAnimation->setDuration(200);
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);
        letterGroupMoving->addPause(50);

        //渐渐消失动画
        QPropertyAnimation *fadeAnimation=new QPropertyAnimation(letter,"opacity",m_lettersGroupFading);
        fadeAnimation->setDuration(1000);
        fadeAnimation->setEndValue(0);
        fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

        m_scene->addItem(letter);   //往游戏场景中添加字符图形选项
    }

    letterGroupMoving->start(QAbstractAnimation::DeleteWhenStopped); //动画停止后删除动画

    //按钮图形项
    QPixmap pix=QPixmap(":/BirdGame/Resources/image/startButton.png");
    m_btnItem=new Button(pix,m_scene);
    m_btnItem->setPos(QPointF((width()-pix.width())/2,300));
    m_btnItem->setZValue(6);

    //按钮渐渐消失动画
    QPropertyAnimation *fadeAnimation=new QPropertyAnimation(m_btnItem,"opacity",m_lettersGroupFading);
    fadeAnimation->setDuration(600); //动画时长
    fadeAnimation->setEndValue(0);  //opacity为0全透明
    fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

    //显示分数
    m_score=new scoreItem(m_scene);
    QString score="0";
    m_score->refreshScore(score);

    //m_scene->addItem(m_score);

    connect(m_btnItem,SIGNAL(clicked()),this,SLOT(onStartBtnClicked()));
    connect(fadeAnimation,&QPropertyAnimation::finished,
            [this](){
       m_startGame=true;
       m_checkGameStatusTimer->start(50);
       //m_birdItem->setPos(0,-180);
       m_birdItem->flyLandfallAnimation();
    });
}

void GameWindow::GameOver()
{
    //“墙都不扶，就服你！”
    const int nletters=9;

    struct{
        char const *pix;
        qreal initX,initY;
        qreal destX,destY;
    }letterData[nletters]={
        {"墙",-1000,-1000,200,100},
        {"都",-800,-1000,250,100},
        {"不",-600,-1000,300,100},
        {"服",-400,-1000,350,100},
        {"！",1000,2000,400,100},
        {"就",800,2000,225,160},
        {"服",600,2000,275,160},
        {"你",400,2000,325,160},
        {"！",200,2000,375,160}
    };

    //game over 文件动画组
    QParallelAnimationGroup* lettersGroupMoving=new QParallelAnimationGroup(this);
    m_lettersGroupFading=new QParallelAnimationGroup(this);

    for(int i=0;i<nletters;i++){
        QString htmlText=QString("<span style=\"font-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819;\">%1</span>").arg(letterData[i].pix);
        QGraphicsTextItem *letter=new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(QPoint(letterData[i].initX,letterData[i].initY));
        //文本项属性动画
        QPropertyAnimation *moveAnimation=new QPropertyAnimation(letter,"pos",lettersGroupMoving);
        moveAnimation->setEndValue(QPoint(letterData[i].destX,letterData[i].destY));
        moveAnimation->setDuration(600);
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);

        QPropertyAnimation *fadeAnimation=new QPropertyAnimation(letter,"opacity",m_lettersGroupFading);
        fadeAnimation->setDuration(300);
        fadeAnimation->setEndValue(0);
        fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);


        //文本图形项添加到游戏场景
        m_scene->addItem(letter);

    }

    lettersGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);

    //将小鸟图形项移除
    m_scene->removeItem(m_birdItem);
    m_birdItem->deleteLater();
    //游戏结束的音效
    m_welcomePlayer->stop();
    m_gameOverPlayer->play();

    QPixmap pix=QPixmap(":/BirdGame/Resources/image/startButton.png");
    m_btnItem=new Button(pix,m_scene);
    m_btnItem->setPos(QPointF(250,220));
    connect(m_btnItem,SIGNAL(clicked()),this,SLOT(onRestartBtnClicked()));
    m_checkGameStatusTimer->stop();
    m_startGame=false;

    connect(lettersGroupMoving,&QParallelAnimationGroup::destroyed,[this](){
        m_birdItem=new BirdItem(m_scene);
        m_scene->addItem(m_birdItem);

    });

    int pipeListSize=m_pipeItemList.size();

    for(int index=0;index<pipeListSize;index++){
        m_scene->removeItem(m_pipeItemList.at(index));
        connect(m_pipeItemList.at(index),&PipeItem::destroyed,[this](){
            m_pipeItemList.removeFirst();
        });
        m_pipeItemList.at(index)->deleteLater();

    }
    //m_pipeItemList.clear();


}

void GameWindow::GameOverMusic()
{
    m_gameOverPlayer=new QMediaPlayer(this);
    m_gameOverPlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/gameover.wav"));
    m_gameOverPlayer->setVolume(100);
}


void GameWindow::onStartBtnClicked()
{
    //启动渐渐消失动画，动画结束后删除动画
    m_lettersGroupFading->start(QAbstractAnimation::DeleteWhenStopped);
    m_scene->removeItem(m_btnItem);
}

void GameWindow::onCheckGmaeStatus()
{
    if(m_birdItem->checkIsCollided()){ //鸟儿是否与其他的场景有冲突
        GameOver();
    }else{
        //m_score->setPlainText(QString("Score:%1").arg(PipeItem::getScore()));
        QString score=QString::number(PipeItem::getScore()); //分数累计
        m_score->refreshScore(score);
    }
}

void GameWindow::onRestartBtnClicked()
{
    m_btnItem->setOpacity(0);
    m_lettersGroupFading->start(QAbstractAnimation::DeleteWhenStopped);
    connect(m_lettersGroupFading,&QAbstractAnimation::destroyed,[this]{
        m_startGame=true;
        m_gameOverPlayer->stop();
        m_welcomePlayer->play();
//        m_birdItem=new BirdItem(m_scene);
//        m_scene->addItem(m_birdItem);
        m_birdItem->flyLandfallAnimation();
        m_checkGameStatusTimer->start(50);
        m_score->setPlainText(QString("Score:0"));
        PipeItem::clearScore();
        m_creatPipeTimer->start(1000);
    });

}

bool GameWindow::getGameStatus()
{
    return m_startGame;
}

