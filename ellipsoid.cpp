#include "ellipsoid.h"

Ellipsoid::Ellipsoid(QObject *parent) :
    QObject(parent)
{
}

void Ellipsoid::config(QString name)
{
    ellipsoid = name;

    // By default use data from WGS84 ellipsoid
    a = 6378137.0;
    b = 6356752.3142;

    // Then search for the specified ellipsoid by user

    //if (ellipsoid == "hayford") {
    if (ellipsoid == QString("hayford")) {
        a = 6378388.0;
        b = 6356911.94613;
        //} else if (ellipsoid == "wgs84") {
    } else if (ellipsoid == QString("wgs84")) {
        a = 6378137.0;
        b = 6356752.3142;
    }
}

double Ellipsoid::eccentricity()
{
    e = sqrt(pow(a, 2) - pow(b, 2)) / a;
    return e;
}

double Ellipsoid::secondEccentricity()
{
    e_second = sqrt(pow(a, 2) - pow(b, 2)) / b;
    return e_second;
}

double Ellipsoid::curvatureRadius()
{
    c = pow(a, 2) / b;
    return c;
}
