#include "bmp180.h"
#include <iostream>

using namespace std;

int main()
{
   cout << "testing BMP180" << endl;
   BMP180 sensor = BMP180();
   double tempC = sensor.readTemperature();
   cout << "Temperature: " << tempC << " C (" << tempC * 1.8 + 32 << " F)" << endl;
   cout << "Pressure: " << sensor.readPressure() << " Pa" << endl;
   cout << "Sea Level Pressure: " << sensor.readSeaLevelPressure() << " Pa" << endl;
   double altM = sensor.readAltitude();
   cout << "Altitude: " << altM << " m (" << altM * 3.28084 << " feet)" << endl;
   return 0;
}
