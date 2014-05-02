#include "bikeracksystem.h"
#include "bikerack.h"
#include "rackstatus.h"
#include "mathutils.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QJsonArray>
#include <QDateTime>

BikeRackSystem::BikeRackSystem(qreal height, qreal width, QWidget *parent) :
    QWidget(parent)
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    view = new QGraphicsView(scene);

    view->showMaximized();
    view->setParent(this->parentWidget());

    this->height = height;
    this->width = width;

    reset();
}


QString BikeRackSystem::getDateStr(qreal epoch)
{
    const QDateTime dt = QDateTime::fromTime_t(epoch);
    const QString textdate = dt.toString( Qt::TextDate );

    return textdate;
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

    //padBoundaries(5, minLatitude, maxLatitude);
    //padBoundaries(5, minLongitude, maxLongitude);

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

    int i = 0;
    qDebug() << statusFiles.size() << " files to be added.";

    foreach (QString fileName, statusFiles)
    {
       emit message("loaded " + QString::number(i) + " files");
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
       }

       RackStatus * rackstatus = new RackStatus(epoch, city, availableBikes);
       timeline.append(rackstatus);
    }

    currentStatusIndex = 0;
    updateStatus();

}

void BikeRackSystem::addRacksToScene()
{
    foreach (BikeRack * rack, bikeracks.values())
    {
        qreal rackX = MathUtils::convertLongitudeToX(rack->getLongitude(),minLongitude, maxLongitude, width);
        qreal rackY = MathUtils::convertLatitudeToY(rack->getLatitude(), minLatitude, maxLatitude, height);

        rack->setX(rackX);
        rack->setY(rackY);
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
    BikeRack * rack = new BikeRack(longitude, latitude, capacity, desc);
    bikeracks[rackID] = rack;
}

bool BikeRackSystem::setDataFolder(QString path)
{
    qDebug() << "datafolder set to " << path;
    if(!QFile(path + "racks.json").exists())
    {
        qDebug() << "Couldn't find a racks.json file";
        return false;
    }

    dataFolder = path;
    reset();

    return true;
}

void BikeRackSystem::setCurrentIndex(int index)
{
    currentStatusIndex = index;
    updateStatus();
}

void BikeRackSystem::nextStatus()
{
    if ((currentStatusIndex+1) < timeline.size())
        currentStatusIndex++;
    updateStatus();
}

void BikeRackSystem::previousStatus()
{
    if ((currentStatusIndex-1) >= 0)
        currentStatusIndex--;
    updateStatus();
}

void BikeRackSystem::loadDataSet()
{
    QString jsonfile = dataFolder + "racks.json";
    QJsonDocument doc = getJsonContents(jsonfile);
    loadRacksFromFile(doc);
    setupTimeline(dataFolder);
    emit datasetLoaded();
}

void BikeRackSystem::updateStatus()
{
   if (currentStatusIndex < 0 || currentStatusIndex >= timeline.size())
   {
       qWarning("Reached illegal status index");
       return;
   }

   RackStatus * currentStatus = timeline[currentStatusIndex];

   emit message(currentStatus->getCityName() + " bike racks status at " + getDateStr(currentStatus->getTime()));
   QHash<int,int> status = currentStatus->getStatus();

   if (currentStatusIndex == (timeline.size() -1))
   {
       emit message(currentStatus->getCityName()
                       + " bike racks status at " +
                       getDateStr(currentStatus->getTime())
                       + " (end of dataset)");
       emit endOfDataset();
   }

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
