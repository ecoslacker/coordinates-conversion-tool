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
