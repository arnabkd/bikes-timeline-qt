#ifndef BIKERACK_H
#define BIKERACK_H

#include <QGraphicsEllipseItem>

class BikeRack : public QGraphicsEllipseItem
{
public:
    BikeRack(qreal longitude, qreal latitude, int capacity, qreal width, qreal height, QString desc="");

    void setPos(qreal x, qreal y) { QGraphicsEllipseItem::setPos(x,y); }
    void updateNumBikes(int bikes);

    /* Getters */
    QString getDesc() { return desc;      }
    qreal getLatitude()  { return latitude;  }
    qreal getLongitude() { return longitude; }

private:
    qreal longitude;
    qreal latitude;

    int capacity;
    int bikes;

    QString desc;
    void changeLum(int R, int G, int B);
};

#endif // BIKERACK_H
