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

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Bike Racks in Oslo");
    qreal width = 1024;
    qreal height = 600;

    browseButton = new QPushButton(this);
    browseButton->setText("Choose data folder");
    browseButton->setGeometry(0,0,200,20);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browseButtonPushed()));

    system = new BikeRackSystem(height, width, this);

    previousButton = new QPushButton(this);
    previousButton->setText("Previous status");
    previousButton->setGeometry(200,0,200,20);
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previousButtonPushed()));

    nextButton = new QPushButton(this);
    nextButton->setText("Next status");
    nextButton->setGeometry(400,0,200,20);
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextButtonPushed()));

    statusText = new QLabel(this);
    statusText->setText("Status bar");
    statusText->setGeometry(0, 25, 1000, 20);
    statusText->setVisible(true);
    connect (this, SIGNAL(statusUpdate(QString)), statusText, SLOT(setText(QString)));



    system->setGeometry(0,25, width, height);
    system->setVisible(true);

    /*
    BikeRack * rack1 = new BikeRack(65.6, 45.6, 20, 400,200,10,10);
    BikeRack * rack2 = new BikeRack(63.6, 45.6, 20, 200,200,10,10);

    racks.insert(0,rack1);
    racks.insert(1,rack2);

    QBrush * fullBrush = new QBrush(QColor::fromRgb(0,250,154));
    rack1->setBrush(* fullBrush);
    rack1->setBrush(QBrush(QColor::fromRgb(0,255,0)));


    rackID = new QLineEdit(this);
    rackID->setPlaceholderText("Rack ID here");
    rackID->setGeometry(20,0,100,20);

    R = new QLineEdit(this);
    R->setPlaceholderText("R");
    R->setGeometry(150,0,20,20);

    G = new QLineEdit(this);
    G->setPlaceholderText("G");
    G->setGeometry(180,0,20,20);

    B = new QLineEdit(this);
    B->setPlaceholderText("B");
    B->setGeometry(210,0,20,20);

    changeButton = new QPushButton(this);
    changeButton->setText("Change value");
    changeButton->setGeometry(240,0,120,20);

    scene->addItem(rack1);
    scene->addItem(rack2);
    scene->addWidget(rackID);
    scene->addWidget(R);
    scene->addWidget(G);
    scene->addWidget(B);
    */

}

/*
void MainWindow::buttonPushed()
{
    int red =  R->text().toInt();
    int green =  G->text().toInt();
    int blue = B->text().toInt();

    int id = rackID->text().toInt();

    changeLum(red,green,blue,id);
} */

void MainWindow::browseButtonPushed()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                QDir::homePath(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    qDebug() << dir;

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

void MainWindow::previousButtonPushed()
{
    emit statusUpdate("Previous status button");
    emit previousStatus();
}

void MainWindow::nextButtonPushed()
{
    emit statusUpdate("Next status button");
    emit nextStatus();
}
