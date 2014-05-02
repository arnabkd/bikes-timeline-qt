#include "bikerack.h"
#include <QBrush>
#include <QTextCodec>
#include <QDebug>
#include <QGraphicsBlurEffect>
#include <mathutils.h>

BikeRack::BikeRack(qreal longitude, qreal latitude, int capacity, QString desc) :
    QGraphicsEllipseItem(0,0,capacity, capacity),
    capacity(capacity),
    longitude(longitude),
    latitude(latitude),
    desc(desc)
{
    setToolTip(desc);
    updateNumBikes(0);
    setOpacity(0.8);

    QGraphicsBlurEffect * blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(1.5);
    setGraphicsEffect(blurEffect);
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
    qreal percentage = (qreal(bikes) / qreal(capacity))*100;

    changeLum(MathUtils::convertPercentageToHue(percentage),255,255);
}

void BikeRack::changeLum(int H, int S, int V)
{
    QBrush * brush = new QBrush(QColor::fromHsv(H,S,V));
    setBrush(* brush);

}
