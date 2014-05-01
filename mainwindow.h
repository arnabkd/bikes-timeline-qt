#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QString;
class QPushButton;
class BikeRackSystem;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    /* Buttons */
    QPushButton * browseButton;
    QPushButton * nextButton;
    QPushButton * previousButton;

    /* Status label */
    QLabel * statusText;

    /* BikeRackSystem object that contains all rack information */
    BikeRackSystem * system;

    void setupButtons();
    void setupBikeRackSystem();


private slots:
    void browseButtonPushed();
    void nextButtonPushed();
    void previousButtonPushed();

signals:
    void nextStatus();
    void previousStatus();
    void statusUpdate(QString);
};

#endif // MAINWINDOW_H
