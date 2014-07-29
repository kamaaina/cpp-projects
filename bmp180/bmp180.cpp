#include "bmp180.h"
#include "i2c.h"
#include <iostream>
#include <unistd.h>

using namespace std;

BMP180::BMP180()
  : _i2c(0), _cal_AC1(0), _cal_AC2(0), _cal_AC3(0), _cal_B1(0), _cal_B2(0), _cal_MB(0),
    _cal_MC(0), _cal_MD(0), _cal_AC4(0), _cal_AC5(0), _cal_AC6(0)
{
  _i2c = new I2C();
  _init();
}

BMP180::~BMP180()
{
  delete _i2c;
  _i2c = 0;
}

void BMP180::_init()
{
  UINT8 data[2];
  _cal_AC1 = _readS16(_i2c->readBytes(BMP085_CAL_AC1, data, 2));
  _cal_AC2 = _readS16(_i2c->readBytes(BMP085_CAL_AC2, data, 2));
  _cal_AC3 = _readS16(_i2c->readBytes(BMP085_CAL_AC3, data, 2));
  _cal_AC4 = _readU16(_i2c->readBytes(BMP085_CAL_AC4, data, 2));
  _cal_AC5 = _readU16(_i2c->readBytes(BMP085_CAL_AC5, data, 2));
  _cal_AC6 = _readU16(_i2c->readBytes(BMP085_CAL_AC6, data, 2));
#ifdef DEBUG
  cout << "calibrated values read:" << endl;
#endif
}

int BMP180::_readU16(UINT *data)
{
  return ((data[0] << 8) | data[1]);
}

int BMP180::_readS16(UINT *data)
{
  int hi = _readS8(data[0]);
  int low = data[1];
  return ((hi << 8) | data[1]);
}

int BMP180::_readS8(UINT *data)
{
  if (data[0] > 127)
    return (int)data[0] - 256;
  else
    return (int)data[0];
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
