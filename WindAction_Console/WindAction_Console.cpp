
#include <iostream>
#include "../WindAction_Library/WindActionLib.h"

using namespace std;
using namespace WindAction_library;


int main()
{
  double Windpresure = WindAction::PeakWindSpeedPressure(1500, 28, 0.02, 12, 'A', 1.0);



  return 0;
}