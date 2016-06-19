#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QGraphicsSceneMouseEvent>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QtGui>
#include <Box2D/Box2D.h>
#include <item.h>
#include <bird.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <unmovableitem.h>
#include <contactlistener.h>
#include <movableitem.h>
#include <character.h>
#include <QKeyEvent>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int screenW;
    int screenH;
    double constant;
    double constant_convert;
    int condition;
    int mini_ready;
    double shoot_x,shoot_y;
    int remove_delay;
    int mouse_tempx,mouse_tempy;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QTimer timer,timer2,timer3,timer4,timer5,timer6,timer7,timer8;
    ContactListener *CL;

    QGraphicsPixmapItem *back,*exit,*restart;
    QGraphicsPixmapItem *score1,*score2,*score3,*score4,*score5,*score6,*score7,*score8;
    double mini_move3_x,mini_move3_y;
    QSound *sound;

    b2World *world;
    Bird *birdie;
    UnMovableItem *UMI;
    character *knight,*bandit,*boss,*boss_ball,*magic;
    QList<character *> BallList;
    MovableItem *MI;
    MovableItem *pig1,*pig2;
    MovableItem *ghost,*ghost_ball;
    QList<MovableItem *> ghostBallList;
    QMediaPlayer *bgm;
    bool bir1,bir2,bir3;
    int volume;
    bool pig1_is_died,pig2_is_died;
    void init();

    int game_score_temp;
    bool eventFilter(QObject *,QEvent *event);

    //void keyPressEvent(QKeyEvent *e);
signals:
    void quitGame();
public slots:
    void result_jump();
private slots:
    void tick();
    void QUITSLOT();
    void knight_move();
    void mini_move();
    void bgm_down();
    void bomb();
    void boss_move();
    void ghost_move();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
