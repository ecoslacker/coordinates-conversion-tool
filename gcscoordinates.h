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
