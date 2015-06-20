#include "mcp9808.h"
#include "i2c.h"
#include <stdint.h>

MCP9808::MCP9808()
 : i2c_(0)
{
   i2c_ = new I2C(MCP9808_I2CADDR_DEFAULT, 0x00);
   init();
}

bool MCP9808::init()
{
   uint8_t data[2];
   i2c_->readBytes(MCP9808_REG_MANUF_ID, data, 2);
//   std::cout << std::hex << "0x" << (int)data[0] << " 0x" << (int)data[1] << std::endl;
   if ((int)data[1] != 0x54)
      return false; // check manufacturer id failed
   i2c_->readBytes(MCP9808_REG_DEVICE_ID, data, 2);
   if ((int)data[0] != 0x04)
      return false; // check device id failed

   return true; // successful
}

MCP9808::~MCP9808()
{
   delete i2c_;
}

float MCP9808::readTemperatureF()
{
   return readTemperatureC() * 1.8 + 32;
}

float MCP9808::readTemperatureC()
{
   uint8_t data[2];
   i2c_->readBytes(MCP9808_REG_AMBIENT_TEMP, data, 2);
   uint16_t t = data[0] << 8;
   t |= data[1];
   float temp = t & 0x0FFF;
   temp /= 16.0;
   if (t & 0x1000)
      temp -= 256;
   return temp;
}
