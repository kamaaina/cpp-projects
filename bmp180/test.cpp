#include "bmp180.h"
#include <iostream>

using namespace std;

int main()
{
   cout << "testing BMP180" << endl;
   BMP180 sensor = BMP180();
   sensor.readTemperature();
   cout << "done" << endl;
   return 0;
}
