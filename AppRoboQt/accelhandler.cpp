#include "accelhandler.h"

AccelHandler::AccelHandler(unsigned samplingPeriod)
{
    this->samplingPeriod = samplingPeriod;
    startListeningExternal();
}

AccelHandler::AccelHandler(LinAlg::Matrix<double> externalDataX, LinAlg::Matrix<double> externalDataY, LinAlg::Matrix<double> externalDataZ)/* : QObject(parent)*/
{
    this->AccelX = externalDataX;
    this->AccelY = externalDataY;
    this->AccelZ = externalDataZ;

    this->samplingPeriod = this->AccelX.getNumberOfColumns();
}

AccelHandler::AccelHandler(QAccelerometer::AccelerationMode AccelMode, unsigned dataRate)
{
    startListeningAccelerometer(AccelMode, dataRate);
}

void AccelHandler::startListeningExternal()
{
    initAccelDataCapacity();
    Update();
}

void AccelHandler::startListeningAccelerometer(QAccelerometer::AccelerationMode AccelMode, unsigned dataRate)
{
    initAccelDataCapacity();

    this->m_sensor = new QAccelerometer(this);
    this->m_sensor->addFilter(this);
    this->m_sensor->connectToBackend();
    this->m_sensor->setAccelerationMode(AccelMode);
    this->m_sensor->setDataRate(dataRate);
    this->m_sensor->start();

    qDebug("Accel started");
}

void AccelHandler::initAccelDataCapacity()
{
    Gravity = "0,0,0";

    this->AccelX = LinAlg::Zeros<double>(1, 1);
    this->AccelY = LinAlg::Zeros<double>(1, 1);
    this->AccelZ = LinAlg::Zeros<double>(1, 1);

    this->physicalXLP1 = 0;
    this->physicalYLP1 = 0;
    this->physicalZLP1 = 0;

    this->physicalXLP2 = 0;
    this->physicalYLP2 = 0;
    this->physicalZLP2 = 0;

    //t.start();
}

void AccelHandler::Update()
{
    if(this->AccelX.getNumberOfColumns() < this->samplingPeriod)
    {
        this->AccelX = this->AccelX | this->x;
        this->AccelY = this->AccelY | this->y;
        this->AccelZ = this->AccelZ | this->z;
    }
    else
    {
        for(unsigned i = 1; i < this->samplingPeriod; ++i)
        {
            this->AccelX(1,i) = this->AccelX(1,i+1);
            this->AccelY(1,i) = this->AccelY(1,i+1);
            this->AccelZ(1,i) = this->AccelZ(1,i+1);
        }

        this->AccelX(1, this->samplingPeriod) = this->x;
        this->AccelY(1, this->samplingPeriod) = this->y;
        this->AccelZ(1, this->samplingPeriod) = this->z;
    }

    emit this->dataUpdated();
}

bool AccelHandler::filter(QAccelerometerReading* reading)
{
    this->x = reading->x();
    this->y = reading->y();
    this->z = reading->z();

    lowPassFilter(0.98);
    //this->deltaTime = t.elapsed();
    lowPassFilter2(0.08, 0.5, 0.1/*this->deltaTime*/);
    //t.start();
    //Update(); //Função com erro
    emit this->dataUpdated();
    //qDebug("Current device acceleration: x=%f y=%f z=%f", physicalXLP1, physicalYLP1, physicalZLP1);
    return 1;
}

void AccelHandler::lowPassFilter(double alpha)
{
    Gravity(1,1) = alpha * Gravity(1,1) + (1 - alpha) * this->x;
    Gravity(1,2) = alpha * Gravity(1,2) + (1 - alpha) * this->y;
    Gravity(1,3) = alpha * Gravity(1,3) + (1 - alpha) * this->z;

    this->physicalXLP1 = this->x - Gravity(1,1);
    this->physicalYLP1 = this->y - Gravity(1,2);
    this->physicalZLP1 = this->z - Gravity(1,3);
}

void AccelHandler::lowPassFilter2(double alpha1, double alpha2, double time)
{
    Gravity(1,1) = Gravity(1,1) + (time / alpha1) * (this->x - Gravity(1,1));
    Gravity(1,2) = Gravity(1,2) + (time / alpha1) * (this->y - Gravity(1,2));
    Gravity(1,3) = Gravity(1,3) + (time / alpha1) * (this->z - Gravity(1,3));

    this->physicalXLP2 = this->physicalXLP2 + (time / alpha2) * ((this->x - Gravity(1,1))- this->physicalXLP2);
    this->physicalYLP2 = this->physicalYLP2 + (time / alpha2) * ((this->y - Gravity(1,2))- this->physicalYLP2);
    this->physicalZLP2 = this->physicalZLP2 + (time / alpha2) * ((this->z - Gravity(1,3))- this->physicalZLP2);
}
