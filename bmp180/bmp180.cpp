#include "bmp180.h"
#include "i2c.h"
#include <iostream>
#include <unistd.h>

using namespace std;

BMP180::BMP180()
  : _i2c(0), _cal_AC1(0), _cal_AC2(0), _cal_AC3(0), _cal_B1(0), _cal_B2(0), _cal_MB(0),
    _cal_MC(0), _cal_MD(0), _cal_AC4(0), _cal_AC5(0), _cal_AC6(0)
{
  _i2c = new I2C(0x77, 0xF4);
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
  _i2c->readBytes(BMP085_CAL_AC1, data, 2);
  _cal_AC1 = _readS16(data);
  _i2c->readBytes(BMP085_CAL_AC2, data, 2);
  _cal_AC2 = _readS16(data);
  _i2c->readBytes(BMP085_CAL_AC3, data, 2);
  _cal_AC3 = _readS16(data);
  _i2c->readBytes(BMP085_CAL_AC4, data, 2);
  _cal_AC4 = _readU16(data);
  _i2c->readBytes(BMP085_CAL_AC5, data, 2);
  _cal_AC5 = _readU16(data);
  _i2c->readBytes(BMP085_CAL_AC6, data, 2);
  _cal_AC6 = _readU16(data);
  _i2c->readBytes(BMT085_CAL_B1, data, 2);
  _cal_B1 = _readS16(data);
  _i2c->readBytes(BMT085_CAL_B2, data, 2);
  _cal_B2 = _readS16(data);
  _i2c->readBytes(BMT085_CAL_MB, data, 2);
  _cal_MB = _readS16(data);
  _i2c->readBytes(BMP085_CAL_MC, data, 2);
  _cal_MC = _readS16(data);
  _i2c->readBytes(BMP085_CAL_MD, data, 2);
  _cal_MD = _readS16(data);
#ifdef DEBUG
  cout << "calibrated values read:" << std::dec << endl;
  cout << "_cal_AC1: " << _cal_AC1 << endl;
  cout << "_cal_AC2: " << _cal_AC2 << endl;
  cout << "_cal_AC3: " << _cal_AC3 << endl;
  cout << "_cal_AC4: " << _cal_AC4 << endl;
  cout << "_cal_AC5: " << _cal_AC5 << endl;
  cout << "_cal_AC6: " << _cal_AC6 << endl;
  cout << "_cal_B1: " << _cal_B1 << endl;
  cout << "_cal_B2: " << _cal_B2 << endl;
  cout << "_cal_MB: " << _cal_MB << endl;
  cout << "_cal_MC: " << _cal_MC << endl;
  cout << "_cal_MD: " << _cal_MD << endl;
#endif
}

int BMP180::_readU16(UINT8 *data)
{
  return ((data[0] << 8) | data[1]);
}

int BMP180::_readS16(UINT8 *data)
{
  int hi = _readS8(&data[0]);
  return ((hi << 8) | data[1]);
}

int BMP180::_readS8(UINT8 *data)
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
    int X1 = ((rawTemp - _cal_AC6) * _cal_AC5) >> 15;
    int X2 = ((_cal_MC << 11) / (X1 + _cal_MD));
    int B5 = X1 + X2;
    double temp = ((B5 + 8) >> 4) / 10.0;
#ifdef DEBUG
    cout << "temp: " << std::dec << temp << " C" << endl;
#endif
    return temp; // in celcisus
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
