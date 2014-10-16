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
