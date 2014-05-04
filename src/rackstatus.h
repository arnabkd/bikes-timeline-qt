#ifndef RACKSTATUS_H
#define RACKSTATUS_H

#include <QObject>
#include <QHash>

class RackStatus : public QObject
{
    Q_OBJECT
public:
    explicit RackStatus(qreal epoch, QString city,
                        QHash<int,int> availableBikes, QObject *parent = 0);


    QHash<int,int> getStatus() { return availableBikes; }
    qreal getTime() { return epoch; }
    QString getCityName() { return city; }

private:
    qreal epoch;
    QString city;
    QHash<int,int> availableBikes;
};

#endif // RACKSTATUS_H
