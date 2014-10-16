#include "utmcoordinates.h"

UTMCoordinates::UTMCoordinates(QObject *parent) :
    QObject(parent)
{
}

void UTMCoordinates::setXY(double xCoord, double yCoord)
{
    coordinates.setX(xCoord);
    coordinates.setY(yCoord);
}

void UTMCoordinates::setZone(int utmZone)
{
    zone = utmZone;
}
void UTMCoordinates::setHemisphere(bool h)
{
    // North == true, South == false
    hemisphere = h;
}

void UTMCoordinates::setEllipsoid(QString cEllipsoid)
{
    qDebug() << "Setting ellipsoid:" << cEllipsoid;
    ellipsoid.config(cEllipsoid);
}

QPointF UTMCoordinates::toGCS()
{
    /*
     * UTM to GCS conversion
     *
     * Returns a pair of coordinates in GCS after conversion from UTM projection
     *
     */

    double c, e, e2, lambda0;
    double phi, nu, a2, A1, A2, J2, J4, J6, alpha, beta, gamma, Bphi, b2, zeta, xi, mu, sinhxi, deltalambda, tau;
    QPointF gcs;

    // Calculation of ellipsoid parameters

    e = ellipsoid.eccentricity();
    e2 = ellipsoid.secondEccentricity();
    c = ellipsoid.curvatureRadius();

    qDebug() << "Eccentricity: " << e;
    qDebug() << "Second eccentricity: " << e2;
    qDebug() << "Curvature radius: " << c;

    // Modify X coordinate to prevent negative
    coordinates.setX(coordinates.x() - 500000);

    // Modify Y coordinate in case of south hemisphere (false)
    if (!hemisphere)
        coordinates.setY(coordinates.y() - 10000000);

    // Get the meridian where the UTM Zone is
    lambda0 = (6 * zone) - 183;

    // Now Coticchia - Surace equations parameters

    // 0.9996 is the scale factor of UTM projection
    phi = coordinates.y() / (6366197.724 * 0.9996);

    nu = (c / sqrt(1.0 + pow(e2, 2) * pow(cos(phi), 2) )) * 0.9996;
    a2 = coordinates.x() / nu;
    A1 = sin(2 * phi);
    A2 = A1 * pow(cos(phi), 2);
    J2 = phi + (A1 / 2.0);
    J4 = (3 * J2 + A2) / 4.0;
    J6 = (5 * J4 + A2 * pow(cos(phi), 2)) / 3.0;
    alpha = (3.0 / 4.0) * pow(e2, 2);
    beta = (5.0 / 3.0) * pow(alpha, 2);
    gamma = (35.0 / 27.0) * pow(alpha, 3);
    Bphi = 0.9996 * c * (phi - alpha * J2 + beta * J4 - gamma * J6);
    b2 = (coordinates.y() - Bphi) / nu;
    zeta = ((pow(e2, 2) * pow(a2, 2.0)) / 2.0) * pow(cos(phi), 2);
    xi = a2 * (1.0 - (zeta / 3.0));
    mu = b2 * (1.0 - zeta) + phi;
    sinhxi = (exp(xi) - exp(-xi)) / 2.0;
    deltalambda = atan(sinhxi / cos(mu));
    tau = atan(cos(deltalambda) * tan(mu));

    // Final calculations to geographic coordinates system:
    // deltalambda (lon) has to be converted from radians to sexagesimal degrees

    // Longitude
    // constant M_PI is the value if "pi" from cmath
    gcs.setX( (deltalambda / M_PI) * 180.0 + lambda0 );

    // Latitude
    gcs.setY( phi + (1 + pow(e2, 2) * pow(cos(phi), 2) - (3.0 / 2.0) * pow(e2, 2) * sin(phi) * cos(phi) * (tau - phi)) * (tau - phi) );

    // Passing from radians to sexagesimal degrees in decimal notation,
    // only is necessary do it to latitude, longitude is already in degrees
    gcs.setY((gcs.y() / M_PI) * 180.0);

    return gcs;
}
