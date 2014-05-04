#include "bikerack.h"
#include <QBrush>
#include <QTextCodec>
#include <QDebug>
#include <QGraphicsBlurEffect>
#include <mathutils.h>


BikeRack::BikeRack(qreal longitude, qreal latitude, int capacity, QString desc) :
    QGraphicsEllipseItem(0,0,9,9),
    longitude(longitude), latitude(latitude),
    capacity(capacity), desc(desc)
{
    setToolTip(desc + " (capacity: " + QString::number(capacity) + ")");
    updateNumBikes(0);
    setOpacity(0.8);

    QGraphicsBlurEffect * blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(1.8);
    setGraphicsEffect(blurEffect);

}



/*!
 * \brief BikeRack::updateNumBikes
 *        Updates the number of bikes free at this bikerack object.
 *        This method also calls on a math function to calculate a HSV value
 *        and calls the BikeRack::changeLum method with that value as input.
 * \param bikes
 */
void BikeRack::updateNumBikes(int bikes)
{
    this->bikes = bikes;

    /*
       Data files might contain bike racks that are offline/out of service.
       The capacity field is set to 0 for these racks. This check is to prevent
       division by zero errors.
    */
    if (capacity == 0)
    {
        //qWarning() << "Error: Rack at " << desc << " has capacity 0";

        return;
    }

    if (bikes > capacity)
        bikes = capacity;

    /* Calculate the hue that should be shown for this amount of bikes */
    qreal percentage = (qreal(bikes) / qreal(capacity))*100;
    int hue = MathUtils::convertPercentageToHue(percentage);

    changeLum(hue,255,255);

}
/*!
 * \brief BikeRack::changeLum
 *        Changes the color of the bikerack to the given HSV code.
 * \param H
 * \param S
 * \param V
 */

void BikeRack::changeLum(int H, int S, int V)
{
    QBrush * brush = new QBrush(QColor::fromHsv(H,S,V));
    setBrush(* brush);

}
