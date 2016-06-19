#ifndef BIRD_H
#define BIRD_H
#include <item.h>
#define BIRD_DENSITY 80.0f
#define BIRD_FRICTION 0.3f
#define BIRD_RESTITUTION 0.6f

class Bird : public item
{

public:
   Bird(float x, float y, float radius, int type, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);

};

#endif // BIRD_H
