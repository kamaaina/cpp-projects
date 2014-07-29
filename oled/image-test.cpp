#include <iostream>
#include <math.h>
#include <string.h>
#include "hi_logo.h" // must be in vertical format!
#include "dahui.h"
#include "i2c.h"

typedef unsigned char UINT8;

using namespace std;

UINT8 data[8];
UINT8 buf[1024];
UINT8 tmp[8];

void getBytes(int index);
void setBytes(int index);
UINT8 reverse(UINT8 b);
void mashData();

int main ()
{
   memset(data, 0x0, sizeof(data));
   memset(buf, 0x0, sizeof(buf));
   memset(tmp, 0x0, sizeof(tmp));

   memcpy(buf, hi_logo, sizeof(hi_logo));

//   cout << "hi_logo size: " << sizeof(hi_logo) << endl;
//   cout << "buf size: " << sizeof(hi_logo) << endl;

/*
   //for (int lcv=0; lcv<sizeof(buf); lcv++)
   for (int lcv=0; lcv<(int)sizeof(buf); lcv++)
   {
      getBytes(lcv);

#if 0 // DEBUG bytes to test
      data[0] = 0xF8;
      data[1] = 0x1F;
      data[2] = 0x00;
      data[3] = 0x00;
      data[4] = 0x10;
      data[5] = 0x30;
      data[6] = 0x00;
      data[7] = 0x00;
#endif

      // rotate the 8x8 tile
      mashData();

#ifdef DEBUG
      cout << "==============temp buffer===========" << endl;
#endif
      // reverse the bites in the byte
      for (int i=0; i<(int)sizeof(tmp); i++)
      {
         tmp[i] = reverse(tmp[i]);
#ifdef DEBUG
         cout << hex << uppercase << "0x" << (int)tmp[i] << endl;
#endif
      }

      setBytes(lcv);
      break; // DEBUG
   }
*/

#if 0
   for (int i=0; i<1024; i++)
   {
      if (i > 0 && i % 8 == 0)
         cout << " - ";
      if (i > 0 && i % 16 == 0)
         cout << endl;
      cout << "0x" << hex << uppercase << (int)buf[i] << " ";
   }
   cout << endl;
#endif

#if 1
   I2C i2c(0x3D, 0x00);

   i2c.writeCommand (1, 0xAE);             // Display off
   i2c.writeCommand (2, 0xD5, 0x80);       // set display clock division
   i2c.writeCommand (2, 0xA8, 0x3F);       // set multiplex
   i2c.writeCommand (2, 0xD3, 0x00);       // set display offset
   i2c.writeCommand (1, 0x40);             // set start line #0
   i2c.writeCommand (2, 0x8D, 0x14);       // set charge pump
   i2c.writeCommand (2, 0x20, 0x00);       // Memory mode
   i2c.writeCommand (1, 0xA1);             // Segremap(0xA0 = reset, 0xA1 = 127 = SEG0)
   i2c.writeCommand (1, 0xC8);             // Com scan dec (0xC0 = reset normal, 0xC8 = scan  from Com[n-1] - Com 0
   i2c.writeCommand (2, 0xDA, 0x12);       // Set com pins
   i2c.writeCommand (2, 0x81, 0xCF);       // Set contrast
   i2c.writeCommand (2, 0xD9, 0xF1);       // Set precharge
   i2c.writeCommand (2, 0xDB, 0x40);       // Set Vcom select
   i2c.writeCommand (1, 0xA4);             // Resume RAM content display
   i2c.writeCommand (1, 0xA6);             // Normal display not inverted
   i2c.writeCommand (1, 0x00);             // low col=0
   i2c.writeCommand (1, 0x10);             // high col=0
   i2c.writeCommand (1, 0x40);             // line #0
   i2c.writeCommand (1, 0xAF);             // Display ON

   i2c.writeCommand (3, 0x21, 0x00, 0x7F); // Set column address; start 0, end 127
   i2c.writeCommand (3, 0x22, 0x00, 0x07); // Set row address; start 0, end 7

   UINT16 uiIndex = 0x00;
   UINT8 uchTemp[0x09];

   while (uiIndex < (1024))
   {
      uchTemp[0] = 0x40;           // Co = 0; D/C# = 1 for data
      for (int i = 0; i < 8; i++)  // Copy 8 bytes to temp buffer
         uchTemp[i+1] = buf[uiIndex+i];
      i2c.writeBytes (uchTemp, 0x09); //Write control byte with 8 data bytes
      uiIndex += 0x08;             // Move to next Octet
   }
#endif

   return 0;
}

void getBytes(int index)
{
   int step = 0;
   for (int i=0; i<(int)sizeof(data); i++, step+=16)
   {
      data[i] = buf[index+step];
#if 0
      cout << "getting byte at index: " << dec << index+step << " with value: " 
         << hex << uppercase << "0x" << (int)buf[index+step] << endl;
#endif
   }
}

void setBytes(int index)
{
   int step = 0;
   for (int i=0; i<(int)sizeof(data); i++, step+=16)
   {
      buf[index+step] |= tmp[i];
#ifdef DEBUG
      cout << "setting byte at index: " << dec << index+step << " with value: "
         << hex << uppercase << "0x" << (int)tmp[i] << endl;
#endif
   }
}

UINT8 reverse(UINT8 b)
{
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void mashData()
{
   // process bits for a bytes in data
   for (int i=0; i<8; i++)
   {
#ifdef DEBUG
      cout << " process bit " << dec << i << endl;
#endif

      // process data bytes
      for (int j=0; j<(int)sizeof(data); j++)
      {
#ifdef DEBUG
         cout << " anding data 0x" << hex << uppercase << (int)data[j]
            << " with 2 to the " << 7-i << endl;
#endif
         UINT8 temp = data[j] & (UINT8)pow(2, 7-i);
#ifdef DEBUG
         cout << "\ttemp before shift: " << hex << uppercase << "0x" << (int)temp << endl;
#endif
         int shift = 7-i-j;
         UINT8 shifted = 0x0;
         if (shift < 0)
         {
            shift *= -1;
#ifdef DEBUG
            cout << "\tshifting temp to the LEFT " << dec << shift << endl;
#endif
            shifted = temp << shift;
         }
         else
         {
#ifdef DEBUG
            cout << "\tshifting temp to the RIGHT " << dec << shift << endl;
#endif
            shifted = temp >> shift;
         }
#ifdef DEBUG
         cout << "\tshifted temp value: " << hex << uppercase << "0x" << (int)shifted << endl;
         cout << "\ttmp[" << dec << i << "] beofre OR: " << hex << uppercase << (int)tmp[i] << endl;
#endif
         tmp[i] |= shifted;
#ifdef DEBUG
         cout << "\ttmp[" << dec << i << "] after OR: " << hex << uppercase << "0x" << (int)tmp[i] << endl;
#endif
      }
      tmp[i] = reverse(tmp[i]);
   }
}
