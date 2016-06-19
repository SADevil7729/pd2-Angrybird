#include "bird.h"

Bird::Bird(float x, float y, float radius,int type, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):item(world)
{
    // Set pixmap

    item_pixmap.setPixmap(pixmap);
    item_pixmap.setTransformOriginPoint(item_pixmap.boundingRect().width()/2,item_pixmap.boundingRect().height()/2);
    item_size = QSize(radius*2,radius*2);
    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.bullet = true;
    bodydef.position.Set(x,y);
    bodydef.userData = this;


    item_type=type;
    item_body = world->CreateBody(&bodydef);
    b2CircleShape bodyshape;
    bodyshape.m_radius = (radius);
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;
    item_body->SetAngularDamping(3);
    item_body->CreateFixture(&fixturedef);

    //Bound timer
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(item_show()));

    scene->addItem(&item_pixmap);
}
