///
/// Created by Igorfel 21/04/16
///
#ifndef ACCELHANDLER_H
#define ACCELHANDLER_H

#include <QTime>
#include <QObject>
#include <QAccelerometer>
#include "matrix.h"

class AccelHandler : public QObject, QAccelerometerFilter
{
    Q_OBJECT
public:
    explicit AccelHandler(unsigned samplingPeriod = 100);                   //Used when communicating with external device
             AccelHandler(LinAlg::Matrix<double> externalDataX = "0",
                          LinAlg::Matrix<double> externalDataY = "0",
                          LinAlg::Matrix<double> externalDataZ = "0");
             AccelHandler(QAccelerometer::AccelerationMode AccelMode = QAccelerometer::Combined, unsigned dataRate = 10);

    void                   setSamplingData();
    LinAlg::Matrix<double> getSamplingData();

    qreal getX(){ return x; }
    qreal getY(){ return y; }
    qreal getZ(){ return z; }

    LinAlg::Matrix<double> getAccelX(){ return AccelX; }
    LinAlg::Matrix<double> getAccelY(){ return AccelY; }
    LinAlg::Matrix<double> getAccelZ(){ return AccelZ; }

    double getPhysicalAccelX(){ return physicalXLP1; }
    double getPhysicalAccelY(){ return physicalYLP1; }
    double getPhysicalAccelZ(){ return physicalZLP1; }

    double getPhysicalAccelX2(){ return physicalXLP2; }
    double getPhysicalAccelY2(){ return physicalYLP2; }
    double getPhysicalAccelZ2(){ return physicalZLP2; }

private:
    unsigned samplingPeriod;

    double physicalXLP1,physicalYLP1,physicalZLP1, physicalXLP2,physicalYLP2,physicalZLP2, deltaTime;
    QTime t;
    qreal x,y,z;
    QAccelerometer* m_sensor;
    LinAlg::Matrix<double> AccelX, AccelY, AccelZ, Gravity;

    void lowPassFilter(double alpha);
    void lowPassFilter2(double alpha1, double alpha2, double time);
    void initAccelDataCapacity();
    void startListeningExternal();
    void startListeningAccelerometer(QAccelerometer::AccelerationMode AccelMode, unsigned dataRate);

    //Override of QAcclerometerFilter::filter(QAccelerometerReading*)
    bool filter(QAccelerometerReading* reading);

private slots:
    void Update();

signals:
    void dataUpdated();

};

#endif // ACCELHANDLER_H
