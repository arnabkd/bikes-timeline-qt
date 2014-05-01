#include "bikeracksystem.h"
#include "bikerack.h"
#include "rackstatus.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QJsonArray>

BikeRackSystem::BikeRackSystem(qreal height, qreal width, QWidget *parent) :
    QWidget(parent)
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    view = new QGraphicsView(scene);

    view->showMaximized();
    view->setParent(this);

    this->height = height;
    this->width = width;

    reset();
}

qreal BikeRackSystem::getY(qreal latitude)
{
    qreal multiplier = (latitude - minLatitude) / (maxLatitude - minLatitude);
    return height - (multiplier * height);
}

qreal BikeRackSystem::getX(qreal longitude)
{
    qreal multiplier = (longitude - minLongitude) / (maxLongitude - minLongitude);
    return width * multiplier;
}

QJsonDocument BikeRackSystem::getJsonContents(QString jsonfile)
{
    QFile loadFile(jsonfile);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open file:");
        return QJsonDocument();
    }

    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());
    return loadDoc;
}

void BikeRackSystem::loadRacksFromFile(QJsonDocument doc)
{
    QJsonObject obj = doc.object();
    QStringList rackIDs = obj.keys();

    for (int i = 0; i < rackIDs.size(); i++)
    {
        QJsonObject rackObj = obj[rackIDs[i]].toObject();
        createBikeRack(rackObj);
    }

    padBoundaries(10, minLatitude, maxLatitude);
    padBoundaries(10, minLongitude, maxLongitude);

    addRacksToScene();
}

void BikeRackSystem::padBoundaries(qreal percentage, qreal &min, qreal &max)
{
    /* Find the amount to pad min and max values by */
    qreal borderY = (max - min) * (percentage/100);
    max = max + borderY;
    min = min - borderY;
}

void BikeRackSystem::setupTimeline(QString dataFolder)
{
    /* Add all json files */
    QDir dir(dataFolder);
    QStringList filters;
    filters.append("*.json");
    dir.setNameFilters(filters);

    QStringList statusFiles = dir.entryList();
    statusFiles.removeAll("racks.json");

    foreach (QString fileName, statusFiles)
    {
       QJsonDocument doc = getJsonContents(dataFolder + "/" +fileName);
       QJsonObject obj = doc.object();

       qreal epoch = obj["time"].toDouble();
       QString city = obj["city"].toString();

       QJsonArray stations = obj["stations"].toArray();
       QHash<int,int> availableBikes;

       foreach (QJsonValue station, stations)
       {
           QJsonObject rackStatus = station.toObject();
           int id = rackStatus["id"].toInt();
           int bikes = rackStatus["bikes"].toInt();
           availableBikes[id] = bikes;
           //qDebug() << rackID.toInt() << " -> " << rack["bikes"].toInt();
       }

       RackStatus * rackstatus = new RackStatus(epoch, city, availableBikes);
       timeline.append(rackstatus);
       //qDebug() << availableBikes;
    }

    currentStatusIndex = 0;
    qDebug() << "timeline now has " << timeline.size() << " files";
    updateStatus();

}

void BikeRackSystem::addRacksToScene()
{
    foreach (BikeRack * rack, bikeracks.values())
    {
        qreal rackX = getX(rack->getLongitude());
        qreal rackY = getY(rack->getLatitude());
        rack->setRect(rackX, rackY, 10,10);
        scene->addItem(rack);
    }
}

void BikeRackSystem::createBikeRack(QJsonObject rackObj)
{
    qreal latitude = rackObj["latitude"].toDouble();
    qreal longitude = rackObj["longitude"].toDouble();

    /* Update the minLatitude, maxLatitude as required */
    if (minLatitude > latitude || minLatitude == -999)
        minLatitude = latitude;
    if (maxLatitude <= latitude)
        maxLatitude = latitude;

    /* Update the minLongitude, maxLongitude as requried */
    if (minLongitude > longitude || minLongitude == -999)
        minLongitude = longitude;
    if (maxLongitude <= longitude)
        maxLongitude = longitude;

    int capacity = rackObj["capacity"].toInt();
    int rackID = rackObj["id"].toInt();
    QString desc = rackObj["description"].toString();
    BikeRack * rack = new BikeRack(longitude, latitude, capacity,10,10, desc);
    bikeracks[rackID] = rack;
}

bool BikeRackSystem::setDataFolder(QString path)
{
    QString jsonfile = path + "racks.json";
    QFile Fout(jsonfile);

    if(!Fout.exists())
    {
        qDebug() << "Couldn't find a racks.json file";
        return false;
    }

    qDebug() << "Cleaning up old objects first";
    reset();

    QJsonDocument doc = getJsonContents(jsonfile);

    loadRacksFromFile(doc);
    setupTimeline(path);
    return true;
}

void BikeRackSystem::nextStatus()
{
    qDebug() << "Showing next status";
    if ((currentStatusIndex+1) < timeline.size())
        currentStatusIndex++;
    updateStatus();
}

void BikeRackSystem::previousStatus()
{
    qDebug() << "Showing previous status";
    if ((currentStatusIndex-1) >= 0)
        currentStatusIndex--;
    updateStatus();
}

void BikeRackSystem::updateStatus()
{
   if (currentStatusIndex < 0 || currentStatusIndex >= timeline.size())
   {
       qWarning("Reached illegal status index");
       return;
   }

   RackStatus * currentStatus = timeline[currentStatusIndex];

   qDebug() << "showing status at " << currentStatus->getTime();
   QHash<int,int> status = currentStatus->getStatus();

   foreach (int rackID, status.keys()) {
      bikeracks[rackID]->updateNumBikes(status[rackID]);
   }


}

void BikeRackSystem::reset()
{
    minLatitude = -999;
    minLongitude = -999;

    maxLatitude = 0;
    maxLongitude = 0;

    foreach (BikeRack *rack, bikeracks.values())
        scene->removeItem(rack);

    bikeracks.clear();
}
