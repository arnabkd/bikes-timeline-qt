#include "datasetloader.h"
#include "bikerack.h"
#include "rackstatus.h"
#include "mathutils.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

DataSetLoader::DataSetLoader(QString dataFolder)
    : dataFolder(dataFolder)
{
}

/*!
 * \brief DataSetLoader::load
 * Loads the racks file and dataset. Emits a datasetLoaded signal when done.
 */
void DataSetLoader::load()
{
    loadRacks();
    loadDataSet();
    emit datasetLoaded();
}

void DataSetLoader::resetVariables()
{
    minLatitude = 0;
    minLongitude = 0;

    maxLatitude = 0;
    maxLongitude = 0;

    bikeracks.clear();
}
/*!
 * \brief DataSetLoader::loadRacks
 * Loads a racks.json file as BikeRack objects.
 */
void DataSetLoader::loadRacks()
{
    resetVariables();

    QString jsonfile = dataFolder + "racks.json";
    QJsonDocument doc = getJsonContents(jsonfile);
    QJsonObject obj = doc.object();
    QStringList rackIDs = obj.keys();

    for (int i = 0; i < rackIDs.size(); i++)
    {
        QJsonObject rackObj = obj[rackIDs[i]].toObject();
        createBikeRack(rackObj);
    }

    MathUtils::padBorders(5, minLatitude, maxLatitude);
    MathUtils::padBorders(5, minLongitude, maxLongitude);

}

void DataSetLoader::createBikeRack(QJsonObject rackObj)
{
    qreal latitude = rackObj["latitude"].toDouble();
    qreal longitude = rackObj["longitude"].toDouble();

    if (bikeracks.keys().size() == 0)
    {
        minLatitude = latitude;
        maxLatitude = latitude;
        minLongitude = longitude;
        maxLongitude = longitude;

        qDebug() << "start of dataset";
    }
    else
    {
        /* Update the minLatitude, maxLatitude as required */
        if (minLatitude > latitude)
            minLatitude = latitude;
        if (maxLatitude <= latitude)
            maxLatitude = latitude;

        /* Update the minLongitude, maxLongitude as requried */
        if (minLongitude > longitude)
            minLongitude = longitude;
        if (maxLongitude <= longitude)
            maxLongitude = longitude;
    }

    //qDebug() << bikeracks.size() << " racks read."<< "min/max: " << minLatitude << "<->" << maxLatitude << " | " << minLongitude << "<->" << maxLongitude;

    int capacity = rackObj["capacity"].toInt();
    int rackID = rackObj["id"].toInt();
    QString desc = rackObj["description"].toString();
    BikeRack * rack = new BikeRack(longitude, latitude, capacity, desc);
    bikeracks[rackID] = rack;
}

/*!
 * \brief DataSetLoader::getJsonContents
 * Opens a file and returns a QJsonObject with the contents of the file.
 * If the file doesn't exist, returns an empty QJsonObject.
 * \param jsonfile
 * \return
 */
QJsonDocument DataSetLoader::getJsonContents(QString jsonfile)
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

void DataSetLoader::loadDataSet()
{
    /* Add all json files */
    QDir dir(dataFolder);
    QStringList filters;
    filters.append("*.json");
    dir.setNameFilters(filters);

    QStringList statusFiles = dir.entryList();
    statusFiles.removeAll("racks.json");

    qDebug() << statusFiles.size() << " files to be added.";

    int i = 0;
    foreach (QString fileName, statusFiles)
    {
        i++;
        emit loadingStatus("Loading file " + QString::number(i) + " of "
                           + QString::number(statusFiles.size()));

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

}
