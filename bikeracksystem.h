#ifndef BIKERACKSYSTEM_H
#define BIKERACKSYSTEM_H

#include <QObject>
#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class BikeRack;
class RackStatus;

class BikeRackSystem : public QWidget
{
    Q_OBJECT
public:
    explicit BikeRackSystem(qreal height, qreal width, QWidget *parent = 0);
    bool setDataFolder(QString path);

    void setCurrentIndex(int index);

public slots:
    void nextStatus();
    void previousStatus();
    void loadDataSet();

private:
    QGraphicsScene * scene;
    QGraphicsView  * view;

    qreal minLatitude;
    qreal maxLatitude;
    qreal minLongitude;
    qreal maxLongitude;
    qreal height;
    qreal width;

    QHash <int, BikeRack *>  bikeracks;

    QList <RackStatus *> timeline;
    int currentStatusIndex;

    QString dataFolder;

    QJsonDocument getJsonContents(QString jsonfile);

    void loadRacksFromFile(QJsonDocument doc);
    void createBikeRack(QJsonObject rackObj);
    void addRacksToScene();
    void padBoundaries(qreal percentage, qreal &min, qreal &max);

    void setupTimeline(QString dataFolder);
    void updateStatus();

    qreal getY(qreal latitude);
    qreal getX(qreal longitude);

    QString getDateStr(qreal epoch);

    void reset();

signals:
    void message(QString message);
    void datasetReadyToBeLoaded();
    void datasetLoaded();
    void endOfDataset();

};

#endif // BIKERACKSYSTEM_H
