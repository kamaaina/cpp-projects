#ifndef OLED_H
#define OLED_H

#include <string>
#include "../common/common.h"

#define WIDTH     128          // 128 columns of 8bits deep
#define HEIGHT    8            // 4 rows, 8 bits high
#define TILE_SIZE 8
#define STEP_SIZE 16

/*
   Class OLED will drive an Adafruit 128x64 I2C OLED Display
   on a Raspberry Pi using i2c port defined above.
*/

class I2C;

class OLED
{
   public:
      OLED  ();
      ~OLED ();
      void clearBuffer ();
      void clearScreen ();
      bool writeScreen ();
      bool writeCenter (std::string text, UINT8 row);
      bool writeText (std::string text, UINT8 row, UINT8 col);
      bool writeImage (UINT8* image);

   private:
      bool init ();
      UINT8 reverseByte (UINT8 b);
      void getTileFromBitmap (int index);
      void setTileInBuffer(int index);
      void rotateTile();

      UINT8 *screenBuf_;
      UINT8 *data_;
      UINT8 *tmp_;
      I2C *i2c_;
};

#endif
