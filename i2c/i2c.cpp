#include <unistd.h>
#include <cstdarg>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#ifdef DEBUG
#include <iostream>
#endif
#include "i2c.h"

/************************************************************
 * Constructor
 ************************************************************/
I2C::I2C (UINT8 deviceAddr, UINT8 control)
: deviceAddr_(deviceAddr), control_(control), device_(0), error_(0)
{
   init();
}

/************************************************************
 * Destructor
 ************************************************************/
I2C::~I2C ()
{
   if (device_)
   {
      close(device_);
      device_ = 0;
   }
}

/************************************************************
 * Initializes the I2C device. If there is an error, the
 * error flag is set to true
 ************************************************************/
void I2C::init()
{
   // Try to open /dev/i2c-x port
   if ((device_ = open(I2C_PORT, O_RDWR)) < 0)
   {
#ifdef DEBUG
      std::cerr << "could not open i2c port: " << I2C_PORT << std::endl;
#endif
      error_ = true;
   }

   // Try to access the device
   if (ioctl(device_, I2C_SLAVE, deviceAddr_) < 0)
   {
#ifdef DEBUG
      std::cerr << "could not find device on address: 0x" <<  std::hex
         << std::uppercase << deviceAddr_ << ". No data will be written."
         << std::endl;
#endif
      close(device_);
      device_ = 0x00;
      error_ = true;
   }
}

/************************************************************
 * Writes the specified number of bytes to the device. This
 * method handles the inserting of the control byte before
 * sending the commands.
 *
 * Parameters:
 *   numBytesToWrite - the number of bytes to be written to
 *                     the device
 *   ...             - the bytes to be written
 *
 * Returns true if the bytes have been successfully written
 * to the device; false otherwise.
 ************************************************************/
bool I2C::writeCommand(int numBytesToWrite, ...)
{
   if (error_)
      return false;

   va_list arguments;
   bool retVal = false;
   UINT8 *data = new UINT8[numBytesToWrite+1];
   memset (data, 0, numBytesToWrite+1);
   data[0] = 0xF4; // send control byte and then cmd(s)

   va_start (arguments, numBytesToWrite);
   for (int i=1; i<numBytesToWrite+1; i++)
      data[i] = ((UINT8)va_arg (arguments, int));
   va_end (arguments);

   writeBytes (data, numBytesToWrite+1);
   delete [] data;
   data = 0;

   return retVal;
}

/************************************************************
 * Writes the bytes to the device
 *
 * Parameters
 *   data - pointer to the array of data to be written
 *   size - size of the array that is to be written
 *
 * Returns true if the data has been successfully written,
 * false otherwise
 ************************************************************/
bool I2C::writeBytes (UINT8 *data, int size)
{
   if (error_)
      return false;

#ifdef DEBUG
  std::cout << "Writing: ";
  for (int i=0; i<size; i++)
    {
      if (i > 0)
        std::cout << ", ";
      std::cout << std::hex << (int)data[i];
    }
  std::cout << " to address " << std::hex << deviceAddr_ << std::endl;
#endif
   if (write(device_, data, size) != size)
   {
#ifdef DEBUG
      std::cerr << "Error writing to i2c " << std::hex << deviceAddr_ << std::endl;
#endif
      return false;
   }
   usleep(1);

   return true;
}

/************************************************************
 * Writes a byte of data to the device
 *
 * Parameters:
 *   data - the data to be written to the device
 *
 * Returns true if the data has been successfully written to
 * the device, false otherwise
 ************************************************************/
bool I2C::writeByte (UINT8 data)
{
   UINT8 buf[1];
   memset(buf, 0x00, sizeof(buf));
   buf[0] = data;
   return writeBytes (buf, 1);
}

/************************************************************
* Reads count bytes into data
*
* Parameters:
*   reg - the register to read from
*   data - the buffer to hold the data read
*   count - the number of bytes to read
*
* Returns true if data has been successfully read from the
* device; false otherwise
************************************************************/
bool I2C::readBytes (UINT8 reg, UINT8 *data, int count)
{
   memset(data, 0x00, count);
   writeBytes(&reg, 1);
   return (read (device_, data, count) == count);
}
