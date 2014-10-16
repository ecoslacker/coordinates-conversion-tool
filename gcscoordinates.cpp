#include "gcscoordinates.h"

GCSCoordinates::GCSCoordinates(QObject *parent) :
    QObject(parent)
{
}

void GCSCoordinates::setLatitude(double DD, double MM, double SS)
{
    latitude.append(DD);
    latitude.append(MM);
    latitude.append(SS);
    decimalNotation = false;
}

void GCSCoordinates::setLatitude(double decLatitude)
{
    lat = decLatitude;
    qDebug() << "Setted latitude:" << lat;
    decimalNotation = true;
}

void GCSCoordinates::setLongitude(double DD, double MM, double SS)
{
    longitude.append(DD);
    longitude.append(MM);
    longitude.append(SS);
    decimalNotation = false;
}

void GCSCoordinates::setLongitude(double decLongitude)
{
    lon = decLongitude;
    qDebug() << "Setted longitude:" << lon;
    decimalNotation = true;
}

void GCSCoordinates::setWestern(bool g)
{
    western = g;
}

void GCSCoordinates::setEllipsoid(QString ellipsoidName)
{
    qDebug() << "Setting ellipsoid:" << ellipsoidName;
    ellipsoid.config(ellipsoidName);
}

double GCSCoordinates::sex2dec(double dd, double mm, double ss)
{
    /*
     *  Convert GCS coordinates from sexagesimal (DDMMSS) to decimal notation
     *
     */

    double degrees;
    if (dd > 0)
        degrees = dd + (mm / 60.0) + (ss / 3600.0);
    else
        degrees = dd - (mm / 60.0) - (ss / 3600.0);

    return degrees;
}

QVector<double> GCSCoordinates::dec2sex(double degrees)
{
    /*
     * Converts GCS coordinates from decimal notation to sexagesimal (DDMMSS)
     *
     */

    double dd;
    double mm;
    double ss;
    QVector<double> ddmmss;

    dd = int(degrees);
    mm = int((degrees - dd) * 60.0);
    ss = ((degrees - dd) * 60.0 - mm) * 60.0;

    ddmmss.append(dd);
    ddmmss.append(mm);
    ddmmss.append(ss);

    return ddmmss;
}


QVector<double> GCSCoordinates::toUTM()
{
    QVector<double> utm;
    double x, y, zone; // Zone should be 'int', but only 'double' type is returned
    double c, e, e2, lambda, phi, lambda0, delta_lambda;
    double A, xi, eta, nu, zeta, A1, A2, J2, J4, J6, alpha, beta, gamma, Bphi;

    // Calculation of ellipsoid parameters
    e = ellipsoid.eccentricity();
    e2 = ellipsoid.secondEccentricity();
    c = ellipsoid.curvatureRadius();

    qDebug() << " Eccentricity: " << e;
    qDebug() << " Second eccentricity: " << e2;
    qDebug() << " Curvature radius: " << c;

    // Convert from sexagesimal to decimal notation if needed
    if (!decimalNotation) {
        lon = sex2dec(longitude.at(0), longitude.at(1), longitude.at(2));
        lat = sex2dec(latitude.at(0), latitude.at(1), latitude.at(2));

        qDebug() << "  Converting lon from:" << longitude.at(0) << longitude.at(1) << longitude.at(2) << "to" << lon;
        qDebug() << "  Converting lat from:" << latitude.at(0) << latitude.at(1) << latitude.at(2) << "to" << lat;
    }

    qDebug() << " Longitude (dec): " << lon;
    qDebug() << " Latitude (dec): " << lat;

    // Convert to radians
    lambda = (lon * M_PI) / 180.0;
    phi = (lat * M_PI) / 180.0;

    qDebug() << " lambda: " << lambda;
    qDebug() << " phi: " << phi;

    if (western) {
        //lon = lon * -1;
        //lambda = lambda * -1;
        qDebug() << " Longitude for western:" << lon;
    }

    zone = int((lon / 6) + 31);
    lambda0 = (zone * 6) - 183;
    delta_lambda = lambda - ((lambda0 * M_PI) / 180.0);

    qDebug() << " zone:" << zone;
    qDebug() << " lambda0:" << lambda0;
    qDebug() << " delta_lambda:" << delta_lambda;

    // Coticchia-Surace equations
    A = cos(phi) * sin(delta_lambda);
    xi = 0.5 * log((1 + A) / (1 - A));
    eta = atan(tan(phi) / cos(delta_lambda)) - phi;
    nu = (c / pow(1 + pow(e2, 2) * pow(cos(phi), 2), 0.5) ) * 0.9996;
    zeta = (pow(e2, 2) / 2) * pow(xi, 2) * pow(cos(phi), 2);
    A1 = sin(2 * phi);
    A2 = A1 * pow(cos(phi), 2);
    J2 = phi + (A1 / 2);
    J4 = (3.0 * J2 + A2) / 4.0;
    J6 = (5 * J4 + A2 * pow(cos(phi), 2)) / 3;
    alpha = (3.0 / 4.0) * pow(e2, 2);
    beta = (5.0 / 3.0) * pow(alpha, 2);
    gamma = (35.0 / 27.0) * pow(alpha, 3);
    Bphi = 0.9996 * c * (phi - alpha * J2 + beta * J4 - gamma * J6);

    qDebug() << " A:" << A;
    qDebug() << " xi:" << xi;
    qDebug() << " eta:" << eta;
    qDebug() << " nu:" << nu;
    qDebug() << " zeta:" << zeta;
    qDebug() << " A1:" << A1;
    qDebug() << " A2:" << A2;
    qDebug() << " J2:" << J2;
    qDebug() << " J4:" << J4;
    qDebug() << " J6:" << J6;
    qDebug() << " alpha:" << alpha;
    qDebug() << " beta:" << beta;
    qDebug() << " gamma:" << gamma;
    qDebug() << " Bphi:" << Bphi;

    // Final calculation of UTM coordinates
    x = xi * nu * (1 + zeta / 3.0) + 500000.0;
    y = eta * nu * (1 + zeta) + Bphi;

    // Prevent negative coordinates at Southern Hemisphere
    if (y < 0) {
        qDebug() << "Preventing negative coordinates at Southern Hemisphere";
        y = y + 10000000;
    }

    utm.append(x);
    utm.append(y);
    utm.append(zone);

    return utm;
}
