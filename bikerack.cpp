#include "bikerack.h"
#include <QBrush>
#include <QTextCodec>
#include <QDebug>

BikeRack::BikeRack(qreal longitude, qreal latitude, int capacity,qreal width, qreal height, QString desc) :
    QGraphicsEllipseItem(0,0,width, height)
{
    this->capacity = capacity;
    this->longitude = longitude;
    this->latitude = latitude;
    this->desc = desc;

    setToolTip(desc);
    updateNumBikes(0);
}

void BikeRack::updateNumBikes(int bikes)
{
    this->bikes = bikes;

    if (capacity == 0)
    {
        qDebug() << "Error: Rack at " << desc << " has capacity 0";
        return;
    }

    /* Calculate the color that should be shown for this amount of bikes */
    qreal percentage = qreal(bikes) / qreal(capacity);

    qreal redMult = 1 - percentage;
    qreal greenMult = percentage;

    int R = (int) (redMult * 255);
    int G = (int) (greenMult * 255);
    changeLum(R,G,0);
}

void BikeRack::changeLum(int R, int G, int B)
{
    QBrush * brush = new QBrush(QColor::fromRgb(R,G,B,255));
    setBrush(* brush);

}
