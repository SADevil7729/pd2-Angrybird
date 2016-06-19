#include "unmovableitem.h"

UnMovableItem::UnMovableItem(float x, float y, float w, float h,int type,QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):item(world)
{
    item_pixmap.setPixmap(pixmap);
    item_size = QSizeF(w,h);
    // Create body
    b2BodyDef bodyDef;
    bodyDef.userData = this;
    bodyDef.position.Set(x+w/2,y-h/2);
    item_body = world->CreateBody(&bodyDef);

    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w/2,h/2);
    item_body->CreateFixture(&bodyBox,9.0f);

    item_type=type;
    scene->addItem(&item_pixmap);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(item_show2()));
}
