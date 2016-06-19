#include "item.h"

item::item(b2World *world)
    :item_body(NULL),item_world(world)
{
    skill=true;
}
void item::setItemWorldSize(QSizeF worldsize,QSizeF windowsize)
{
    item_windowsize = windowsize;
    item_worldsize = worldsize;
}
QSizeF item::item_windowsize = QSizeF();
QSizeF item::item_worldsize = QSizeF();

void item::item_show()
{
    b2Vec2 pos = item_body->GetPosition();
    QPointF mappedPoint;
    mappedPoint.setX(((pos.x-(item_size.width())/2) * item_windowsize.width())/item_worldsize.width());
    mappedPoint.setY((1.0f - (pos.y+item_size.height()/2)/item_worldsize.height()) * item_windowsize.height());

    item_pixmap.setPos(mappedPoint);
    item_pixmap.resetTransform();
    item_pixmap.setRotation(-(item_body->GetAngle()*180/3.14159));
}

void item::item_show2()
{
    b2Vec2 pos = item_body->GetPosition();
    double angle;
    angle=-(item_body->GetAngle());
    QPointF mappedPoint;
    mappedPoint.setX(((pos.x+(-1*item_size.width()*cos(angle)+item_size.height()*sin(angle))/2) * item_windowsize.width())/item_worldsize.width());
    mappedPoint.setY((1.0f - (pos.y+(item_size.width()*sin(angle)+item_size.height()*cos(angle))/2)/item_worldsize.height()) * item_windowsize.height());
    item_pixmap.setPos(mappedPoint);
    item_pixmap.resetTransform();
    item_pixmap.setRotation(angle*180/3.14159);
}


void item::setSpeed(b2Vec2 v)
{
    item_body->SetLinearVelocity(v);
}
