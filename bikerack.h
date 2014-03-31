#ifndef BIKERACK_H
#define BIKERACK_H

#include <QGraphicsEllipseItem>

class BikeRack : public QGraphicsEllipseItem
{
public:
    BikeRack(qreal longitude, qreal latitude, int capacity,
             qreal x, qreal y, qreal width, qreal height);

    void setPos(qreal x, qreal y) {setPos(x,y); }
    void changeLum(int R, int G, int B);

private:
    qreal longitude;
    qreal latitude;

    int capacity;
    int bikes;
};

#endif // BIKERACK_H
