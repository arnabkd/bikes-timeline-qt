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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Bike Racks in Oslo");
    qreal width = 1024;
    qreal height = 600;

    system = new BikeRackSystem(height, width, this);
    connect(this, SIGNAL(nextStatus()), system, SLOT(nextStatus()));
    connect(this, SIGNAL(previousStatus()), system, SLOT(previousStatus()));

    createActions();
    createMenu();
    createToolBar();
    createStatusBar();

    connect(this, SIGNAL(statusUpdate(QString)), this, SLOT(setStatus(QString)));
    connect(system, SIGNAL(timeString(QString)), this, SLOT(setStatus(QString)));
    setCentralWidget(system);

    setMinimumSize(width, height);
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

    playAction = new QAction(tr("Show animation"), this);
    playAction->setShortcuts(QKeySequence::Print);
    playAction->setStatusTip(tr("Animate the dataset"));
    connect(playAction, SIGNAL(triggered()), this, SLOT(play()));

    pauseAction = new QAction(tr("Stop animation"), this);
    pauseAction->setShortcuts(QKeySequence::Back);
    pauseAction->setStatusTip(tr("Stop the animation"));
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pause()));
}

void MainWindow::createToolBar()
{
}

void MainWindow::createMenu()
{
    menu = menuBar()->addMenu("App menu");
    menu->addAction(browseAction);
    menu->addAction(nextAction);
    menu->addAction(previousAction);
    menu->addAction(playAction);
    menu->addAction(pauseAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

/*
void MainWindow::setupButtons()
{

    browseButton = new QPushButton(this);
    browseButton->setText("Choose data folder");
    browseButton->setGeometry(0,0,200,20);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browseButtonPushed()));

    previousButton = new QPushButton(this);
    previousButton->setText("Previous status");
    previousButton->setGeometry(200,0,200,20);
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previousButtonPushed()));

    nextButton = new QPushButton(this);
    nextButton->setText("Next status");
    nextButton->setGeometry(400,0,200,20);
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextButtonPushed()));

    statusText = new QLabel(this);
    statusText->setStyleSheet("QLabel {border : 1px solid black};");
    statusText->setText("Status bar");
    statusText->setGeometry(610,0, 400, 20);
    statusText->setVisible(true);
    connect (this, SIGNAL(statusUpdate(QString)), statusText, SLOT(setText(QString)));

}
*/

void MainWindow::browse()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (system != NULL)
    {
        if (system->setDataFolder(dir + "/"))
        {
            emit statusUpdate("Successfully loaded racks file.");
        }
        else
        {
            emit statusUpdate("Something went wrong");
        }
    }
}

void MainWindow::previous()
{
    emit previousStatus();
}

void MainWindow::play()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(next()));
    timer->start(25);
}

void MainWindow::pause()
{
    if (timer)
    {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(next()));
    }
}

void MainWindow::setStatus(QString message)
{
    statusBar()->showMessage(message);
}

void MainWindow::next()
{
    emit nextStatus();
}
