#include "mcp9808.h"
#include <iostream>

using namespace std;

int main()
{
   MCP9808 sensor;
   cout << "Temperature: " << sensor.readTemperatureF() << " F ("
        << sensor.readTemperatureC() << " C)" << endl;
   return 0;
}
