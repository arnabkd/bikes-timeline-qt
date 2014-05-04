#include "mathutils.h"

#include <QtCore/qmath.h>

/*!
 * \brief MathUtils::getRescaleMultiplier
 * Rescaling function.
 * Used to transform [minValue, ..., value, ... maxValue]
 * to [0...newRange]
 *
 * Calculate the multiplier function first:
 * f(x) = (x-min) / (max-min)
 * f(min) = (min-min) / (max-min) = 0
 * f(max) = (max-min) / (max-min) = 0
 *
 * Then multiply with the newRange param to get the return value.
 * If desired, the value might be flipped to so that the range goes from [newRange, newRange-1,...,0]
 * \param value
 * \param minValue
 * \param maxValue
 * \return
 */
qreal MathUtils::getRescaledValue(qreal value, qreal minValue, qreal maxValue, qreal newRange, bool flip)
{
    qreal multiplier = (value - minValue) / (maxValue - minValue);
    qreal newVal = multiplier * newRange;

    return flip? (newRange - newVal) : newVal;
}

/*!
 * \brief MathUtils::padBorders
 * Increases the bounds of a range by the given percentage.
 * Calculates the length of the range given, multiply by the percentage.
 * Then adds that value to the max in the range, and subtracts from the minimum.
 * \param percentage
 * \param min
 * \param max
 */
void MathUtils::padBorders(qreal percentage, qreal &min, qreal &max)
{
    /* Find the amount to pad min and max values by */
    qreal borderY = (max - min) * (percentage/100);
    max = max + borderY;
    min = min - borderY;
}
