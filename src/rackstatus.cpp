#include "rackstatus.h"
#include <QDebug>

RackStatus::RackStatus(qreal epoch, QString city,
                       QHash<int,int> availableBikes,QObject *parent) :
    QObject(parent), epoch(epoch), city(city), availableBikes(availableBikes)
{
    //qDebug() << availableBikes;
}
