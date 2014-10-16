#ifndef GCSCOORDINATES_H
#define GCSCOORDINATES_H
#define _USE_MATH_DEFINES

#include <QObject>
#include <QVector>
#include <QDebug>
#include <cmath>

#include "ellipsoid.h"

class GCSCoordinates : public QObject
{
    Q_OBJECT

public:
    explicit GCSCoordinates(QObject *parent = 0);
    void setLatitude(double DD, double MM, double SS);
    void setLatitude(double decLatitude);
    void setLongitude(double DD, double MM, double SS);
    void setLongitude(double decLongitude);
    void setWestern(bool g);
    void setEllipsoid(QString ellipsoidName);
    double sex2dec(double dd, double mm, double ss);
    QVector<double> dec2sex(double degrees);
    QVector<double> toUTM();

signals:

public slots:

private:
    QVector<double> latitude;
    QVector<double> longitude;
    double lat;
    double lon;
    bool decimalNotation;
    bool western;
    Ellipsoid ellipsoid;
};

#endif // GCSCOORDINATES_H
