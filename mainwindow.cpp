#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Hide the text edit widgets from batch conversion
    ui->geoCoordinates->hide();
    ui->utmCoordinates->hide();
    ui->convertButton->hide();

    this->adjustSize();

    // By creating an action group the items become auto exclusive, only one can be checked
    datumGroup = new QActionGroup(this);
    datumGroup->addAction(ui->actionHayford);
    datumGroup->addAction(ui->actionWGS84);

    connect(ui->toGCS, SIGNAL(clicked()), SLOT(utm2gcs()));
    connect(ui->toUTM, SIGNAL(clicked()), SLOT(gcs2utm()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionHayford, SIGNAL(triggered()), SLOT(updateStatusBar()));
    connect(ui->actionWGS84, SIGNAL(triggered()), SLOT(updateStatusBar()));
    connect(ui->latitudeSexsagecimal, SIGNAL(textChanged(QString)), SLOT(lat2dec(QString)));
    connect(ui->longitudeSexagecimal, SIGNAL(textChanged(QString)), SLOT(lon2dec(QString)));
    connect(ui->actionBatchConversion, SIGNAL(triggered(bool)), SLOT(configureBatchConversion()));
    connect(ui->convertButton, SIGNAL(clicked(bool)), SLOT(batchConversionGCS()));

    ui->actionWGS84->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::utm2gcs()
{
    /*******************************************************************************************
     *                                                                                         *
     * Converts coordinates from Universal Transverse Mercator to Geographic Coordinate System *
     *                                                                                         *
     *******************************************************************************************/

    if (ui->actionBatchConversion->isChecked()) {

        // Perform a batch conversion
        qDebug() << "Not implemented yet.";

    } else {

        // Perform a single coordinates conversion

        if (ui->x->text().toDouble() > 0 && ui->y->text().toDouble() > 0 && ui->zone->text().toDouble() > 0) {
            qDebug() << ">>>>>>> Running utm2gcs <<<<<<<";
            QPointF gcs;

            // Configure UTM coordinates from GUI
            UTMCoordinates utm;

            // Set ellipsoid
            if (ui->actionHayford->isChecked())
                utm.setEllipsoid(QString("hayford"));
            else if (ui->actionWGS84->isChecked())
                utm.setEllipsoid(QString("wgs84"));

            // Set coordinates
            utm.setXY(ui->x->text().toDouble(), ui->y->text().toDouble());
            utm.setZone(ui->zone->text().toInt());
            utm.setHemisphere(ui->northern->isChecked());

            // Perform conversion to GCS
            gcs = utm.toGCS();

            // Set coordinates to GUI
            ui->latitude->setText(QString::number(gcs.y(), 'f', 8));
            ui->longitude->setText(QString::number(gcs.x(), 'f', 8));
        }
    }
}

void MainWindow::gcs2utm()
{
    /*******************************************************************************************
     *                                                                                         *
     * Converts coordinates from Geographic Coordinate System to Universal Transverse Mercator *
     *                                                                                         *
     *******************************************************************************************/


    // Perform a single coordinates conversion

    if (ui->longitude->text().toDouble() != 0 && ui->latitude->text().toDouble() != 0) {
        qDebug() << ">>>>>>> Running gcs2utm <<<<<<<";
        QVector<double> utm;
        QVector<double> lat;
        QVector<double> lon;
        QString textLat;
        QString textLon;

        // Configure GCS coordinates from GUI

        textLat = ui->latitude->text();
        textLon = ui->longitude->text();

        lat = coordFormatter(textLat);
        lon = coordFormatter(textLon);

        GCSCoordinates gcs;
        // Set latitude
        if (lat.size() == 3) {
            gcs.setLatitude(lat.at(0), lat.at(1), lat.at(2));
        } else if (lat.size() == 2) {
            gcs.setLatitude(lat.at(0), lat.at(1), 0);
        } else if (lat.size() == 1) {
            gcs.setLatitude(lat.at(0));
        }

        // Set longitude
        if (lon.size() == 3) {
            gcs.setLongitude(lon.at(0), lon.at(1), lon.at(2));
        } else if (lon.size() == 2) {
            gcs.setLongitude(lon.at(0), lon.at(1), 0);
        } else if (lon.size() == 1) {
            gcs.setLongitude(lon.at(0));
        }

        if (lon.at(0) < 0)
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

        // Set coordinates to GUI
        ui->x->setText(QString::number(utm.at(0), 'f', 6));
        ui->y->setText(QString::number(utm.at(1), 'f', 6));
        ui->zone->setText(QString::number(utm.at(2)));

        // Set hemisphere
        if (lat.at(0) < 0)
            ui->southern->setChecked(true);
        else
            ui->northern->setChecked(true);
    }
}

QVector<double> MainWindow::coordFormatter(QString rawCoordinates)
{
    /**********************************************************************************
     *                                                                                *
     * Format the coordinates from a string (space separated) to a vector of doubles. *
     *                                                                                *
     **********************************************************************************/

    QVector<double> coord;
    QStringList formatedCoordinate;

    formatedCoordinate = rawCoordinates.split(" ");

    //qDebug() << "Formmating: " << formatedCoordinate;

    // Check for decimal or sexagesimal notation
    if (formatedCoordinate.size() == 1) {

        // Decimal, should be one item in the array for DD only
        coord.clear();
        coord.append(formatedCoordinate.at(0).toDouble());

    } else if (formatedCoordinate.size() == 2) {

        // Sexagesimal, should be three items in the array when it has DDMM only
        coord.clear();
        for (int i = 0; i < formatedCoordinate.size(); i++) {
            coord.append(formatedCoordinate.at(i).toDouble());
        }

    } else if (formatedCoordinate.size() == 3) {

        // Sexagesimal, should be three items in the array when it has DDMMSS
        coord.clear();
        for (int i = 0; i < formatedCoordinate.size(); i++) {
            coord.append(formatedCoordinate.at(i).toDouble());
        }

    } else {

        qDebug() << "Coordinates have a wrong format!";
        coord.clear();

    }

    return coord;
}

QString MainWindow::formatMessyCoordinates(QString rawCoordinates)
{
    /*****************************************************************************
     *                                                                           *
     * Format the coordinates to a string format that can be handled more easily *
     *                                                                           *
     *****************************************************************************/

    QString formattedCoordinates;

    // Replace and remove some stuff
    rawCoordinates.replace("`", "\'");
    rawCoordinates.replace(QChar::fromLatin1('´'), '\'');
    rawCoordinates.replace(QChar::fromLatin1('°'), ' ');
    rawCoordinates.replace(QChar::fromLatin1('º'), ' ');
    rawCoordinates.replace(QChar(148), ' ');
    rawCoordinates.replace(QChar::fromLatin1('”'), ' ');
    rawCoordinates.replace('_', ' ');
//    rawCoordinates.replace(QString::fromLatin1("´"), "\'");
//    rawCoordinates.replace(QString::fromLatin1("°"), " ");
//    rawCoordinates.replace(QString::fromLatin1("º"), " ");
    rawCoordinates.replace("\"", " ");
    rawCoordinates.replace("\'", " ");
    rawCoordinates.replace(",", " ");
    rawCoordinates.replace("long.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("lon.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("lat.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("n.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("o.", " ", Qt::CaseInsensitive);
    rawCoordinates.replace("\t", " ", Qt::CaseInsensitive);
    //rawCoordinates.replace(QRegExp("[\s]{2}"), " ");

    // Remove double spaces
    for (int times = 0; times < 30; times++) {
        rawCoordinates.replace(' ', " ");
        rawCoordinates.replace("  ", " ");
    }

    rawCoordinates.remove(QRegExp("[a-zA-Z]"));
    rawCoordinates.remove(QRegExp(QString::fromUtf8("[-~!@#$%^&*()_—+=|:;<>«»?/{}]")));
    //rawCoordinates.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
    formattedCoordinates = rawCoordinates.trimmed();
    return formattedCoordinates;
}

void MainWindow::about()
{
    AboutDialog aboutDlg;
    aboutDlg.exec();
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

void MainWindow::lat2dec(QString sexagesimal)
{
    double lat;
    lat = sex2dec(sexagesimal);

    ui->latitude->setText(QString::number(lat, 'f', 8));
}

void MainWindow::lon2dec(QString sexagesimal)
{
    double lon;
    lon = sex2dec(sexagesimal);

    ui->longitude->setText(QString::number(lon, 'f', 8));
}

double MainWindow::sex2dec(QString sexagesimal)
{
    double dd, mm, ss;
    double degrees;

    dd = sexagesimal.split("°").at(0).toDouble();
    mm = sexagesimal.split("°").at(1).split("'").at(0).toDouble();
    ss = sexagesimal.split("°").at(1).split("'").at(1).toDouble();

    if (dd > 0)
        degrees = dd + (mm / 60.0) + (ss / 3600.0);
    else
        degrees = dd - (mm / 60.0) - (ss / 3600.0);

    qDebug() << dd << "°" << mm << "'" << ss << "\'\' = " << degrees;

    return degrees;
}

double MainWindow::sex2dec(double dd, double mm, double ss)
{
    double degrees;

    if (dd > 0)
        degrees = dd + (mm / 60.0) + (ss / 3600.0);
    else
        degrees = dd - (mm / 60.0) - (ss / 3600.0);

    qDebug() << dd << "°" << mm << "'" << ss << "\'\' = " << degrees;

    return degrees;
}

void MainWindow::configureBatchConversion()
{
    if (ui->actionBatchConversion->isChecked()) {
        ui->toUTM->hide();
        ui->toGCS->hide();
        ui->convertButton->show();

        ui->geoCoordinates->show();
        ui->utmCoordinates->show();

        ui->groupDecimal->hide();
        ui->groupSexagesimal->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->x->hide();
        ui->y->hide();
        ui->northern->hide();
        ui->southern->hide();
        ui->zone->hide();
    } else {
        ui->toGCS->show();
        ui->toUTM->show();
        ui->convertButton->hide();

        ui->geoCoordinates->hide();
        ui->utmCoordinates->hide();

        ui->groupDecimal->show();
        ui->groupSexagesimal->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->label_5->show();
        ui->x->show();
        ui->y->show();
        ui->northern->show();
        ui->southern->show();
        ui->zone->hide();
    }

    this->adjustSize();
}

void MainWindow::batchConversionGCS()
{
    // Perform a batch conversion

    qDebug() << "Performing a batch conversion from GCS to UTM";

    QStringList coordinates;
    QString text;

    // Get the text in the field, and separate each line
    QStringList textLines = ui->geoCoordinates->toPlainText().split("\n");

    if (textLines.isEmpty())
        return;

    // Format each line
    for (int i = 0; i < textLines.length(); i++) {
        qDebug() << "Line:" << i << textLines.at(i);

        // Format the coordinates
        QString formattedCoordinates = formatMessyCoordinates(textLines.at(i));
        QStringList coordinatesList = formattedCoordinates.split(" ");

        qDebug() << "  -Elements:" << coordinatesList.size();

        switch (coordinatesList.size()) {
        case 6: {

            double longitude;
            double latitude;

            // Get latitude
            latitude = sex2dec(coordinatesList.at(0).toDouble(),
                               coordinatesList.at(1).toDouble(),
                               coordinatesList.at(2).toDouble());

            longitude =  sex2dec(coordinatesList.at(3).toDouble(),
                                 coordinatesList.at(4).toDouble(),
                                 coordinatesList.at(5).toDouble());

            formattedCoordinates.clear();
            formattedCoordinates = QString::number(latitude, 'f', 6) +
                    " " + QString::number(longitude, 'f', 6);
        }
            break;
        default:
            break;
        }
        coordinates.append(formattedCoordinates);
    }

    text = coordinates.join('\n');
    ui->utmCoordinates->setText(text);
}
