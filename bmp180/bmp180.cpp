#include "bmp180.h"
#include "i2c.h"
#include <iostream>
#include <unistd.h>

using namespace std;

BMP180::BMP180()
  : _i2c(0)
{
  _i2c = new I2C();
}

BMP180::~BMP180()
{
  delete _i2c;
  _i2c = 0;
}

void BMP180::init()
{
  cout << "init" << endl;
  // FIXME: read calibration
}

double BMP180::readTemperature()
{
  UINT8 data[2];
  UINT16 rawTemp;
  _i2c->writeCommand (1, 0x2E);
  usleep(100);
  bool retval = _i2c->readBytes(BMP085_TEMPDATA, data, 2);
  if (retval)
  {
    rawTemp = (data[0] << 8) | data[1];
#ifdef DEBUG
    cout << "raw temperature: 0x" << std::hex << rawTemp << endl;
#endif
  }
  return 0;
}

double BMP180::readPressure()
{
  return 0;
}

double BMP180::readAltitude()
{
  return 0;
}

double BMP180::readSeaLevelPressure()
{
  return 0;
}
