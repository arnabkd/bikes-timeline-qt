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
#include <QGraphicsScene>
#include <QGraphicsView>
#include "bikerack.h"
#include <QHash>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);

    BikeRack * rack1 = new BikeRack(65.6, 45.6, 20, 400,200,10,10);
    BikeRack * rack2 = new BikeRack(63.6, 45.6, 20, 200,200,10,10);

    racks.insert(0,rack1);
    racks.insert(1,rack2);

    /*QBrush * fullBrush = new QBrush(QColor::fromRgb(0,250,154));
    rack1->setBrush(* fullBrush);
    rack1->setBrush(QBrush(QColor::fromRgb(0,255,0)));
    */

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

    change = new QPushButton(this);
    change->setText("Change value");
    change->setGeometry(240,0,120,20);

    scene->addItem(rack1);
    scene->addItem(rack2);
    scene->addWidget(rackID);
    scene->addWidget(R);
    scene->addWidget(G);
    scene->addWidget(B);

    view->showMaximized();
    view->setParent(this);



    setWindowTitle("Bike Racks in Oslo");
}

void MainWindow::changeLum(int R, int G, int B, int id)
{
    racks[id]->changeLum(R,G,B);
}


