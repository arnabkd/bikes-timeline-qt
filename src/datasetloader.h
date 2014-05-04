#ifndef DATASETLOADER_H
#define DATASETLOADER_H

#include <QObject>
#include "bikerack.h"
class RackStatus;


class DataSetLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataSetLoader(QString dataFolder);

    qreal getMinLatitude() { return minLatitude; }
    qreal getMaxLatitude() { return maxLatitude; }
    qreal getMinLongitude() { return minLongitude; }
    qreal getMaxLongitude() { return maxLongitude; }

    QHash <int, BikeRack *> getBikeRacks() { return bikeracks; }
    QList <RackStatus *> getTimeline() { return timeline; }

signals:
    void datasetLoaded();
    void loadingStatus(QString);

public slots:
    void load();

private:
    QString dataFolder;

    qreal minLatitude;
    qreal maxLatitude;
    qreal minLongitude;
    qreal maxLongitude;

    QHash <int, BikeRack *>  bikeracks;
    QList <RackStatus *> timeline;

    void loadRacks();
    void loadDataSet();

    void createBikeRack(QJsonObject rackObject);

    QJsonDocument getJsonContents(QString jsonfile);

};

#endif // DATASETLOADER_H
