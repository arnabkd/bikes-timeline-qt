#ifndef BIKERACKSYSTEM_H
#define BIKERACKSYSTEM_H

#include <QObject>
#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class BikeRack;
class RackStatus;
class DataSetLoader;

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
    void dataObjectsRead();

private:
    QGraphicsScene * scene;
    QGraphicsView  * view;

    DataSetLoader * datasetLoader;

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

    void addRacksToScene();

    void updateDisplay();

    QString getDateStr(qreal epoch);


signals:
    void message(QString message);
    void datasetLoaded();
    void endOfDataset();

};

#endif // BIKERACKSYSTEM_H
