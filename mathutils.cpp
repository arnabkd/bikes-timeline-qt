#include "mathutils.h"
#include <QtCore/qmath.h>

qreal MathUtils::convertLatitudeToY(qreal latitude, qreal minLatitude, qreal maxLatitude, qreal height)
{
    qreal multiplier = (latitude - minLatitude) / (maxLatitude - minLatitude);
    return height - (multiplier * height);
}

qreal MathUtils::convertLongitudeToX(qreal longitude, qreal minLongitude, qreal maxLongitude, qreal width)
{
    qreal multiplier = (longitude - minLongitude) / (maxLongitude - minLongitude);
    return width * multiplier;
}

void MathUtils::padBorders(qreal percentage, qreal &min, qreal &max)
{
    /* Find the amount to pad min and max values by */
    qreal borderY = (max - min) * (percentage/100);
    max = max + borderY;
    min = min - borderY;
}

int MathUtils::convertPercentageToHue(int percentage)
{
    int hue =  qFloor((100 - percentage) * 120/ 100);  // span hue from 0-120 (red to green)
    return hue;
}
