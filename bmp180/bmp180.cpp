#include "bmp180.h"
#include "i2c.h"
#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

BMP180::BMP180(int mode)
  : _i2c(0), _cal_AC1(0), _cal_AC2(0), _cal_AC3(0), _cal_B1(0), _cal_B2(0), _cal_MB(0),
    _cal_MC(0), _cal_MD(0), _mode(mode), _cal_AC4(0), _cal_AC5(0), _cal_AC6(0)
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
  _i2c->readBytes(BMP085_CAL_B1, data, 2);
  _cal_B1 = _readS16(data);
  _i2c->readBytes(BMP085_CAL_B2, data, 2);
  _cal_B2 = _readS16(data);
  _i2c->readBytes(BMP085_CAL_MB, data, 2);
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
  UINT16 rawTemp = _readRawTemperature();
  int X1 = ((rawTemp - _cal_AC6) * _cal_AC5) >> 15;
  int X2 = ((_cal_MC << 11) / (X1 + _cal_MD));
  int B5 = X1 + X2;
  double temp = ((B5 + 8) >> 4) / 10.0;
#ifdef DEBUG
  cout << "temp: " << std::dec << temp << " C" << endl;
#endif
  return temp;
}

UINT16 BMP180::readPressure()
{
  UINT16 UT = _readRawTemperature();
  UINT32 UP = _readRawPressure();
  UINT16 p = 0x00;

  // temperature
  int X1 = ((UT - _cal_AC6) * _cal_AC5) >> 15;
  int X2 = (_cal_MC << 11) / (X1 + _cal_MD);
  int B5 = X1 + X2;

  // pressure
  int B6 = B5 - 4000;
  X1 = (_cal_B2 * (B6 * B6) >> 12) >> 11;
  X2 = (_cal_AC2 * B6) >> 11;
  int X3 = X1 + X2;
  int B3 = (((_cal_AC1 * 4 + X3) << _mode) + 2) / 4;

  X1 = (_cal_AC3 * B6) >> 13;
  X2 = (_cal_B1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  int B4 = (_cal_AC4 * (X3 + 32768)) >> 15;
cout << "UP: " << dec << UP << endl;
cout << "B3: " << dec << B3 << endl;
cout << "shift: " << dec << (50000 >> _mode) << endl;

  UINT32 B7 = ((UP - B3) * (50000 >> _mode));

  if (B7 < 0x80000000)
    p = (B7 * 2) / B4;
  else
    p = (B7 / B4) * 2;

  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

#ifdef DEBUG
  cout << "mode: " << dec << _mode << endl;
  cout << "B5: " << dec << B5 << endl;
  cout << "B6: " << dec << B6 << endl;
  cout << "B3: " << dec << B3 << endl;
  cout << "B4: " << dec << B4 << endl;
  cout << "B7: " << dec << B7 << endl;
#endif

  p = p + ((X1 + X2 + 3791) >> 4);

#ifdef DEBUG
  cout << "pressure: " << std::dec << p << endl;
#endif

  return p;
}

double BMP180::readAltitude(UINT16 seaLevelPressure)
{
  double altitude = 0;
  float pressure = readPressure();
  altitude = 44330.0 * (1.0 - pow(pressure / seaLevelPressure, 0.1903));
#ifdef DEBUG
  cout << "altitude: " << std::dec << altitude << endl;
#endif
  return altitude;
}

double BMP180::readSeaLevelPressure()
{
  return 0;
}

UINT16 BMP180::_readRawTemperature()
{
  UINT8 data[2];
  UINT16 retval = 0;
  _i2c->writeCommand (1, BMP085_READTEMPCMD);
  usleep(5000);
  if (_i2c->readBytes(BMP085_TEMPDATA, data, 2))
    retval = (data[0] << 8) | data[1];
#ifdef DEBUG
  cout << "raw temperature: 0x" << std::uppercase << std::hex << retval << endl;
#endif
  return retval;
}

UINT32 BMP180::_readRawPressure()
{
  _i2c->writeCommand(1, BMP085_READPRESSURECMD);
  switch (_mode)
  {
    case BMP085_ULTRALOWPOWER:
      usleep(5000);
      break;
    case BMP085_HIGHRES:
      usleep(14000);
      break;
    case BMP085_ULTRAHIGHRES:
      usleep(26000);
      break;
    default:
      usleep(8000);
  }

  UINT8 data = 0x00;
  _i2c->readBytes(BMP085_PRESSUREDATA, &data, 1);
  UINT8 msb = data;
  _i2c->readBytes(BMP085_PRESSUREDATA+1, &data, 1);
  UINT8 lsb = data;
  _i2c->readBytes(BMP085_PRESSUREDATA+2, &data, 1);
  UINT8 xlsb = data;
  UINT32 retval = ((msb << 16) + (lsb << 8) + xlsb) >> (8 - _mode);
#ifdef DEBUG
  cout << "raw pressure: 0x" << std::uppercase << std::hex << retval << endl;
#endif
  return retval;
}
