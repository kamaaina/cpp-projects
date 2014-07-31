#include "bmp180.h"
#include <iostream>

using namespace std;

int main()
{
   cout << "testing BMP180" << endl;
   BMP180 sensor = BMP180();
   cout << "Temperature: " << sensor.readTemperature() << " C" << endl;
   cout << "Pressure: " << sensor.readPressure() << " Pa" << endl;
   cout << "Sea Level Pressure: " << sensor.readSeaLevelPressure() << " Pa" << endl;
   cout << "Altitude: " << sensor.readAltitude() << " m" << endl;
   return 0;
}
