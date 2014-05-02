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
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QLabel>
#include <QtWidgets>
#include <QFormLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Bike Racks in Oslo");
    qreal width = 1024;
    qreal height = 600;



    createActions();
    createMenu();
    createToolBar();
    createStatusBar();
    createBikeRackSystem(width, height);


    setCentralWidget(system);

    setMinimumSize(width, height);
    showMaximized();

    setBackgroundRole(QPalette::Shadow);
}

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

    nextAction->setEnabled(false);
    previousAction->setEnabled(false);
    playPauseAction->setEnabled(false);
    stopAction->setEnabled(false);
}

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

void MainWindow::createMenu()
{
    menu = menuBar()->addMenu("App menu");
    menu->addAction(browseAction);
    menu->addAction(nextAction);
    menu->addAction(previousAction);
    menu->addAction(playPauseAction);
    menu->addAction(stopAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createBikeRackSystem(int width, int height)
{
    system = new BikeRackSystem(height, width, this);
    system->setGeometry(0,0,width, height);
    connect(this, SIGNAL(nextStatus()), system, SLOT(nextStatus()));
    connect(this, SIGNAL(previousStatus()), system, SLOT(previousStatus()));
    connect(this, SIGNAL(statusUpdate(QString)), this, SLOT(setStatus(QString)));
    connect(system, SIGNAL(message(QString)), this, SLOT(setStatus(QString)));
    connect(system, SIGNAL(endOfDataset()), this, SLOT(stop()));
    connect(system,SIGNAL(datasetLoaded()), this, SLOT(dataSetLoaded()));
}

void MainWindow::dataSetLoaded()
{
    nextAction->setEnabled(true);
    previousAction->setEnabled(true);
    playPauseAction->setEnabled(true);
    stopAction->setEnabled(true);
}


void MainWindow::browse()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (system != NULL)
    {
        if (!system->setDataFolder(dir + "/"))
        {
            emit statusUpdate("Something went wrong. Check that you opened the correct directory");
        }
    }

    //TODO: Move the data loading to a worker object
    system->loadDataSet();

    /* Load the dataset in a different set so GUI stays responsive
    QThread * thread = new QThread();
    connect (thread, SIGNAL(started()), system, SLOT(loadDataSet()));
    system->moveToThread(thread);
    thread->start();*/
}

void MainWindow::setStatus(QString message)
{
    statusBar()->showMessage(message);
}



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

void MainWindow::stop()
{
   timer->stop();
   disconnect(timer, SIGNAL(timeout()), this, SLOT(next()));
   timer = NULL;

   system->setCurrentIndex(0);
   playPauseAction->setIcon(style()->standardIcon
                       (QStyle::SP_MediaPlay));
   return;
}



void MainWindow::next()
{
    emit nextStatus();
}

void MainWindow::previous()
{
    emit previousStatus();
}
