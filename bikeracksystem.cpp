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
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    this->height = height;
    this->width = width;

}

/*!
 * \brief BikeRackSystem::getDateStr
 *        Calculate the date string for an epoch.
 *        Format as Day_of_week Month Day_of_month HH:MM:SS YYYY
 * \param epoch
 * \return
 */
QString BikeRackSystem::getDateStr(qreal epoch)
{
    QDateTime dt = QDateTime::fromTime_t(epoch);
    QString textdate = dt.toString( Qt::TextDate );

    return textdate;
}

/*!
 * \brief BikeRackSystem::addRacksToScene
 * Adds the BikeRack objects to the QGraphicsScene.
 */
void BikeRackSystem::addRacksToScene()
{
    foreach (BikeRack * rack, bikeracks.values())
    {
        qreal rackX = MathUtils::getRescaledValue(rack->getLongitude(), minLongitude, maxLongitude, width);
        qreal rackY = MathUtils::getRescaledValue(rack->getLatitude(), minLatitude, maxLatitude, height, true);

        rack->setX(rackX);
        rack->setY(rackY);
        scene->addItem(rack);
    }
}

/*!
 * \brief BikeRackSystem::setDataFolder
 * Tries to set a folder as the datafolder. Verify and return true if the folder
 * contains a racks.json file.
 * \param path
 * \return true if the folder contains a racks.json file, false otherwise
 */
bool BikeRackSystem::setDataFolder(QString path)
{
    if(!QFile(path + "racks.json").exists())
    {
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
    foreach (BikeRack *rack , bikeracks) {
        scene->removeItem(rack);
    }
    timeline.clear();
    bikeracks.clear();

    loader = new DataSetLoader(dataFolder);
    connect(loader, SIGNAL(datasetLoaded()), this, SLOT(dataObjectsRead()));
    connect(loader, SIGNAL(loadingStatus(QString)), this, SIGNAL(message(QString)));

    QThread *thread = new QThread();
    connect(thread, SIGNAL(started()), loader, SLOT(load()));
    loader->moveToThread(thread);

    thread->start();
}

void BikeRackSystem::dataObjectsRead()
{
    if (!loader)
    {
        loadDataSet();
        return;
    }
    this->bikeracks = loader->getBikeRacks();
    this->timeline = loader->getTimeline();

    this->minLatitude = loader->getMinLatitude();
    this->maxLatitude = loader->getMaxLatitude();
    this->minLongitude = loader->getMinLongitude();
    this->maxLongitude = loader->getMaxLongitude();

    addRacksToScene();
    setCurrentIndex(0);
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
        emit message(currentStatus->getCityName()  + " bike racks status at "
                     + getDateStr(currentStatus->getTime()) + " (end of dataset)");
        emit endOfDataset();
    }

    foreach (int rackID, status.keys()) {
        /* Verify that this rack ID was in the original racks.json file.
           Sometimes status files are contaminated with dummy objects used by bikeshare
           companies for testing purposes
        */
        if (bikeracks.contains(rackID))
            bikeracks[rackID]->updateNumBikes(status[rackID]);
    }
}

