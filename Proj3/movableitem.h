#ifndef MOVABLEITEM_H
#define MOVABLEITEM_H
#include "item.h"

class MovableItem : public item
{

public:
    MovableItem(float x, float y, float w, float h, int type, int life, double density, double friction, double restitution, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    int status;
    int item_life;
    int count;
    double init_speedx,init_speedy;
    bool remove;
    int removedelay;


};

#endif // MOVABLEITEM_H
