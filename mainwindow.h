#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QString;
class QPushButton;
class BikeRackSystem;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    /* Buttons */
    QPushButton            * browseButton;
    QPushButton            * nextButton;
    QPushButton            * previousButton;

    /* Status label */
    QLabel                 * statusText;

    /* BikeRackSystem object that contains all rack information */
    BikeRackSystem         * system;


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
