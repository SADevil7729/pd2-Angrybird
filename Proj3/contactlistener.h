#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H
#include "Box2D/Box2D.h"
#include <iostream>
#include "movableitem.h"
#include <QSound>
class ContactListener :public b2ContactListener
{
public:
    static QList<MovableItem *> itemList;
    static QList<MovableItem *> removeList;
    QSound *sound;
    static int game_score;
private:

    void BeginContact(b2Contact *co);
    //void PostSolve(b2Contact* co, const b2ContactImpulse* impulse);
    //void EndContact(contact* contact);


};

#endif // CONTACTLISTENER_H
