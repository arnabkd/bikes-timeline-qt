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
    QAction * playPauseAction;
    QAction * stopAction;
    QAction * aboutAction;

    /* Menu */
    QMenu * fileMenu;
    QMenu * helpMenu;

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
    void createBikeRackSystem(int width, int height);

    void setControlsEnabled (bool enable);

public slots:
    void dataSetLoaded();

private slots:
    void browse();
    void next();
    void previous();
    void playPause();
    void stop();
    void aboutQt();

    void setStatus(QString message);

signals:
    void nextStatus();
    void previousStatus();
    void statusUpdate(QString);
};

#endif // MAINWINDOW_H
