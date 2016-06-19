#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //梓喵大好♪
    srand(time(NULL));
    ui->setupUi(this);
    qApp->installEventFilter(this);
    this->setWindowIcon(QIcon(":/1/image/icon.jpg"));
    this->setIconSize(QSize(500,568));
    screenW = 1280;
    screenH = 640;
    constant=18;
    constant_convert=double(screenH/constant);

    this->setGeometry(8,30, screenW , screenH);
    this->setWindowTitle("Azusa");
    this->setWindowIcon(QIcon(":/1/image/icon.jpg"));
    this->setIconSize(QSize(1024,768));
    this->setFixedHeight(screenH);
    this->setFixedWidth(screenW);

    view=new QGraphicsView(this);
    scene=new QGraphicsScene (0,0,screenW*1.3-5,screenH*1-20);

    init();
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    connect(&timer2,SIGNAL(timeout()),this,SLOT(knight_move()));
    connect(&timer3,SIGNAL(timeout()),this,SLOT(mini_move()));
    connect(&timer4,SIGNAL(timeout()),this,SLOT(bgm_down()));
    connect(&timer5,SIGNAL(timeout()),this,SLOT(bomb()));
    connect(&timer6,SIGNAL(timeout()),this,SLOT(boss_move()));
    connect(&timer7,SIGNAL(timeout()),this,SLOT(ghost_move()));
    connect(&timer8,SIGNAL(timeout()),this,SLOT(result_jump()));
    timer.start(1000/60);
    timer8.start(10);
}
MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        QGraphicsSceneMouseEvent* ME= (QGraphicsSceneMouseEvent *) event;
        if(condition==0&&mini_ready==1)
        {
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit14.png").scaled(2*constant_convert,2*constant_convert));
            timer3.start(700);
            mini_ready=0;
        }
        else if(condition==2)
        {
            condition=3;
            timer2.start(90);
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit5.png").scaled(2*constant_convert,2*constant_convert));
            if(sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400))<100)
            {
                birdie->item_body->SetTransform(b2Vec2(ME->scenePos().x()/constant_convert,(640-ME->scenePos().y())/constant_convert),birdie->item_body->GetAngle());
                mouse_tempx=ME->scenePos().x();
                mouse_tempy=ME->scenePos().y();
            }
            else
            {
                double x,y;
                x=300+100*(ME->scenePos().x()-300)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                y=400+100*(ME->scenePos().y()-400)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));

                birdie->item_body->SetTransform(b2Vec2(x/constant_convert,(640-y)/constant_convert),birdie->item_body->GetAngle());
                mouse_tempx=x;
                mouse_tempy=y;
            }
        }
        else if(condition==5)
        {
            if(birdie->skill==true&&birdie->item_type==4)
            {
                sound = new QSound(":/1/sound/031.wav");
                sound->play();
                double skill_x,skill_y;
                skill_x=ME->scenePos().x() - birdie->item_body->GetPosition().x*constant_convert;
                skill_y=((640-ME->scenePos().y())-birdie->item_body->GetPosition().y*constant_convert);
                birdie->item_body->SetLinearVelocity(b2Vec2(skill_x/20,skill_y/20));
                birdie->skill=false;
            }
            if(birdie->skill==true&&birdie->item_type==5)
            {
                sound = new QSound(":/1/sound/030.wav");
                sound->play();
                birdie->item_body->SetAngularVelocity(-1000);
                birdie->item_body->SetAngularDamping(0.002);
                birdie->skill=false;
            }

        }
        else if(condition==7)
        {
            condition=8;
            if(sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400))<100)
            {
                ghost->item_body->SetTransform(b2Vec2(ME->scenePos().x()/constant_convert,(640-ME->scenePos().y())/constant_convert),ghost->item_body->GetAngle());
                mouse_tempx=ME->scenePos().x();
                mouse_tempy=ME->scenePos().y();
            }
            else
            {
                double x,y;
                x=300+100*(ME->scenePos().x()-300)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                y=400+100*(ME->scenePos().y()-400)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                ghost->item_body->SetTransform(b2Vec2(x/constant_convert,(640-y)/constant_convert),ghost->item_body->GetAngle());
                mouse_tempx=x;
                mouse_tempy=y;
            }
        }
        else if(condition==10||condition==11)
        {
            if(condition==11&&ME->scenePos().x()>=400&&ME->scenePos().x()<=600&&ME->scenePos().y()>=200&&ME->scenePos().y()<=400)
            {
                bgm->stop();
                init();
            }
            else if(condition==11&&ME->scenePos().x()>=700&&ME->scenePos().x()<=900&&ME->scenePos().y()>=200&&ME->scenePos().y()<=400)
            {
                emit quitGame();
                qApp->quit();
            }
            else if(ghost->skill==true)
            {
                ghost->item_body->SetActive(false);
                double x,y;
                x=(ME->scenePos().x()-1.5*constant_convert-ghost->item_body->GetPosition().x*constant_convert);
                y=((640-ME->scenePos().y())-ghost->item_body->GetPosition().y*constant_convert);
                shoot_x=x*15/sqrt(x*x+y*y);
                shoot_y=y*15/sqrt(x*x+y*y);
                timer7.start(150);
                ghost->skill=false;
            }

        }
        //std::cout<<ME->scenePos().x()<<" "<<ME->scenePos().y()<<std::endl;
    }

    else if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        QGraphicsSceneMouseEvent* ME= (QGraphicsSceneMouseEvent *) event;
        if(condition==3)
        {
            if(sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400))<100)
            {
                birdie->item_body->SetTransform(b2Vec2(ME->scenePos().x()/constant_convert,(640-ME->scenePos().y())/constant_convert),birdie->item_body->GetAngle());
                mouse_tempx=ME->scenePos().x();
                mouse_tempy=ME->scenePos().y();
            }
            else
            {
                double x,y;
                x=300+100*(ME->scenePos().x()-300)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                y=400+100*(ME->scenePos().y()-400)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                birdie->item_body->SetTransform(b2Vec2(x/constant_convert,(640-y)/constant_convert),birdie->item_body->GetAngle());
                mouse_tempx=x;
                mouse_tempy=y;
            }
        }
        else if(condition==8)
        {
            if(sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400))<100)
            {
                ghost->item_body->SetTransform(b2Vec2(ME->scenePos().x()/constant_convert,(640-ME->scenePos().y())/constant_convert),ghost->item_body->GetAngle());
                mouse_tempx=ME->scenePos().x();
                mouse_tempy=ME->scenePos().y();
            }
            else
            {
                double x,y;
                x=300+100*(ME->scenePos().x()-300)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                y=400+100*(ME->scenePos().y()-400)/sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400));
                ghost->item_body->SetTransform(b2Vec2(x/constant_convert,(640-y)/constant_convert),ghost->item_body->GetAngle());
                mouse_tempx=x;
                mouse_tempy=y;
            }
        }
    }
    else if(event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        QGraphicsSceneMouseEvent* ME= (QGraphicsSceneMouseEvent *) event;
        if(condition==3)
        {
            if(sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400))<100)
            {
                shoot_x=-(ME->scenePos().x()-300)/3;
                shoot_y=(ME->scenePos().y()-400)/3;
            }
            else
            {
                shoot_x=-(mouse_tempx-300)/3;
                shoot_y=(mouse_tempy-400)/3;
            }
            condition=4;
            timer3.start(80);
        }
        else if(condition==8)
        {
            condition=9;
            if(sqrt((ME->scenePos().x()-300)*(ME->scenePos().x()-300)+(ME->scenePos().y()-400)*(ME->scenePos().y()-400))<100)
            {
                shoot_x=-(ME->scenePos().x()-300)/3;
                shoot_y=(ME->scenePos().y()-400)/3;
            }
            else
            {
                shoot_x=-(mouse_tempx-300)/3;
                shoot_y=(mouse_tempy-400)/3;
            }
            bandit->status=7;
            timer3.start(80);
        }
    }
    return false;
}

void MainWindow::tick()
{
    world->Step(1.0/60.0,6,2);

    if(condition==5)
    {
    int i;
    bool find=false;
    view->centerOn(birdie->item_body->GetPosition().x*constant_convert,0);
    b2Body *list=world->GetBodyList();
    for(i=0;i<=world->GetBodyCount();i++)
    {
        if(abs(list->GetLinearVelocity().x)>=0.1||abs(list->GetLinearVelocity().y)>=0.1||abs(list->GetAngularVelocity())>=0.05)
        {
            find=true;
            break;
        }
        if(list->GetNext()!=NULL)
        {
            list=list->GetNext();
        }
    }
    if(find==false)
    {
        remove_delay++;
        if(remove_delay>30)
        {
            birdie->item_world->DestroyBody(birdie->item_body);
            delete birdie;
            view->centerOn(0,0);
            condition=0;
            remove_delay=0;
            if(pig1_is_died==true)
            {
                condition=6;
                timer4.start(180);
                sound = new QSound("./sound/end.wav");
                sound->play();
                }
            }
        }
        else
        {
            remove_delay=0;
        }
    }
    foreach (MovableItem* i,ContactListener::removeList )
    {
        if(ContactListener::removeList.empty()==false)
        {
            if(i->removedelay>0)
            {
                i->removedelay--;
            }
            else
            {
                ContactListener::removeList.removeOne(i);
                if(i->item_type==6)
                {
                    pig1_is_died=true;
                }
                else if(i->item_type==9)
                {
                    pig2_is_died=true;
                    timer4.start(180);
                }
                else if(i->item_type==8)
                {
                    ghostBallList.removeOne(i);
                }
                i->item_body->SetActive(false);
                i->item_world->DestroyBody(i->item_body);
                delete i;
            }
        }
    }
    foreach(MovableItem* i,ghostBallList)
    {
        i->item_body->SetLinearVelocity(b2Vec2(i->init_speedx,i->init_speedy));
    }
    scene->update();
}
void MainWindow::knight_move()
{
    if(knight->status==0)
    {
        knight->status=1;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight3.png").scaled(3*constant_convert,4*constant_convert));
    }
    else if(knight->status==1)
    {
        knight->status=2;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight4.png").scaled(3*constant_convert,4*constant_convert));
    }
    else if(knight->status==2)
    {
        knight->status=3;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight5.png").scaled(3*constant_convert,4*constant_convert));
        timer2.stop();
    }
    else if(knight->status==3)
    {
        knight->status=4;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight6.png").scaled(3*constant_convert,4*constant_convert));
    }
    else if(knight->status==4)
    {
        knight->status=5;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight7.png").scaled(3*constant_convert,4*constant_convert));
    }
    else if(knight->status==5)
    {
        knight->status=6;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight8.png").scaled(3*constant_convert,4*constant_convert));
        timer2.start(700);
    }
    else if(knight->status==6)
    {
        knight->status=1;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight1.png").scaled(3*constant_convert,4*constant_convert));
        timer2.stop();
    }
}
void MainWindow::mini_move()
{
    if(bandit->status<=6&&bandit->pos_x<200)
    {
        if(condition==1)
        {
            bandit->mini_move();
            birdie->item_body->SetTransform(b2Vec2((bandit->pos_x+30)/constant_convert,(640-bandit->pos_y+20)/constant_convert),birdie->item_body->GetAngle());
        }

        if(bandit->status==0)
        {
            bandit->status=1;
            timer3.start(80);
        }
        else if(bandit->status==1)
        {
            sound = new QSound(":/1/sound/069.wav");
            sound->play();
            condition=1;
            timer3.start(400);
            int temp;
            bool find;
            find=false;
            if(bir1==false||bir2==false||bir3==false)
            {
                while(find==false)
                {
                    temp=rand()%3;
                    if(temp==0&&bir1==false)
                    {
                        find=true;
                    }
                    else if(temp==1&&bir2==false)
                    {
                        find=true;
                    }
                    else if(temp==2&&bir3==false)
                    {
                        find=true;
                    }
                }
            }
            else
            {
                temp =rand()%3;
            }
            if(temp==0)
            {
                birdie = new Bird(-100,-100,0.53f,2,&timer,QPixmap(":/1/image/bird1.png").scaled(50,50),world,scene);
                bir1=true;
            }
            else if(temp==1)
            {
                birdie = new Bird(-100,-100,0.53f,4,&timer,QPixmap(":/1/image/bird2.png").scaled(50,50),world,scene);
                bir2=true;
            }
            else if(temp==2)
            {
                birdie = new Bird(-100,-100,0.53f,5,&timer,QPixmap(":/1/image/bird3.png").scaled(50,50),world,scene);
                bir3=true;
            }

            birdie->item_body->SetTransform(b2Vec2((bandit->pos_x+30)/constant_convert,(640-bandit->pos_y+20)/constant_convert),birdie->item_body->GetAngle());
            birdie->item_body->SetGravityScale(0);
            bandit->status=2;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit2.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==2)
        {
            bandit->status=3;
            timer3.start(80);
        }
        else if(bandit->status==3)
        {
            sound = new QSound(":/1/sound/003.wav");
            sound->play();
            bandit->status=4;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit2.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==4)
        {

            bandit->status=5;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit3.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==5)
        {
            sound = new QSound(":/1/sound/004.wav");
            sound->play();
            bandit->status=6;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit4.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==6)
        {
            bandit->status=3;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit3.png").scaled(2*constant_convert,2*constant_convert));
        }
        return ;
    }
    else if(bandit->status<=6&&bandit->pos_x>=200)
    {
        sound = new QSound(":/1/sound/009.wav");
        sound->play();
        bandit->status=7;
        condition=2;
        timer3.stop();
        return ;
    }

    if(bandit->status>=7)
    {
        if(bandit->status==7)
        {
            sound = new QSound(":/1/sound/017.wav");
            sound->play();
            bandit->status=8;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit6.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==8)
        {
            bandit->status=9;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit7.png").scaled(2*constant_convert,2*constant_convert));
            if(condition==9)
            {
                mini_move3_x=ghost->item_body->GetPosition().x*constant_convert-bandit->pos_x-80;
                mini_move3_y=(640-ghost->item_body->GetPosition().y*constant_convert)-bandit->pos_y-35;
            }
            else
            {
                mini_move3_x=birdie->item_body->GetPosition().x*constant_convert-bandit->pos_x-70;
                mini_move3_y=(640-birdie->item_body->GetPosition().y*constant_convert)-bandit->pos_y-30;
            }
        }
        else if(bandit->status==9)
        {
            if(bandit->mini_move3_count<10)
            {
                bandit->mini_move3(mini_move3_x/10,mini_move3_y/10);
                bandit->mini_move3_count++;
            }
            else
            {
                if(condition!=9)
                {
                    timer2.start(100);
                }
                bandit->status=10;
                bandit->mini_move3_count=0;
            }
        }
        else if(bandit->status==10)
        {
            sound = new QSound(":/1/sound/045.wav");
            sound->play();
            bandit->status=11;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit8.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==11)
        {
            bandit->status=12;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit9.png").scaled(2*constant_convert,2*constant_convert));
            timer3.start(800);
        }
        else if(bandit->status==12)
        {
            sound = new QSound(":/1/sound/006.wav");
            sound->play();
            bandit->status=13;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit10.png").scaled(2*constant_convert,2*constant_convert));
            if(condition==9)
            {
                ghost->item_body->SetLinearVelocity(b2Vec2(shoot_x/5,shoot_y/5));
                ghost->item_body->SetGravityScale(1);
                condition=10;
            }
            else
            {
                birdie->item_body->SetLinearVelocity(b2Vec2(shoot_x,shoot_y));
                birdie->item_body->SetGravityScale(1);
                condition=5;
            }

        }
        else if(bandit->status==13)
        {
            bandit->status=14;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit7.png").scaled(2*constant_convert,2*constant_convert));
            timer3.start(80);
            mini_move3_x=-40;
            mini_move3_y=505-bandit->pos_y;
        }
        else if(bandit->status==14)
        {
            if(bandit->mini_move3_count<10)
            {
                bandit->mini_move3_count++;
                bandit->mini_move3(mini_move3_x/10,mini_move3_y/10);
            }
            else
            {
                sound = new QSound(":/1/sound/009.wav");
                sound->play();
                bandit->status=15;
                bandit->mini_move3_count=0;
            }
        }
        else if(bandit->status==15)
        {
            bandit->status=16;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit1.png").scaled(2*constant_convert,2*constant_convert));
        }


    }

    if(bandit->status>=16&&bandit->status<=19&&bandit->pos_x>50)
    {
        bandit->mini_move2();
        if(bandit->status==16)
        {
            sound = new QSound(":/1/sound/003.wav");
            sound->play();
            bandit->status=17;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit11.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==17)
        {
            bandit->status=18;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit12.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==18)
        {
            sound = new QSound(":/1/sound/004.wav");
            sound->play();
            bandit->status=19;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit13.png").scaled(2*constant_convert,2*constant_convert));
        }
        else if(bandit->status==19)
        {
            bandit->status=16;
            bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit12.png").scaled(2*constant_convert,2*constant_convert));
        }
        return ;
    }
    else if(bandit->status>=16&&bandit->status<=19)
    {
        sound = new QSound(":/1/sound/009.wav");
        sound->play();
        bandit->status=0;
        bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit1.png").scaled(2*constant_convert,2*constant_convert));
        timer3.stop();
        mini_ready=1;
        return ;
    }
}
void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
}
void MainWindow::bgm_down()
{
    bgm->setVolume(volume);
    volume=volume-1;
    if(volume<=50)
    {
        volume-=4;

    }
    if(volume<=0)
    {
        timer4.stop();
        if(condition==6)
        {
        bgm->setMedia(QUrl::fromLocalFile("./sound/bgm2.wav"));
        timer5.start(2500);
        volume=80;
        }
        else if(condition==10)
        {
            condition=11;
            restart=new QGraphicsPixmapItem();
            restart->setPixmap(QPixmap(":/1/image/restart.png").scaled(200,200));
            restart->setPos(400,200);
            scene->addItem(restart);

            exit=new QGraphicsPixmapItem();
            exit->setPixmap(QPixmap(":/1/image/exit.png").scaled(200,200));
            exit->setPos(700,200);
            scene->addItem(exit);

            bgm->setMedia(QUrl::fromLocalFile("./sound/pass.wav"));
            volume=100;
        }
        bgm->play();
        bgm->setVolume(volume);
    }
}
void MainWindow::bomb()
{

    sound = new QSound(":/1/sound/020.wav");
    sound->play();
    foreach(MovableItem *i,ContactListener::itemList)
    {
        if(i->item_life>1000)
        {
            i->item_life=1000;
        }
        i->item_body->SetLinearVelocity(b2Vec2(-25-rand()%10,rand()%15+1));
    }
    timer5.stop();
    timer6.start(1800);

}
void MainWindow::boss_move()
{
    if(boss->status==0)
    {
        boss->status++;
        sound = new QSound(":/1/sound/061.wav");
        sound->play();
        timer6.start(240);
        boss->item_pixmap.setX(1000);
        boss->item_pixmap.setY(472);
        boss->pos_y=472;
    }
    else if(boss->status==1)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show2.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==2)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show3.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==3)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show4.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==4)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show5.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==5)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss3.png").scaled(2*constant_convert,3*constant_convert));
        timer6.start(1500);
    }
    else if(boss->status==6)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss5.png").scaled(3*constant_convert,3*constant_convert));
        timer6.start(100);
    }
    else if(boss->status==7)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss6.png").scaled(3*constant_convert,3*constant_convert));
    }
    else if(boss->status==8)
    {
        foreach(MovableItem *i,ContactListener::itemList)
        {
            if(i->remove==false)
            {
                ContactListener::removeList.push_back(i);
                i->remove=true;
                ContactListener::itemList.removeOne(i);
            }
        }
        sound = new QSound(":/1/sound/100.wav");
        sound->play();
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss7.png").scaled(3*constant_convert,3*constant_convert));
    }
    else if(boss->status==9)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss8.png").scaled(3*constant_convert,3*constant_convert));
    }
    else if(boss->status==10)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss9.png").scaled(3*constant_convert,3*constant_convert));
        knight->knight_tempx=knight->pos_x;
    }
    else if(boss->status<=35)
    {
        if(boss->status%3==2)
        {
            sound = new QSound(":/1/sound/092.wav");
            sound->play();
        }
        if(boss->status==19)
        {
            knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight2.png").scaled(3*constant_convert,4*constant_convert));
        }
        if(boss->status==11)
        {
            boss_ball=new character(900,boss->pos_y+constant_convert,QPixmap(":/1/image/boss_ball1.png").scaled(3*constant_convert,2*constant_convert),scene);
        }
        else
        {
            boss_ball=new character(900,boss->pos_y+constant_convert,QPixmap(":/1/image/boss_ball2.png").scaled(3*constant_convert,2*constant_convert),scene);
        }
        foreach (character * i, BallList)
        {
            i->item_pixmap.setX(i->pos_x-50);
            i->pos_x-=50;
            if(i->pos_x<=knight->pos_x+2*constant_convert)
            {
                boss->boss_ball_count++;
                if(boss->boss_ball_count%3==1)
                {
                    sound = new QSound(":/1/sound/085.wav");
                    sound->play();
                    knight->pos_x-=20;
                    knight->item_pixmap.setX(knight->pos_x);
                }
                else
                {
                    knight->pos_x+=10;
                    knight->item_pixmap.setX(knight->pos_x);
                }
                BallList.removeOne(i);
                delete i;
            }
        }
        BallList.push_back(boss_ball);
        boss->status++;
    }
    else if(boss->status<=50)
    {
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss3.png").scaled(2*constant_convert,3*constant_convert));
        foreach (character * i, BallList)
        {
            if(i->pos_x<=knight->pos_x+3*constant_convert)
            {
                boss->boss_ball_count++;

                if(boss->boss_ball_count%3==1&&boss->status<=42)
                {
                    sound = new QSound(":/1/sound/085.wav");
                    sound->play();
                    knight->pos_x-=20;
                    knight->item_pixmap.setX(knight->pos_x);
                }
                else if(boss->status<=42)
                {
                    knight->pos_x+=10;
                    knight->item_pixmap.setX(knight->pos_x);
                }
                else if(boss->status==43)
                {
                    knight->pos_x=knight->knight_tempx;
                    knight->item_pixmap.setX(knight->pos_x);
                    knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight15.png").scaled(3*constant_convert,4*constant_convert));
                    sound = new QSound(":/1/sound/032.wav");
                    sound->play();
                }
                else if(boss->status==47)
                {
                    knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight16.png").scaled(3*constant_convert,4*constant_convert));
                    sound = new QSound(":/1/sound/032.wav");
                    sound->play();
                }
                else if(boss->status==50)
                {
                    knight->pos_y+=40;
                    knight->item_pixmap.setY(knight->pos_y);
                    knight->pos_x+=20;
                    knight->item_pixmap.setX(knight->pos_x);
                    knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight9.png").scaled(4*constant_convert,3*constant_convert));

                    sound = new QSound(":/1/sound/033.wav");
                    sound->play();
                }
                BallList.removeOne(i);
                delete i;
            }
            else
            {
                i->item_pixmap.setX(i->pos_x-50);
                i->pos_x-=50;
            }
        }
        boss->status++;
        return ;
    }
    else if(boss->status==51)
    {
        boss->status++;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight10.png").scaled(4*constant_convert,3*constant_convert));
    }
    else if(boss->status==52)
    {
        knight->item_pixmap.setY(knight->pos_y);
        boss->status++;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight11.png").scaled(4*constant_convert,3*constant_convert));
        timer6.start(740);
    }
    else if(boss->status==53)
    {
        boss->status++;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight12.png").scaled(4*constant_convert,3*constant_convert));
    }
    else if(boss->status==54)
    {
        boss->status++;
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight13.png").scaled(4*constant_convert,3*constant_convert));
        timer6.start(1300);
    }
    else if(boss->status==55)
    {
        boss->status++;
        sound = new QSound(":/1/sound/061.wav");
        sound->play();
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show1.png").scaled(2*constant_convert,3*constant_convert));
        timer6.start(240);
    }
    else if(boss->status==56)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show2.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==57)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show3.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==58)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show4.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==59)
    {
        boss->status++;
        boss->item_pixmap.setPixmap(QPixmap(":/1/image/boss_show5.png").scaled(2*constant_convert,3*constant_convert));
    }
    else if(boss->status==60)
    {
        boss->status++;
        boss->item_pixmap.hide();
    }
    else if(boss->status==61)
    {
        sound = new QSound(":/1/sound/A14.wav");
        sound->play();
        knight->item_pixmap.setPixmap(QPixmap(":/1/image/knight17.png").scaled(4*constant_convert,3*constant_convert));
        boss->status++;
        timer6.start(1200);
    }
    else if(boss->status==62)
    {
        delete knight;
        boss->status++;
    }
    else if(boss->status==63)
    {
        sound = new QSound(":/1/sound/sad15.wav");
        sound->play();
        bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit14.png").scaled(2*constant_convert,2*constant_convert));
        boss->status++;
    }
    else if(boss->status==64)
    {
        bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit15.png").scaled(2*constant_convert,2*constant_convert));
        boss->status++;
    }
    else if(boss->status==65)
    {
        bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit1.png").scaled(2*constant_convert,2*constant_convert));
        boss->status++;
    }
    else if(boss->status==66)
    {
        sound = new QSound(":/1/sound/043.wav");
        sound->play();
        bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit14.png").scaled(2*constant_convert,2*constant_convert));
        boss->status++;
        magic=new character(153,525,QPixmap(":/1/image/magic1.png").scaled(4*constant_convert,2*constant_convert),scene);
    }
    else if(boss->status==67)
    {
        sound = new QSound(":/1/sound/043.wav");
        sound->play();
        delete magic;
        magic=new character(135,518,QPixmap(":/1/image/magic1.png").scaled(5*constant_convert,2.5*constant_convert),scene);
        boss->status++;
    }
    else if(boss->status==68)
    {
        sound = new QSound(":/1/sound/043.wav");
        sound->play();
        delete magic;
        magic=new character(118,511,QPixmap(":/1/image/magic1.png").scaled(6*constant_convert,3*constant_convert),scene);
        boss->status++;
    }
    else if(boss->status==69)
    {
        sound = new QSound(":/1/sound/043.wav");
        sound->play();
        delete magic;
        magic=new character(100,505,QPixmap(":/1/image/magic1.png").scaled(7*constant_convert,3.5*constant_convert),scene);
        boss->status++;
    }
    else if(boss->status==70)
    {
        sound = new QSound(":/1/sound/089.wav");
        sound->play();
        delete magic;
        magic=new character(90,0,QPixmap(":/1/image/magic2.png").scaled(7.4*constant_convert,18.2*constant_convert),scene);
        boss->status++;
    }
    else if(boss->status==71)
    {
        sound = new QSound(":/1/sound/093.wav");
        sound->play();
        delete magic;
        ghost=new MovableItem(4.9f,4.9f,2.0f,3.0f,7,2000000,8,0.7,0.1,&timer,QPixmap(":/1/image/ghost1.png").scaled(2*constant_convert,3*constant_convert),world,scene);
        ghost->item_body->SetGravityScale(0);
        ghost->item_body->SetFixedRotation(true);
        bandit->item_pixmap.setPixmap(QPixmap(":/1/image/bandit1.png").scaled(2*constant_convert,2*constant_convert));
        boss->status++;
        timer6.start(700);
    }
    else if(boss->status==72)
    {
        MI=new MovableItem(20.0f,4.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(20.0f,6.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(20.0f,8.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(20.0f,10.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(20.0f,12.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(20.0f,14.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(20.0f,16.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(40.0f,4.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(40.0f,6.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(40.0f,8.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(40.0f,10.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(40.0f,12.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(40.0f,14.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(40.0f,16.0f,2.0f,2.0f,3,15000,85,0.2,0.3,&timer,QPixmap(":/1/image/block1.png").scaled(2*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        //屋頂
        MI=new MovableItem(20.0f,18.0f,22.0f,2.0f,3,100000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(22*constant_convert,2*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(23.0f,10.0f,1.0f,8.0f,3,30000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(1*constant_convert,8*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(38.0f,10.0f,1.0f,8.0f,3,30000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(1*constant_convert,8*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(22.0f,11.0f,18.0f,1.0f,3,50000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(18*constant_convert,1*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        //人
        MI=new MovableItem(35.0f,17.0f,1.0f,9.6f,3,30000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(1*constant_convert,9.6*constant_convert),world,scene);
        MI->item_body->SetTransform(b2Vec2(MI->item_body->GetPosition().x,MI->item_body->GetPosition().y),65*3.1415/180);
        //MI->item_body->SetFixedRotation(true);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(26.0f,17.0f,1.0f,9.6f,3,30000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(1*constant_convert,9.6*constant_convert),world,scene);
        MI->item_body->SetTransform(b2Vec2(MI->item_body->GetPosition().x,MI->item_body->GetPosition().y),-65*3.1415/180);
        //MI->item_body->SetFixedRotation(true);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(30.5f,14.0f,1.0f,3.7f,3,15000,15,0.2,0.3,&timer,QPixmap(":/1/image/wood3.png").scaled(1*constant_convert,3.7*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        //內
        MI=new MovableItem(25.0f,3.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(25.0f,5.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(25.0f,6.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(25.0f,8.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(35.5f,3.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(35.5f,5.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(35.5f,6.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(35.5f,8.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(26.5f,3.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(26.5f,5.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(26.5f,6.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(26.5f,8.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(34.0f,3.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(34.0f,5.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(34.0f,6.5f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);
        MI=new MovableItem(34.0f,8.0f,1.5f,1.5f,3,6000,5,0.2,0.3,&timer,QPixmap(":/1/image/block2.png").scaled(1.5*constant_convert,1.5*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        MI=new MovableItem(25.0f,9.0f,12.0f,1.0f,3,30000,5,0.2,0.3,&timer,QPixmap(":/1/image/wood2.png").scaled(12.0*constant_convert,1.0*constant_convert),world,scene);
        ContactListener::itemList.push_back(MI);

        //中豬
        pig2=new MovableItem(29.5f,5.0f,3.0f,3.0f,9,60000,8,0.2,0.3,&timer,QPixmap(":/1/image/pig2.png").scaled(3*constant_convert,3*constant_convert),world,scene);
        ContactListener::itemList.push_back(pig2);

        condition=7;
        timer6.stop();
    }

}
void MainWindow::ghost_move()
{
    if(ghost->status==0)
    {
        sound = new QSound(":/1/sound/018.wav");
        sound->play();
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost2.png").scaled(2*constant_convert,3*constant_convert));
        ghost->status++;
    }
    else if(ghost->status==1)
    {
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost3.png").scaled(2*constant_convert,3*constant_convert));
        ghost->status++;
    }
    else if(ghost->status==2)
    {
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost4.png").scaled(2*constant_convert,3*constant_convert));
        ghost->status++;
    }
    else if(ghost->status==3)
    {
        sound = new QSound(":/1/sound/019.wav");
        sound->play();
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost5.png").scaled(2.5*constant_convert,3*constant_convert));
        ghost->status++;
        ghost_ball=new MovableItem(ghost->item_body->GetPosition().x+0.3,ghost->item_body->GetPosition().y+1.6,3.0f,3.0f,8,12500,9,0,0.2,&timer,QPixmap(":/1/image/ghost_ball1.png").scaled(3*constant_convert,3*constant_convert),world,scene);
        ghost_ball->item_body->SetFixedRotation(true);
        ghost_ball->item_body->SetGravityScale(0);
        ghost_ball->init_speedx=shoot_x;
        ghost_ball->init_speedy=shoot_y;
        ghost_ball->item_body->SetLinearVelocity(b2Vec2(shoot_x,shoot_y));
        ContactListener::itemList.push_back(ghost_ball);
        ghostBallList.push_back(ghost_ball);
    }
    else if(ghost->status==4)
    {
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost6.png").scaled(2.5*constant_convert,3*constant_convert));
        ghost->status++;
    }
    else if(ghost->status==5)
    {
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost7.png").scaled(2.5*constant_convert,3*constant_convert));
        ghost->status++;
    }
    else if(ghost->status==6)
    {
        ghost->item_pixmap.setPixmap(QPixmap(":/1/image/ghost1.png").scaled(2*constant_convert,3*constant_convert));
        ghost->status=0;
        ghost->skill=true;
        timer7.stop();
    }


}

void MainWindow::init()
{
    volume=100;
    condition=0;
    //0開始
    //1小兵走路中
    //2小兵就定位
    //3已按下press
    //4小兵跳
    //5鳥在空中
    //6動畫中
    //7動畫結束
    //8已按下press
    //9小兵跳
    //10ghost在空中
    //11遊戲結束

    //item_type
    //type1 ground
    //type2 bird1
    //type3 wood1
    //type4 bird2
    //type5 bird3
    //type6 pig1
    //type7 ghost
    //type8 ghost_ball
    //type9 pig2
    mini_ready=1;
    remove_delay=0;
    pig1_is_died=false;
    pig2_is_died=false;
    bir1=false;
    bir2=false;
    bir3=false;
    ContactListener::game_score=0;
    game_score_temp=0;

    bgm=new QMediaPlayer;
    bgm->setMedia(QUrl::fromLocalFile("./sound/bgm.wav"));
    bgm->play();
    bgm->setVolume(100);


    view->setGeometry(0,0,screenW,screenH);
    view->setScene(scene);
    view->centerOn(0,0);

    world = new b2World(b2Vec2(0.0f, -9.8f));
    item::setItemWorldSize(QSizeF(constant*2,constant),QSize(screenW,screenH));

    CL=new ContactListener();
    world->SetContactListener(CL);


    //背景
    back=new QGraphicsPixmapItem();
    back->setPixmap(QPixmap(":/1/image/bg.png").scaled(1551,569));
    back->setPos(35,0);
    scene->addItem(back);

    //靶子
    back=new QGraphicsPixmapItem();
    back->setPixmap(QPixmap(":/1/image/shoot.png").scaled(200,200));
    back->setPos(200,300);
    scene->addItem(back);

    //score
    score1=new QGraphicsPixmapItem();
    scene->addItem(score1);
    score2=new QGraphicsPixmapItem();
    scene->addItem(score2);
    score3=new QGraphicsPixmapItem();
    scene->addItem(score3);
    score4=new QGraphicsPixmapItem();
    scene->addItem(score4);
    score5=new QGraphicsPixmapItem();
    scene->addItem(score5);
    score6=new QGraphicsPixmapItem();
    scene->addItem(score6);
    score7=new QGraphicsPixmapItem();
    scene->addItem(score7);
    score8=new QGraphicsPixmapItem();
    scene->addItem(score8);

    UMI=new UnMovableItem(0.0f,2.0f,72.0f,2.0f,1,&timer,QPixmap(":/1/image/ground.png").scaled(72*constant_convert,2*constant_convert),world,scene);
    UMI=new UnMovableItem(0.0f,25.0f,72.0f,2.0f,1,&timer,QPixmap(":/1/image/ground.png").scaled(72*constant_convert,2*constant_convert),world,scene);
    UMI=new UnMovableItem(44.0f,38.0f,6.0f,37.0f,1,&timer,QPixmap(":/1/image/ground.png").scaled(6*constant_convert,37*constant_convert),world,scene);
    UMI=new UnMovableItem(-5.0f,38.0f,6.0f,37.0f,1,&timer,QPixmap(":/1/image/ground.png").scaled(6*constant_convert,37*constant_convert),world,scene);

    knight=new character(100,432,QPixmap(":/1/image/knight1.png").scaled(3*constant_convert,4*constant_convert),scene);
    bandit=new character(25,505,QPixmap(":/1/image/bandit1.png").scaled(2*constant_convert,2*constant_convert),scene);
    boss=new character(-100,-100,QPixmap(":/1/image/boss_show1.png").scaled(2*constant_convert,3*constant_convert),scene);



    //主架構
    MI=new MovableItem(27.0f,7.0f,1.0f,5.0f,3,10500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,5*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(33.0f,7.0f,1.0f,5.0f,3,10500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,5*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(27.0f,8.0f,7.0f,1.0f,3,10500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood2.png").scaled(7*constant_convert,1*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    //左小

    MI=new MovableItem(22.0f,5.0f,1.0f,3.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(24.0f,5.0f,1.0f,3.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(22.0f,6.0f,3.0f,1.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood2.png").scaled(3*constant_convert,1*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);


    //右小
    MI=new MovableItem(36.0f,5.0f,1.0f,3.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(38.0f,5.0f,1.0f,3.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(36.0f,6.0f,3.0f,1.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood2.png").scaled(3*constant_convert,1*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    //中小
    MI=new MovableItem(29.0f,11.0f,1.0f,3.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(31.0f,11.0f,1.0f,3.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood1.png").scaled(1*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    MI=new MovableItem(29.0f,12.0f,3.0f,1.0f,3,5500,15,0.2,0.3,&timer,QPixmap(":/1/image/wood2.png").scaled(3*constant_convert,1*constant_convert),world,scene);
    ContactListener::itemList.push_back(MI);

    //中豬
    pig1=new MovableItem(29.0f,4.0f,3.0f,3.0f,6,7000,8,0.2,0.3,&timer,QPixmap(":/1/image/pig1.png").scaled(3*constant_convert,3*constant_convert),world,scene);
    ContactListener::itemList.push_back(pig1);
}
void MainWindow::result_jump()
{
    char temp[100];


    if(ContactListener::game_score-game_score_temp<3)
    {
        timer8.start(280);
    }
    else if(ContactListener::game_score-game_score_temp<10)
    {
        timer8.start(200);
    }
    else if(game_score_temp*100/ContactListener::game_score<50)
    {
     timer8.start(10);
    }
    else if(game_score_temp*100/ContactListener::game_score<33)
    {
     timer8.start(12);
    }
    else if (game_score_temp*100/ContactListener::game_score<25)
    {
     timer8.start(14);
    }
    else if(game_score_temp*100/ContactListener::game_score<9)
    {
     timer8.start(20);
    }
    else if(game_score_temp*100/ContactListener::game_score<2)
    {
     timer8.start(25);
    }
    else if(game_score_temp*100/ContactListener::game_score<1)
    {
     timer8.start(50);
    }
    else
    {
     timer8.start(70);
    }
game_score_temp=game_score_temp+(ContactListener::game_score-game_score_temp)/13+1;
if(game_score_temp>ContactListener::game_score)
{
    game_score_temp=ContactListener::game_score;
}

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,game_score_temp%10);
strcat(temp,".png");
score1->setPixmap(QPixmap(temp).scaled(50,66));
score1->setPos(1050,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%100-game_score_temp%10)/10);
strcat(temp,".png");
score2->setPixmap(QPixmap(temp).scaled(50,66));
score2->setPos(1000,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%1000-game_score_temp%100)/100);
strcat(temp,".png");
score3->setPixmap(QPixmap(temp).scaled(50,66));
score3->setPos(950,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%10000-game_score_temp%1000)/1000);
strcat(temp,".png");
score4->setPixmap(QPixmap(temp).scaled(50,66));
score4->setPos(900,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%100000-game_score_temp%10000)/10000);
strcat(temp,".png");
score5->setPixmap(QPixmap(temp).scaled(50,66));
score5->setPos(850,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%1000000-game_score_temp%100000)/100000);
strcat(temp,".png");
score6->setPixmap(QPixmap(temp).scaled(50,66));
score6->setPos(800,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%10000000-game_score_temp%1000000)/1000000);
strcat(temp,".png");
score7->setPixmap(QPixmap(temp).scaled(50,66));
score7->setPos(750,0);

strcpy(temp,":/1/image/");
sprintf(temp,"%s%d",temp,(game_score_temp%100000000-game_score_temp%10000000)/10000000);
strcat(temp,".png");
score8->setPixmap(QPixmap(temp).scaled(50,66));
score8->setPos(700,0);
}
