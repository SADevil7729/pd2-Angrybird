#ifndef UNMOVABLEITEM_H
#define UNMOVABLEITEM_H
#include <item.h>
#include <iostream>
class UnMovableItem : public item
{
public:
    UnMovableItem(float x, float y, float w, float h, int type, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);

};

#endif // UNMOVABLEITEM_H
