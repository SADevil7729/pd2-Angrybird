#ifndef ITEM_H
#define ITEM_H

#include <QSound>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTransform>
#include <QtMath>
#include <math.h>
#include <Box2D/Box2D.h>
#include <iostream>

#include <QPixmap>
class item : public QObject
{
    Q_OBJECT

public:
    item(b2World *world);
    static void setItemWorldSize(QSizeF worldsize, QSizeF windowsize);

    b2Body *item_body;
    QGraphicsPixmapItem item_pixmap;
    QSizeF item_size;
    QGraphicsScene *item_scene;
    b2World *item_world;
    int item_type;
    bool skill;

    static QSizeF item_worldsize,item_windowsize;
    void setSpeed(b2Vec2 v);
signals:

public slots:
    void item_show();
    void item_show2();

};

#endif // ITEM_H
