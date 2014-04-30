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
    explicit BikeRackSystem(qreal height, qreal width, QWidget *parent = 0);
    bool setDataFolder(QString path);

signals:

public slots:

private:
    QJsonDocument getJsonContents(QString jsonfile);
    void doCalculations(QJsonDocument doc);
    void addNewBikeRack(QJsonObject rackObj);

    qreal getY(qreal latitude);
    qreal getX(qreal longitude);

    QGraphicsScene * scene;
    QGraphicsView  * view;

    qreal minLatitude;
    qreal maxLatitude;
    qreal minLongitude;
    qreal maxLongitude;

    qreal height;
    qreal width;


    QHash <int, BikeRack *>  bikeracks;
};

#endif // BIKERACKSYSTEM_H
