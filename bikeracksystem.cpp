#include "bikeracksystem.h"
#include "bikerack.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

BikeRackSystem::BikeRackSystem(QWidget *parent) :
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

    bikeracks = new QHash<int, BikeRack>();
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

       double latitude = rackObj["latitude"].toDouble();
       double longitude = rackObj["longitude"].toDouble();

       if (minLatitude > latitude || minLatitude == -999)
           minLatitude = latitude;
       if (maxLatitude <= latitude)
           maxLatitude = latitude;

       if (minLongitude > longitude || minLongitude == -999)
           minLongitude = longitude;
       if (maxLongitude <= longitude)
           maxLongitude = longitude;
   }

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
