#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QString;
class BikeRackSystem;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    /* Actions */
    QAction * browseAction;
    QAction * nextAction;
    QAction * previousAction;
    QAction * playAction;
    QAction * pauseAction;

    /* Menu */
    QMenu * menu;

    /* Toolbar */
    QToolBar * toolBar;

    /* Status label */
    QLabel * statusText;

    /* BikeRackSystem object that contains all rack information */
    BikeRackSystem * system;

    QTimer * timer;

    void createActions();
    void createToolBar();
    void createMenu();
    void createStatusBar();

private slots:
    void browse();
    void next();
    void previous();
    void play();
    void pause();

    void setStatus(QString message);

signals:
    void nextStatus();
    void previousStatus();
    void statusUpdate(QString);
};

#endif // MAINWINDOW_H
