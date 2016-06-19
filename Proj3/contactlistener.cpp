#include "contactlistener.h"
QList<MovableItem *> ContactListener::itemList=QList<MovableItem *>();
QList<MovableItem *> ContactListener::removeList=QList<MovableItem *>();
int ContactListener::game_score;
void ContactListener::BeginContact(b2Contact* co )
{
    double v_x,v_y;

    v_x=co->GetFixtureB()->GetBody()->GetMass()*co->GetFixtureB()->GetBody()->GetLinearVelocity().x-co->GetFixtureA()->GetBody()->GetMass()*co->GetFixtureA()->GetBody()->GetLinearVelocity().x;
    v_y=co->GetFixtureA()->GetBody()->GetMass()*co->GetFixtureA()->GetBody()->GetLinearVelocity().y-co->GetFixtureB()->GetBody()->GetMass()*co->GetFixtureB()->GetBody()->GetLinearVelocity().y;

    double damage=sqrt(v_x*v_x+v_y*v_y);

    MovableItem*  t1=(MovableItem*) (co->GetFixtureA()->GetBody()->GetUserData());
    MovableItem*  t2=(MovableItem*) (co->GetFixtureB()->GetBody()->GetUserData());

    if(t1->item_type==8||t2->item_type==8)
    {
        if(t1->item_type==8)
        {
            if(t1->count<=0)
            {
                sound = new QSound("./sound/006.wav");
                sound->play();
                t1->count+=11;
            }
            else
            {
                t1->count-=2;
                if(t1->item_type!=1&&t2->item_type!=1)
                {
                    t1->count-=3;
                }
            }
        }
        else
        {
            if(t2->count<=0)
            {
                sound = new QSound("./sound/006.wav");
                sound->play();
                t2->count+=11;
            }
            else
            {
                t2->count-=2;
                if(t1->item_type!=1&&t2->item_type!=1)
                {
                    t1->count-=3;
                }
            }
        }

    }
    else if((t1->item_type==3||t2->item_type==3)&&damage>300&&(rand()%100)>70)
    {
        sound = new QSound("./sound/035.wav");
        sound->play();
    }


    foreach(MovableItem *i,itemList)
    {
        if(co->GetFixtureA()->GetBody()->GetUserData()==i->item_body->GetUserData())
        {
            if(i->item_type!=8)
            {
                game_score+=damage*3;
            }
            if(i->item_type==6||i->item_type==9)
            {
                game_score+=damage*45;
            }
            i->item_life-=damage;
            //std::cout<<i->item_life<<std::endl;
        }
        if(co->GetFixtureB()->GetBody()->GetUserData()==i->item_body->GetUserData())
        {
            if(i->item_type!=8)
            {
                game_score+=damage*3;
            }
            if(i->item_type==6||i->item_type==9)
            {
                game_score+=damage*45;
            }
            i->item_life-=damage;
            //std::cout<<i->item_life<<std::endl;
        }
        if(i->item_life<0&&i->remove==false)
        {
            if(i->item_type!=8)
            {
                game_score+=damage*50;
            }
            if(i->item_type==6||i->item_type==9)
            {
                game_score+=800000;
            }
            removeList.push_back(i);
            i->remove=true;
            itemList.removeOne(i);
        }
    }
}

