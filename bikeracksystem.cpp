#include "bikeracksystem.h"
#include "bikerack.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

BikeRackSystem::BikeRackSystem(qreal height, qreal width, QWidget *parent) :
    QWidget(parent)
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    view = new QGraphicsView(scene);

    view->showMaximized();
    view->setParent(this);

    minLatitude = -999;
    minLongitude = -999;

    maxLatitude = 0;
    maxLongitude = 0;

    this->height = height;
    this->width = width;

}

qreal BikeRackSystem::getY(qreal latitude)
{
  qreal diffY = latitude - minLatitude;

  return height - (diffY*height);
}

qreal BikeRackSystem::getX(qreal longitude)
{
  qreal diffX = longitude - minLongitude;
  qDebug() << "diffX is " << diffX;
  return width * diffX;
}

QJsonDocument BikeRackSystem::getJsonContents(QString jsonfile)
{
  QFile loadFile(jsonfile);

  if (!loadFile.open(QIODevice::ReadOnly))
  {
          qWarning("Couldn't open save file.");
          return QJsonDocument();
  }

  QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());

  return loadDoc;
}

void BikeRackSystem::doCalculations(QJsonDocument doc)
{
   QJsonObject obj = doc.object();
   QStringList rackIDs = obj.keys();

   for (int i = 0; i < rackIDs.size(); i++)
   {
       QJsonObject rackObj = obj[rackIDs[i]].toObject();
       addNewBikeRack(rackObj);
   }

   qDebug() << minLatitude;
   qDebug() << minLongitude;

   foreach (BikeRack * rack, bikeracks.values()) {
       qDebug() << "Rack at " << rack->getDesc();
       qreal rackX = getX(rack->getLongitude());
       qreal rackY = getY(rack->getLatitude());
       qDebug() << "X: " << rackX << " longitude : " << rack->getLongitude();
       qDebug() << "Y: " << rackY << " latitude : " << rack->getLatitude();

       rack->setPos(rackX, rackY);
       rack->setBrush(* new QBrush(QColor::fromRgb(0,255,0)));
       scene->addItem(rack);
   }

}

void BikeRackSystem::addNewBikeRack(QJsonObject rackObj)
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

    BikeRack * rack = new BikeRack(longitude, latitude, capacity, 0,0,10,10, desc);
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

    qDebug() << "Found a racks.json file";

    QJsonDocument doc = getJsonContents(jsonfile);

    doCalculations(doc);
    return true;
}
