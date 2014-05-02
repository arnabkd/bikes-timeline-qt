#include "bikeracksystem.h"
#include "bikerack.h"
#include "rackstatus.h"
#include "mathutils.h"
#include "datasetloader.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QThread>

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

}

QString BikeRackSystem::getDateStr(qreal epoch)
{
    const QDateTime dt = QDateTime::fromTime_t(epoch);
    const QString textdate = dt.toString( Qt::TextDate );

    return textdate;
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


bool BikeRackSystem::setDataFolder(QString path)
{
    qDebug() << "datafolder set to " << path;
    if(!QFile(path + "racks.json").exists())
    {
        qDebug() << "Couldn't find a racks.json file";
        return false;
    }

    dataFolder = path;

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
    loader = new DataSetLoader(dataFolder);
    connect(loader, SIGNAL(datasetLoaded()), this, SLOT(dataObjectsRead()));

    QThread *thread = new QThread();
    connect(thread, SIGNAL(started()), loader, SLOT(load()));
    loader->moveToThread(thread);

    thread->start();
}

void BikeRackSystem::dataObjectsRead()
{
    if (!loader)
        return;

    this->bikeracks = loader->getBikeRacks();
    this->timeline = loader->getTimeline();

    this->minLatitude = loader->getMinLatitude();
    this->maxLatitude = loader->getMaxLatitude();
    this->minLongitude = loader->getMinLongitude();
    this->maxLongitude = loader->getMaxLongitude();

    addRacksToScene();
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

