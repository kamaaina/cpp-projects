#include <iostream>
#include "i2c.h"

using namespace std;

// simple program to turn off OLED

int main ()
{
   cout << "Turning display off" << endl;
   I2C i2c(0x3D, 0x00);
   i2c.writeCommand (1, 0xAE);
   return 0;
}
