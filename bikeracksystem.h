#ifndef BIKERACKSYSTEM_H
#define BIKERACKSYSTEM_H

#include <QObject>
#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class BikeRack;

class BikeRackSystem : public QWidget
{
    Q_OBJECT
public:
    explicit BikeRackSystem(QWidget *parent = 0);
    bool setDataFolder(QString path);

signals:

public slots:

private:
    QJsonDocument getJsonContents(QString jsonfile);
    void doCalculations(QJsonDocument doc);

    QGraphicsScene         * scene;
    QGraphicsView          * view;

    double                   minLatitude;
    double                   maxLatitude;
    double                   minLongitude;
    double                   maxLongitude;

    QHash <int, BikeRack>  * bikeracks;
};

#endif // BIKERACKSYSTEM_H
