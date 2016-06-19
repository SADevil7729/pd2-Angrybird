#ifndef CHARACTER_H
#define CHARACTER_H

#include "unmovableitem.h"
class character
{

public:
    character(int x, int y,  QPixmap pixmap, QGraphicsScene *scene);
    int status;
    int mini_move3_count;
    int boss_ball_count;
    int knight_tempx;
    QSound *sound;
    float pos_x,pos_y;

    QGraphicsPixmapItem item_pixmap;
public slots:
     void character_show();
     void mini_move();
     void mini_move2();
     void mini_move3(double x, double y);

private slots:



};

#endif // CHARACTER_H
