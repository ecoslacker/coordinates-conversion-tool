/*
 * Coordinates
 * Conversion between geographic coordinates (LatLon) and UTM projection
 * Copyright (C) 2014-2017 Eduardo Jiménez <ecoslacker@irriapps.com>
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

#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <QObject>
#include <cmath>
#include <QString>
//#include <string>

//using namespace std;

class Ellipsoid : public QObject
{
    Q_OBJECT

public:
    //Ellipsoid(string name);
    explicit Ellipsoid(QObject *parent = 0);
    void config(QString name);
    double eccentricity();
    double secondEccentricity();
    double curvatureRadius();

private:
    //string ellipsoid;
    QString ellipsoid;
    double e;
    double a;
    double b;
    double c;
    double e_second;
};

#endif // ELLIPSOID_H
