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

#ifndef UTMCOORDINATES_H
#define UTMCOORDINATES_H
#define _USE_MATH_DEFINES

#include <QObject>
#include <QString>
#include <QPointF>
#include <QDebug>
#include <cmath>

#include "ellipsoid.h"

class UTMCoordinates : public QObject
{
    Q_OBJECT
public:
    explicit UTMCoordinates(QObject *parent = 0);
    void setXY(double xCoord, double yCoord);
    void setZone(int utmZone);
    void setHemisphere(bool h);
    void setEllipsoid(QString cEllipsoid);
    QPointF toGCS();

signals:

public slots:

private:
    QPointF coordinates;
    int zone;
    bool hemisphere;
    Ellipsoid ellipsoid;
};

#endif // UTMCOORDINATES_H
