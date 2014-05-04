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

/*!
 * \brief BikeRackSystem::setCurrentIndex
 * Sets the current index and updates the display
 * \param index
 */
void BikeRackSystem::setCurrentIndex(int index)
{
    currentStatusIndex = index;
    updateDisplay();
}

/*!
 * \brief BikeRackSystem::nextStatus
 * Checks if there is a next index to be displayed. If there is, sets the current
 * index to be the next one.
 */
void BikeRackSystem::nextStatus()
{
    if ((currentStatusIndex+1) < timeline.size())
        setCurrentIndex(currentStatusIndex + 1);

}

/*!
 * \brief BikeRackSystem::previousStatus
 * Checks if there is a previous index to be displayed. If there is, sets the current
 * index to be the previous one.
 */
void BikeRackSystem::previousStatus()
{
    if ((currentStatusIndex-1) >= 0)
        setCurrentIndex(currentStatusIndex - 1);

}

/*!
 * \brief BikeRackSystem::loadDataSet
 * Clears the existing objects from the scene, assign a datasetloader to a
 * thread and start the thread.
 */
void BikeRackSystem::loadDataSet()
{
    foreach (BikeRack *rack , bikeracks) {
        scene->removeItem(rack);
    }
    timeline.clear();
    bikeracks.clear();

    datasetLoader = new DataSetLoader(dataFolder);
    connect(datasetLoader, SIGNAL(datasetLoaded()), this, SLOT(dataObjectsRead()));
    connect(datasetLoader, SIGNAL(loadingStatus(QString)), this, SIGNAL(message(QString)));

    QThread *thread = new QThread();
    connect(thread, SIGNAL(started()), datasetLoader, SLOT(load()));
    datasetLoader->moveToThread(thread);

    thread->start();
}

/*!
 * \brief BikeRackSystem::dataObjectsRead
 * Called when the dataObjects from the datasetLoader are ready to be stored in this object.
 */
void BikeRackSystem::dataObjectsRead()
{
    /* Fail-safe, just in case this was called without the datasetLoader being initialized.
       In that case, call loadDataSet()
    */
    if (!datasetLoader)
    {
        loadDataSet();
        return;
    }

    /* Copy the data objects from the loader */
    this->bikeracks = datasetLoader->getBikeRacks();
    this->timeline = datasetLoader->getTimeline();

    this->minLatitude = datasetLoader->getMinLatitude();
    this->maxLatitude = datasetLoader->getMaxLatitude();
    this->minLongitude = datasetLoader->getMinLongitude();
    this->maxLongitude = datasetLoader->getMaxLongitude();

    /* Add the racks to the scene and set the current index to be at the start of the dataset */
    addRacksToScene();
    setCurrentIndex(0);
    emit datasetLoaded();
}

/*!
 * \brief BikeRackSystem::updateDisplay
 * Updates the display to show the current status (as given by the currentStatusIndex variable).
 */
void BikeRackSystem::updateDisplay()
{
    if (currentStatusIndex < 0 || currentStatusIndex >= timeline.size())
    {
        qWarning("Reached illegal status index");
        return;
    }

    RackStatus * currentStatus = timeline[currentStatusIndex];

    emit message(currentStatus->getCityName() + " bike racks status at " + getDateStr(currentStatus->getTime()));
    QHash<int,int> status = currentStatus->getStatus();

    /* When we reach the end of the dataset, let the GUI know so the controls can react accordingly. */
    if (currentStatusIndex == (timeline.size() -1))
    {
        emit endOfDataset();
    }

    foreach (int rackID, status.keys()) {
        /*
           Fail-safe against dummy data in data files.
           Verify that this rack ID was in the original racks.json file.
           Sometimes status files are contaminated with dummy objects used by bikeshare
           companies for testing purposes
        */
        if (bikeracks.contains(rackID))
            bikeracks[rackID]->updateNumBikes(status[rackID]);
    }
}

