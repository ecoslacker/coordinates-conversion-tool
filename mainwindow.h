/*
 * Coordinates
 * Conversion between geographic coordinates (LatLon) and UTM projection
 * Copyright (C) 2014-2018 Eduardo Jiménez <ecoslacker@irriapps.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QString>
#include <QStringList>
#include <QActionGroup>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "csv/csv.h"
#include "utmcoordinates.h"
#include "gcscoordinates.h"
#include "aboutdialog.h"
#include "helpdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QActionGroup *datumGroup;
    QActionGroup *delimiterGroup;
    QChar delimiter;

private slots:
    void about();
    void help();
    void conversion();
    bool openCsvFile();
    bool saveCsvFile();
    QString utm2gcs(QStringList inputText) const;
    QString gcs2utm(QStringList inputText) const;
    QString dms2dec(QStringList inputText) const;
//    QVector<double> coordFormatter(QString rawCoordinates);
    QString formatMessyCoordinates(QString rawCoordinates) const;
    double sex2dec(QString sexagesimal) const;
    double sex2dec(double dd, double mm, double ss) const;
    QChar setDelimiter();  // This function has no use
    void updateStatusBar();
    void hideAndShow();
    void clear();

};

#endif // MAINWINDOW_H
