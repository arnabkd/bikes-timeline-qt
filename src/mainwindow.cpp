/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "bikeracksystem.h"
#include <QFileDialog>
#include <QAction>
#include <QToolBar>
#include <QStyle>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Bike share visualization");
    qreal width = 800;
    qreal height = 600;

    createActions();
    createMenu();
    createToolBar();
    createStatusBar();
    createBikeRackSystem(width, height);

    setMinimumSize(width, height);
    showFullScreen();

    setBackgroundRole(QPalette::Shadow);

    connect(this, SIGNAL(statusUpdate(QString)), this, SLOT(setStatus(QString)));

}

/*!
 * \brief MainWindow::createActions
 * Creates the actions:
 * - browseAction
 * - nextAction
 * - previousAction
 * - playPauseAction
 * - stopAction
 *
 * All except browseAction are disabled at start.
 */
void MainWindow::createActions()
{
    browseAction = new QAction(tr("Set data folder"), this);
    browseAction->setShortcuts(QKeySequence::Open);
    browseAction->setStatusTip(tr("Set data folder"));
    connect(browseAction, SIGNAL(triggered()), this, SLOT(browse()));

    nextAction = new QAction(tr("Next status"), this);
    nextAction->setShortcuts(QKeySequence::MoveToNextChar);
    nextAction->setStatusTip(tr("See the next rack status"));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));

    previousAction = new QAction(tr("Previous status"), this);
    previousAction->setShortcuts(QKeySequence::MoveToPreviousChar);
    previousAction->setStatusTip(tr("See the previous rack status"));
    connect(previousAction, SIGNAL(triggered()), this, SLOT(previous()));

    playPauseAction = new QAction(tr("Play/pause animation"), this);
    QList<QKeySequence> playPauseSequence;
    playPauseSequence << QKeySequence(Qt::Key_Space);
    playPauseAction->setShortcuts(playPauseSequence);
    playPauseAction->setStatusTip(tr("Play/pause the animation"));
    connect(playPauseAction, SIGNAL(triggered()), this, SLOT(playPause()));

    stopAction = new QAction(tr("Stop animation"), this);
    stopAction->setShortcuts(QKeySequence::Save);
    stopAction->setStatusTip(tr("Stop the animation"));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

    aboutAction = new QAction(tr("About Qt"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutQt()));

    setControlsEnabled(false);
}

/*!
 * \brief MainWindow::createToolBar
 * Creates toolbar and adds all actions.
 */
void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("Toolbar"));
    toolBar->addAction(browseAction);
    browseAction->setIcon(style()->standardIcon
                          (QStyle::SP_DirOpenIcon));

    toolBar->addAction(previousAction);
    previousAction->setIcon(style()->standardIcon
                            (QStyle::SP_MediaSeekBackward));

    toolBar->addAction(playPauseAction);
    playPauseAction->setIcon(style()->standardIcon
                             (QStyle::SP_MediaPlay));

    toolBar->addAction(stopAction);
    stopAction->setIcon(style()->standardIcon
                        (QStyle::SP_MediaStop));

    toolBar->addAction(nextAction);
    nextAction->setIcon(style()->standardIcon
                        (QStyle::SP_MediaSeekForward));
}

/*!
 * \brief MainWindow::createMenu
 * Creates a menu and adds the actions to it.
 */
void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(browseAction);
    fileMenu->addAction(nextAction);
    fileMenu->addAction(previousAction);
    fileMenu->addAction(playPauseAction);
    fileMenu->addAction(stopAction);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAction);
}

/*!
 * \brief MainWindow::createStatusBar
 */
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

/*!
 * \brief MainWindow::createBikeRackSystem
 * Creates a BikeRackSystem object and sets up connections.
 * \param width
 * \param height
 */
void MainWindow::createBikeRackSystem(int width, int height)
{
    system = new BikeRackSystem(height, width, this);
    system->setGeometry(0,0,width, height);

    /* Setup connections for navigation */
    connect(this, SIGNAL(nextStatus()), system, SLOT(nextStatus()));
    connect(this, SIGNAL(previousStatus()), system, SLOT(previousStatus()));

    /* Setup connections for the bikerack system to communicate back to the GUI */
    connect(system, SIGNAL(message(QString)), this, SLOT(setStatus(QString)));
    connect(system, SIGNAL(endOfDataset()), this, SLOT(stop()));
    connect(system, SIGNAL(datasetLoaded()), this, SLOT(dataSetLoaded()));

    /* Set the bikerack system to be the central widget */
    setCentralWidget(system);
}

/*!
 * \brief MainWindow::setControlsEnabled
 * Enables or disables controls.
 * \param enable
 */
void MainWindow::setControlsEnabled(bool enable)
{
    nextAction->setEnabled(enable);
    previousAction->setEnabled(enable);
    playPauseAction->setEnabled(enable);
    stopAction->setEnabled(enable);
}

/*!
 * \brief MainWindow::dataSetLoaded
 * Enables the controls after the dataset has been loaded.
 */
void MainWindow::dataSetLoaded()
{
    setControlsEnabled(true);
}


/*!
 * \brief MainWindow::browse
 *  The browse action calls this slot. Prompts the user to choose a dataset folder containing a racks.json file
 */
void MainWindow::browse()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory containing a racks.json file"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (system == NULL)
    {
        return;
    }

    if (!system->setDataFolder(dir + "/"))
    {
        QMessageBox::warning(this,"Error", "Could not find a racks.json file in that folder. Check that you opened the correct directory");
        return;
    }


    stop();
    system->loadDataSet();
    setControlsEnabled(false);

}

/*!
 * \brief MainWindow::setStatus
 *  Sets the text for statusbar.
 * \param message
 */
void MainWindow::setStatus(QString message)
{
    statusBar()->showMessage(message);
}


/*!
 * \brief MainWindow::playPause
 *  Play/pause slot. Plays/pauses the animation.
 */
void MainWindow::playPause()
{
    if (timer)
    {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(next()));
        timer = NULL;
        playPauseAction->setIcon(style()->standardIcon
                                 (QStyle::SP_MediaPlay));
        return;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(next()));
    timer->start(25);

    playPauseAction->setIcon(style()->standardIcon
                             (QStyle::SP_MediaPause));
}

/*!
 * \brief MainWindow::stop
 * Stops the animation.
 */
void MainWindow::stop()
{
    if (!timer)
    {
        return;
    }
    else
    {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(next()));
        timer = NULL;
    }
    system->setCurrentIndex(0);
    playPauseAction->setIcon(style()->standardIcon
                             (QStyle::SP_MediaPlay));

}

/*!
 * \brief MainWindow::aboutQt
 * Calls QApplication::aboutQt()
 */
void MainWindow::aboutQt()
{
    QApplication::aboutQt();
}

/*!
 * \brief MainWindow::next
 * Emits a nextStatus() signal (intended for the BikeRackSystem object)
 */
void MainWindow::next()
{
    emit nextStatus();
}

/*!
 * \brief MainWindow::previous
 * Emits a previousStatus() signal (intended for the BikeRackSystem object)
 */
void MainWindow::previous()
{
    emit previousStatus();
}
