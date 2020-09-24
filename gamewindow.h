#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include "gamesecene.h"
#include "birditem.h"
#include <QParallelAnimationGroup>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "button.h"
#include "scoreitem.h"
#include <QList>
#include "pipeitem.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    //重写绘图事件
   void paintEvent(QPaintEvent *event) override;

   //重写键盘事件
   void keyPressEvent(QKeyEvent* event) override;

private:
    void initControl();
    void initBackgroundMusic();
    void loadStyleSheet(const QString& sheetName);
    void startWelcome();
    void GameOver();
    void GameOverMusic();

private slots:
    void onStartBtnClicked();
    void onCheckGmaeStatus();
    void onRestartBtnClicked();

public:
    static bool getGameStatus();

private:
    Ui::GameWindow *ui;
    QMediaPlayer* m_welcomePlayer; //欢迎的音乐
    QMediaPlaylist* m_welcomePlayerList;    //片头音乐播放列表
    QMediaPlayer* m_gameOverPlayer; //游戏结束音效
    GameScene* m_scene;  //游戏场景
    BirdItem* m_birdItem;   //鸟儿对象
    QParallelAnimationGroup* m_lettersGroupFading; //文本渐渐消失动画组
    QTimer* m_checkGameStatusTimer; //检测游戏状态
    QTimer* m_creatPipeTimer;  //生成管道间隔时间
    int m_pipeNum;  //生成管道的数量
    QList<PipeItem*> m_pipeItemList;
    Button* m_btnItem;
    scoreItem *m_score; //分数
    static bool m_startGame;   //游戏开始标志
};
#endif // GAMEWINDOW_H
