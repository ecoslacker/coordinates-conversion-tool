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
