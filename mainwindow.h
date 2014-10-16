#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QString>
#include <QStringList>
#include <QActionGroup>

#include "utmcoordinates.h"
#include "gcscoordinates.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QActionGroup *datumGroup;

private slots:
    void utm2gcs();
    void gcs2utm();
    QVector<double> coordFormatter(QString rawCoordinates);
    void about();
    void updateStatusBar();
    void lat2dec(QString sexagesimal);
    void lon2dec(QString sexagesimal);
    double sex2dec(QString sexagesimal);
};

#endif // MAINWINDOW_H
