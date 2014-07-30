#ifndef _BMP180_H_
#define _BMP180_H_

#include "../common/common.h"

// Modes
#define BMP085_ULTRALOWPOWER     0
#define BMP085_STANDARD          1
#define BMP085_HIGHRES           2
#define BMP085_ULTRAHIGHRES      3

// registers
#define BMP085_CAL_AC1           0xAA  // Calibration data (16 bits)
#define BMP085_CAL_AC2           0xAC  // Calibration data (16 bits)
#define BMP085_CAL_AC3           0xAE  // Calibration data (16 bits)
#define BMP085_CAL_AC4           0xB0  // Calibration data (16 bits)
#define BMP085_CAL_AC5           0xB2  // Calibration data (16 bits)
#define BMP085_CAL_AC6           0xB4  // Calibration data (16 bits)
#define BMP085_CAL_B1            0xB6  // Calibration data (16 bits)
#define BMP085_CAL_B2            0xB8  // Calibration data (16 bits)
#define BMP085_CAL_MB            0xBA  // Calibration data (16 bits)
#define BMP085_CAL_MC            0xBC  // Calibration data (16 bits)
#define BMP085_CAL_MD            0xBE  // Calibration data (16 bits)
#define BMP085_CONTROL           0xF4
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD       0x2E
#define BMP085_READPRESSURECMD   0x34

class I2C;

class BMP180
{
  public:
    BMP180(int mode=BMP085_STANDARD);
    ~BMP180();

    double readTemperature();
    UINT16 readPressure();
    double readAltitude(UINT16 seaLevelPressure=101325);
    double readSeaLevelPressure();

  private:
    void _init();
    int _readU16(UINT8 *data);
    int _readS16(UINT8 *data);
    int _readS8(UINT8 *data);
    UINT16 _readRawTemperature();
    UINT32 _readRawPressure();

    I2C* _i2c;
    int _cal_AC1, _cal_AC2, _cal_AC3, _cal_B1, _cal_B2, _cal_MB, _cal_MC, _cal_MD, _mode;
    UINT16 _cal_AC4, _cal_AC5, _cal_AC6;
};

#endif /* _BMP180_H_ */
