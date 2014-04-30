#include "bikerack.h"
#include <QBrush>

BikeRack::BikeRack(qreal longitude, qreal latitude, int capacity,
                   qreal x, qreal y, qreal width, qreal height) :
    QGraphicsEllipseItem(x,y,width, height)
{
    this->capacity = capacity;
    this->bikes = 0;
    this->longitude = longitude;
    this->latitude = latitude;
    changeLum(0,255,0);
}

void BikeRack::changeLum(int R, int G, int B)
{
    QBrush * brush = new QBrush(QColor::fromRgb(R,G,B,150));
    setBrush(* brush);
}
