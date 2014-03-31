#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "bikerack.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsView;
class QLineEdit;
class QString;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    QGraphicsScene         * scene;
    QGraphicsView          * view;
    QHash <int, BikeRack *>  racks;
    QLineEdit              * rackID;
    QLineEdit              * R;
    QLineEdit              * G;
    QLineEdit              * B;
    QPushButton            * change;

private slots:
    void changeLum(int R, int G, int B, int id);
};

#endif // MAINWINDOW_H
