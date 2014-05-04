#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QtGlobal>

class MathUtils
{
public:
    static qreal getRescaledValue(qreal value, qreal minValue, qreal maxValue, qreal newRange, bool flip= false);

    static void padBorders(qreal percentage, qreal &min, qreal &max);

    static int convertPercentageToHue(int percentage);

};

#endif // MATHUTILS_H
