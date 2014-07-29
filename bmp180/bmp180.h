#ifndef BMP180_H_
#define BMP180_H_

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
    BMP180();
    ~BMP180();

    void init();
    double readTemperature();
    double readPressure();
    double readAltitude();
    double readSeaLevelPressure();

  private:
    I2C* _i2c;
};

#endif /* BMP180_H_ */
