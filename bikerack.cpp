#include "bikerack.h"
#include <QBrush>
#include <QTextCodec>

BikeRack::BikeRack(qreal longitude, qreal latitude, int capacity,
                   qreal x, qreal y, qreal width, qreal height, QString desc) :
    QGraphicsEllipseItem(x,y,width, height)
{
    /* Set this in order to handle norwegian names in the tooltips for the bike racks*/


    this->capacity = capacity;
    this->bikes = 0;
    this->longitude = longitude;
    this->latitude = latitude;
    changeLum(0,255,0);

    this->desc = desc;
    setToolTip(desc);


}

void BikeRack::changeLum(int R, int G, int B)
{
    QBrush * brush = new QBrush(QColor::fromRgb(R,G,B,150));
    setBrush(* brush);
}
