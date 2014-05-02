#include "bikerack.h"
#include <QBrush>
#include <QTextCodec>
#include <QDebug>
#include <QtCore/qmath.h>

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
        //qWarning() << "Error: Rack at " << desc << " has capacity 0";
        return;
    }

    /* Calculate the color that should be shown for this amount of bikes */
    qreal percentage = qreal(bikes) / qreal(capacity);
    percentage = percentage * 100;

    int hue =  qFloor((100 - percentage) * 120/ 100);  // go from green to red
    //int saturation = 1;   // fade to white as it approaches 50
    changeLum(hue, 255,255);
}

void BikeRack::changeLum(int H, int S, int V)
{
    QBrush * brush = new QBrush(QColor::fromHsv(H,S,V));
    setBrush(* brush);

}
