#include "character.h"

character::character(int x, int y, QPixmap pixmap, QGraphicsScene *scene)
{
    pos_x=x;
    pos_y=y;
    item_pixmap.setPixmap(pixmap);
    mini_move3_count=0;
    boss_ball_count=0;

    status=0;
    character_show();
    scene->addItem(&item_pixmap);
}
void character::character_show()
{
    QPointF mappedPoint;
    mappedPoint.setX(pos_x);
    mappedPoint.setY(pos_y);
    item_pixmap.setPos(mappedPoint);
    item_pixmap.resetTransform();
}
void character::mini_move()
{
    pos_x+=4;
    character_show();
}
void character::mini_move2()
{
    pos_x-=4;
    character_show();
}
void character::mini_move3(double x,double y)
{
    pos_x+=x;
    pos_y+=y;
    character_show();
}
