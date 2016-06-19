#include "movableitem.h"
MovableItem::MovableItem(float x, float y, float w, float h,int type, int life,double density,double friction,double restitution, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):item(world)
{
    item_pixmap.setPixmap(pixmap);
    item_size = QSizeF(w,h);
    // Create body
    status=0;
    count=0;
    item_life=life;
    removedelay=20;
    remove=false;
    b2BodyDef bodyDef;
    bodyDef.type=b2_dynamicBody;
    bodyDef.userData = this;
    bodyDef.position.Set(x+w/2,y-h/2);
    item_body = world->CreateBody(&bodyDef);
    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w/2,h/2);
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyBox;
    fixturedef.density = density;
    fixturedef.friction = friction;
    fixturedef.restitution = restitution;
    item_body->SetAngularDamping(3);
    item_body->CreateFixture(&fixturedef);

    item_type=type;
    scene->addItem(&item_pixmap);
    item_scene=scene;
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(item_show2()));
}


