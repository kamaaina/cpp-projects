#ifndef I2C_H
#define I2C_H

#include "../common/common.h"

#define I2C_PORT "/dev/i2c-1"

class I2C
{
  public:
    I2C (UINT8 deviceAddr, UINT8 control);
    ~I2C();
    bool writeCommand (int numBytesToWrite, ...);
    bool writeByte (UINT8 data);
    bool writeBytes (UINT8 *data, int size);
    bool readBytes (UINT8 reg, UINT8 *data, int count);

  private:
    void init();

    UINT8 deviceAddr_;
    UINT8 control_;
    int device_; // the I2C device
    bool error_;
};

#endif
