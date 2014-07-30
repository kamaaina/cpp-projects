#include "bmp180.h"
#include <iostream>

using namespace std;

int main()
{
   cout << "testing BMP180" << endl;
   BMP180 sensor = BMP180();
   cout << "Temperature: " << sensor.readTemperature() << " m" << endl;
   cout << "Pressure: " << sensor.readPressure() << " Pa" << endl;
   cout << "Altitude: " << sensor.readAltitude() << " m" << endl;
   return 0;
}
