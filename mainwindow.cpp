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

#include <stdexcept>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

const int MIN_UTM = 3;
const int MIN_GEO = 2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set window icon in the title bar
    QIcon winIcon("://icons/16x16/globe-icon.png");
    setWindowIcon(winIcon);

    // Set the application icon
    QIcon appIcon("://icons/48x48/globe-icon.png");
    QApplication::setWindowIcon(appIcon);

    //this->adjustSize();

    // By creating an action group the items become auto exclusive, only one can be checked
    datumGroup = new QActionGroup(this);
    datumGroup->addAction(ui->actionHayford);
    datumGroup->addAction(ui->actionWGS84);

    delimiterGroup = new QActionGroup(this);
    delimiterGroup->addAction(ui->actionComma);
    delimiterGroup->addAction(ui->actionSpace);
    delimiterGroup->addAction(ui->actionTab);

    connect(ui->actionComma,       SIGNAL(triggered(bool)), SLOT(setDelimiter()));
    connect(ui->actionTab,         SIGNAL(triggered(bool)), SLOT(setDelimiter()));
    connect(ui->actionSpace,       SIGNAL(triggered(bool)), SLOT(setDelimiter()));
    connect(ui->actionAbout,       SIGNAL(triggered()),     SLOT(about()));
    connect(ui->actionHelp,        SIGNAL(triggered()),     SLOT(help()));
    connect(ui->actionHayford,     SIGNAL(triggered()),     SLOT(updateStatusBar()));
    connect(ui->actionWGS84,       SIGNAL(triggered()),     SLOT(updateStatusBar()));
    connect(ui->actionOpenFile,    SIGNAL(triggered()),     SLOT(openCsvFile()));
    connect(ui->actionSaveResults, SIGNAL(triggered()),     SLOT(saveCsvFile()));
    connect(ui->actionConvert,     SIGNAL(triggered()),     SLOT(conversion()));
    connect(ui->actionClear,       SIGNAL(triggered()),     SLOT(clear()));
    connect(ui->convert,           SIGNAL(pressed()),       SLOT(conversion()));
    connect(ui->clearButton,       SIGNAL(pressed()),     SLOT(clear()));
    connect(ui->toUtm,             SIGNAL(toggled(bool)),   SLOT(hideAndShow()));
    connect(ui->ddmmss,            SIGNAL(toggled(bool)),   SLOT(hideAndShow()));

    ui->actionWGS84->setChecked(true);
    ui->actionComma->setChecked(true);

    // Initialize the state of the checkboxes
    hideAndShow();

    delimiter = ',';
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    // Showa the about dialog
    AboutDialog aboutDlg;
    aboutDlg.exec();
}

void MainWindow::help()
{
    // Shows the help dialog
    HelpDialog helpDlg;
    helpDlg.exec();
}

void MainWindow::conversion()
{
    QString content = ui->inputText->toPlainText();
    QStringList inputText = content.split('\n');

    QString result;

    // From LonLat to UTM Conversion
    if (ui->toUtm->isChecked()) {
        if (ui->ddmmss->isChecked()) {
            // Convert from LonLat to UTM (input uses DDMMSS format)
            // IMPORTANT: This just performs a simple conversion from DDMMSS to DECIMAL, NOT TO UTM!
            result = dms2dec(inputText);
            ui->outputText->setPlainText(result);
        } else {
            // Convert from LatLon to UTM (input uses decimal format)
            result = gcs2utm(inputText);
            ui->outputText->setPlainText(result);
        }
    } else {
        if (ui->ddmmss->isChecked()) {
            // ********** THIS SHOULD NEVER BE REACHED **********
            // In case this is reached, perform a simple conversion from DDMMSS to DECIMAL
            // Convert from LatLon (input uses DDMMSS format) to LatLon (output in decimal format)
            result = dms2dec(inputText);
            ui->outputText->setPlainText(result);
        } else {
            // Convert from UTM to LatLon (output in decimal format)
            result = utm2gcs(inputText);
            ui->outputText->setPlainText(result);
        }
    }
}

bool MainWindow::openCsvFile()
{
    QString fileName;
    QString text;
    QList<QStringList> data;

    fileName = QFileDialog::getOpenFileName(this, tr("Open CSV File"), QDir::homePath(), tr("CSV Files (*.csv);;Text delimited files (*.txt);;All files (*.*)"));
    qDebug() << "Opening file:" << fileName;

    // Check file actually exists
    QFileInfo testFile(fileName);
    if (!testFile.exists())
        return false;

    // Read points
    data = CSV::parseFromFile(fileName, "UTF-8");
    qDebug() << "Data length: " << data.length();

    // Get the text from file
    for (int i=0; i < data.length(); i++) {
        QStringList line = data.at(i);
        QString textLine = line.join(delimiter);
        text.append(textLine);
        text.append("\n");

        qDebug() << " Line " << i << ": " << textLine;
    }

    // Set the text to the input widget
    ui->inputText->clear();
    ui->inputText->setPlainText(text);
    return true;
}

bool MainWindow::saveCsvFile()
{
    QString fileName;
    QStringList text;
    QList<QStringList> data;
//    int replace = true;

    fileName = QFileDialog::getSaveFileName(this, tr("Save CSV file"), QDir::homePath(), tr("CSV Files (*.csv);;Text delimited files (*.txt)"));
    qDebug() << "Saving file:" << fileName;

//    // Check file actually exists
//    QFileInfo testFile(fileName);
//    if (testFile.exists()) {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle(tr("Document already exists"));
//        msgBox.setText(tr("The document already exists. Do you want to replace it?"));
//        msgBox.setInformativeText(tr("A file or directory already exists in this location. Replacing it will overwrite its current content."));
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
//        msgBox.setDefaultButton(QMessageBox::Save);
//        replace = msgBox.exec();
//    }

//    if (replace == false) {
//        qDebug() << "Exiting without replacing file: " << fileName;
//        return false;
//    }

    // Creating data
    text = ui->outputText->toPlainText().split("\n");
    qDebug() << "Lines:" << text.length();

    for (int i=0; i < text.length(); i++) {
        if (!text.at(i).isEmpty()) {
            QStringList elements;
            qDebug() << "Line " << i << " :" << text.at(i);
            elements = text.at(i).split(delimiter);
            data.append(elements);
        }
    }

    // Write CSV file
    CSV::write(data, fileName, "UTF-8");

    return true;
}

QString MainWindow::utm2gcs(QStringList inputText) const
{
    /*******************************************************************************************
     *                                                                                         *
     * Converts coordinates from Universal Transverse Mercator to Geographic Coordinate System *
     *                                                                                         *
     *******************************************************************************************/

    QString result;
    QString textError;
    QString lineNumber;
    QPointF gcs;
    int errorCount{0};

    int i{1};

    foreach (QString line, inputText) {
        // Check the line
        if (!line.isEmpty()) {

            QString gcs_line;
            QStringList lineCoordinates;

            lineCoordinates = line.split(delimiter);
            lineNumber.setNum(i);

            // Check the delimiter and number of data
            if (lineCoordinates.size() < MIN_UTM) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Input error"));
                msgBox.setText(tr("Not enough data, each line should have 3 numbers. Maybe wrong delimiter?"));
                msgBox.setInformativeText(tr("The conversion cannot be performed with the input provided. Maybe the coordinates are in the wrong format or the selected delimiter is wrong."));
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                return result;
            } else if (lineCoordinates.size() > MIN_UTM) {
                QString dataNumber, minData;
                dataNumber.setNum(lineCoordinates.size());
                minData.setNum(MIN_UTM);
                errorCount++;
                textError.append("Line " + lineNumber + " has more than " + minData + " data (" + dataNumber + "), remaining data will be ignored!\n");
            }

            // Check the conversion from string to double
            bool ok_x;
            bool ok_y;
            bool ok_z;

            double coord_x = lineCoordinates.at(0).toDouble(&ok_x);
            double coord_y = lineCoordinates.at(1).toDouble(&ok_y);
            double utm_zone = lineCoordinates.at(2).toInt(&ok_z);

            if (!ok_x || !ok_y || !ok_z) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Input error"));
                msgBox.setText(tr("An error ocurred during conversion. Maybe empty or non-numeric data?"));
                msgBox.setInformativeText(tr("The conversion cannot be performed with the input provided. Maybe the data provided is non-numeric or the selected delimiter is wrong."));
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                return result;
            }

            // Allright, continue with the conversion
            UTMCoordinates utm;

            // Set ellipsoid
            if (ui->actionHayford->isChecked())
                utm.setEllipsoid(QString("hayford"));
            else if (ui->actionWGS84->isChecked())
                utm.setEllipsoid(QString("wgs84"));

            // Set coordinates
            utm.setXY(coord_x, coord_y);
            utm.setZone(utm_zone);

            // Set the selected hemisphere
            if (ui->useSouthernHemisphere->isChecked())
                utm.setHemisphere(false); // Use Southern hemisphere
            else
                utm.setHemisphere(true);  // Use Northern hemisphere

            // Perform conversion to GCS
            gcs = utm.toGCS();

            gcs_line = QString::number(gcs.x(), 'f', 8) + ", " + QString::number(gcs.y(), 'f', 8);
            result.append(gcs_line + "\n");
            i++;
        } else {
            qDebug() << "Error in line " << i << ": " << line;
        }
    }

    if (errorCount > 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("More data than required"));
        msgBox.setText(tr("To perform this conversion, each line requires only 3 numbers. The input has more!"));
        msgBox.setInformativeText(tr("The input data after this number will be ignored during conversion."));
        msgBox.setDetailedText(textError);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    return result;
}

QString MainWindow::gcs2utm(QStringList inputText) const
{
    /*******************************************************************************************
     *                                                                                         *
     * Converts coordinates from Geographic Coordinate System to Universal Transverse Mercator *
     *                                                                                         *
     *******************************************************************************************/

    QString result;
    QVector<double> utm;
    QString lineNumber;
    QString textError;
    int errorCount{0};

    int i{1};

    foreach (QString line, inputText) {

        // Check the line
        if (!line.isEmpty()) {

            QString utm_line;
            QString hemisphere;
            QStringList lineCoordinates;

            lineCoordinates = line.split(delimiter);
            lineNumber.setNum(i);

            // Check the delimiter and number of data
            if (lineCoordinates.size() < MIN_GEO) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Input error"));
                msgBox.setText(tr("Not enough data, each line should have 2 numbers. Maybe wrong delimiter?"));
                msgBox.setInformativeText(tr("The conversion cannot be performed with the input provided. Maybe the coordinates are in the wrong format or the selected delimiter is wrong."));
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                return result;
            } else if (lineCoordinates.size() > MIN_GEO) {
                QString dataNumber, minData;
                dataNumber.setNum(lineCoordinates.size());
                minData.setNum(MIN_GEO);

                textError.append("Line " + lineNumber + " has more than " + minData + " data (" + dataNumber + "), remaining data will be ignored!\n");

                errorCount++;
            }

            // Check the conversion from string to double
            bool ok_x;
            bool ok_y;

            double coord_x = lineCoordinates.at(0).toDouble(&ok_x);
            double coord_y = lineCoordinates.at(1).toDouble(&ok_y);

            if (!ok_x || !ok_y) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Input error"));
                msgBox.setText(tr("An error ocurred during conversion. Maybe empty or non-numeric data?"));
                msgBox.setInformativeText(tr("The conversion cannot be performed with the input provided. Maybe the data provided is non-numeric or the selected delimiter is wrong."));
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                return result;
            }

            // Allright, continue with the conversion
            GCSCoordinates gcs;
            gcs.setLongitude(coord_x);
            gcs.setLatitude(coord_y);

            // Set the hemisphere from the latitude (y-axis coordinate)
            if (coord_y < 0)
                hemisphere = "S";
            else
                hemisphere = "N";

            // TODO: What happens if X = 0? And if Y = 0?
            if (coord_x <= 0)
                gcs.setWestern(true);
            else
                gcs.setWestern(false);

            // Set ellipsoid
            if (ui->actionHayford->isChecked())
                gcs.setEllipsoid(QString("hayford"));
            else if (ui->actionWGS84->isChecked())
                gcs.setEllipsoid(QString("wgs84"));

            // Perform conversion to UTM
            utm = gcs.toUTM();

            // Get converted coordinates
            utm_line = QString::number(utm.at(0), 'f', 6) + ", " + QString::number(utm.at(1), 'f', 6) + ", " + QString::number(utm.at(2)) + ", " + hemisphere;

            result.append(utm_line + "\n");
            i++;
        } else {
            qDebug() << "Error in line " << i << ": " << line;
        }
    }

    if (errorCount > 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("More data than required"));
        msgBox.setText(tr("To perform this conversion, each line requires only 2 numbers. The input has more!"));
        msgBox.setInformativeText(tr("The remaining data after this number will be ignored during conversion."));
        msgBox.setDetailedText(textError);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    return result;
}

QString MainWindow::dms2dec(QStringList inputText) const
{
    QString result;

    int i{1};
    foreach (QString line, inputText) {

        if (!line.isEmpty()) {
            qDebug() << "Line " << i << ": " << line;
            double x;
            double y;
            QString decLine;
            QString coord_x = line.split(delimiter).at(0);
            QString coord_y = line.split(delimiter).at(1);

            qDebug() << "Coord X:" << coord_x << " Coord Y:" << coord_y;

            x = sex2dec(coord_x.trimmed());
            y = sex2dec(coord_y.trimmed());

            decLine = QString::number(x, 'f', 6) + ", " + QString::number(y, 'f', 6);
            result.append(decLine + "\n");
            i++;
        } else {
            qDebug() << "Error in line " << i << ": " << line;
        }
    }
    return result;
}

//QVector<double> MainWindow::coordFormatter(QString rawCoordinates)
//{
//    /**********************************************************************************
//     *                                                                                *
//     * Format the coordinates from a string (space separated) to a vector of doubles. *
//     *                                                                                *
//     **********************************************************************************/

//    QVector<double> coord;
//    QStringList formatedCoordinate;

//    formatedCoordinate = rawCoordinates.split(" ");

//    //qDebug() << "Formmating: " << formatedCoordinate;

//    // Check for decimal or sexagesimal notation
//    if (formatedCoordinate.size() == 1) {

//        // Decimal, should be one item in the array for DD only
//        coord.clear();
//        coord.append(formatedCoordinate.at(0).toDouble());

//    } else if (formatedCoordinate.size() == 2) {

//        // Sexagesimal, should be three items in the array when it has DDMM only
//        coord.clear();
//        for (int i = 0; i < formatedCoordinate.size(); i++) {
//            coord.append(formatedCoordinate.at(i).toDouble());
//        }

//    } else if (formatedCoordinate.size() == 3) {

//        // Sexagesimal, should be three items in the array when it has DDMMSS
//        coord.clear();
//        for (int i = 0; i < formatedCoordinate.size(); i++) {
//            coord.append(formatedCoordinate.at(i).toDouble());
//        }

//    } else {

//        qDebug() << "Coordinates have a wrong format!";
//        coord.clear();

//    }

//    return coord;
//}

QString MainWindow::formatMessyCoordinates(QString rawCoordinates) const
{
    /*****************************************************************************
     *                                                                           *
     * Format the coordinates to a string format that can be handled more easily *
     *                                                                           *
     *****************************************************************************/

    QString formattedCoordinates;

    // Replace and remove some stuff
    // WARNING! Should not delete commas and tabs, they could be used as text delimiters
    rawCoordinates.replace("`", "\'");
    rawCoordinates.replace(QChar(148), ' ');

//    rawCoordinates.replace(QChar::fromLatin1('”'), ' ');
//    rawCoordinates.replace(QChat::fromLatin1('´'), '\'');
//    rawCoordinates.replace(QString::fromLatin1('°'), ' ');
//    rawCoordinates.replace(QString::fromLatin1('º'), ' ');

    rawCoordinates.replace(QString::fromLatin1("”"), " ");
    rawCoordinates.replace(QString::fromLatin1("´"), "\'");
    rawCoordinates.replace(QString::fromLatin1("°"), " ");
    rawCoordinates.replace(QString::fromLatin1("º"), " ");
    rawCoordinates.replace('_', ' ');
    rawCoordinates.replace("\"", " ");
    rawCoordinates.replace("\'", " ");
    //rawCoordinates.replace(",", " ");
    rawCoordinates.replace("long.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("lon.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("lat.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("n.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("o.", " ", Qt::CaseInsensitive);
    //rawCoordinates.replace(QRegExp("[\s]{2}"), " ");

    // Remove double spaces
    for (int times = 0; times < 30; times++) {
        rawCoordinates.replace(' ', " ");
        rawCoordinates.replace("  ", " ");
    }

    rawCoordinates.remove(QRegExp("[a-zA-Z]"));
    rawCoordinates.remove(QRegExp(QString::fromUtf8("[~!@#$%^&*()_—+=|:;<>«»?/{}]")));
    //rawCoordinates.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
    formattedCoordinates = rawCoordinates.trimmed();
    return formattedCoordinates;
}


double MainWindow::sex2dec(QString sexagesimal) const
{
    double dd{0}, mm{0}, ss{0};
    double degrees{-1};
    QString coord;
    QStringList coordinates;

    if (sexagesimal.isEmpty())
        return degrees;

    coord = formatMessyCoordinates(sexagesimal);
    coordinates = coord.split(" ");

    switch (coordinates.length()) {
    case 0:
        break;
    case 1:
    {
        dd = coordinates.at(0).toDouble();
        mm = 0;
        ss = 0;
    }
        break;
    case 2:
    {
        dd = coordinates.at(0).toDouble();
        mm = coordinates.at(1).toDouble();
        ss = 0;
    }
        break;
    case 3:
    {
        dd = coordinates.at(0).toDouble();
        mm = coordinates.at(1).toDouble();
        ss = coordinates.at(2).toDouble();
    }
        break;
    default:
        // Should never be reached
        break;
    }

    if (dd > 0)
        degrees = dd + (mm / 60.0) + (ss / 3600.0);
    else
        degrees = dd - (mm / 60.0) - (ss / 3600.0);

    qDebug() << dd << "°" << mm << "'" << ss << "\'\' = " << degrees;

    return degrees;
}

double MainWindow::sex2dec(double dd, double mm, double ss) const
{
    double degrees;

    if (dd > 0)
        degrees = dd + (mm / 60.0) + (ss / 3600.0);
    else
        degrees = dd - (mm / 60.0) - (ss / 3600.0);

    qDebug() << dd << "°" << mm << "'" << ss << "\'\' = " << degrees;

    return degrees;
}

QChar MainWindow::setDelimiter()
{
    //QString delimiter;
    if (ui->actionComma->isChecked()) {
        delimiter = ',';
        qDebug() << "  Selected delimiter is comma.";
    } else if (ui->actionTab->isChecked()) {
        delimiter = '\t';
        qDebug() << "  Selected delimiter is tabulator.";
    }

    return delimiter;
}

void MainWindow::updateStatusBar()
{
    QString datum;

    if (ui->actionHayford->isChecked())
        datum = ui->actionHayford->text();
    else if (ui->actionWGS84->isChecked())
        datum = ui->actionWGS84->text();

    datum.remove(QChar('&'));
    ui->statusBar->showMessage(datum);
}

void MainWindow::hideAndShow()
{
    if (ui->toLatLon->isChecked()) {

        // Input in UTM and output in Lon Lat

        ui->label_2->setText(tr("Input (UTM):"));
        ui->label_3->setText(tr("Output (Lon Lat):"));
        ui->ddmmss->setChecked(false);
        ui->useSouthernHemisphere->setEnabled(true);
        ui->ddmmss->setEnabled(false);
    } else if (ui->toUtm->isChecked()) {

        // Input in Lon Lat and output in UTM

        ui->label_2->setText(tr("Input (Lon Lat):"));
        ui->label_3->setText(tr("Output (UTM):"));
        ui->useSouthernHemisphere->setChecked(false);
        ui->useSouthernHemisphere->setEnabled(false);
        ui->ddmmss->setEnabled(true);

        if (ui->ddmmss->isChecked()) {
            ui->label_2->setText(tr("Input (Lon Lat DDMMSS):"));
            ui->label_3->setText(tr("Output (Lon Lat DECIMAL):"));
        }
    }
}

void MainWindow::clear()
{
    ui->inputText->clear();
    ui->outputText->clear();

    hideAndShow();
}
