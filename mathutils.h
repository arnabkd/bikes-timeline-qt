#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QtGlobal>

class MathUtils
{
public:
    static qreal convertLatitudeToY(qreal latitude, qreal minLatitude, qreal maxLatitude, qreal height);
    static qreal convertLongitudeToX(qreal longitude, qreal minLongitude, qreal maxLongitude, qreal width);

    static void padBorders(qreal percentage, qreal &min, qreal &max);

    static int convertPercentageToHue(int percentage);

};

#endif // MATHUTILS_H
