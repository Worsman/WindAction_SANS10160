#pragma once

#ifndef WINDACTIONLIB_H
#define WINDACTIONLIB_H


#include <math.h> 
#include <stdexcept> // std::invalid_argument
#include <algorithm> // for std::find
#include <iterator>  // for std::begin, std::end

namespace WindAction_library
{

  static const int BASICWINDSPEED[] = { 28,32,36 };
  static const char TERRAINCATEGORY[] = { 'A','B', 'C', 'D'};

  class WindAction
  {
    public:
            

    /**
    * Source : SANS 10160-3:2011; section 7.4; page 19
    *
    *     qp(z) = 1/2 x p x vp^2(z)
    *
    *   _iAltitude    =  meters above sea level (m)
    *   _iFundamentalBasicWindSpeed    = basic wind speed @ 10 meter above ground (m/sec)
    *   _dProbabilityofExceedance     = probability 0.02 for 50 year return
    *   _dHeight      =  meters above the terrain (m)
    *   _cTerrainCategory   = A, B ,C, D
    *   _dTopogaphyactor    = 1.0
    *
    **/
    static double PeakWindSpeedPressure(const unsigned int _iAltitude,
                                        const int _iFundamentalBasicWindSpeed,
                                        const double _dProbabilityofExceedance,
                                        const int _iHeight,
                                        const char _cTerrainCategory,
                                        const double _dTopogaphyactor);

    /**
    * Source : SANS 10160-3:2011; section 7.4; page 20
    *
    *   _iAltitude = meters above sea level (m)
    *   AirDensity  =  kilograms per cubic meter (kg/m^3)
    **/
    static double AirDensity(const unsigned int _iAltitude);

    /**
    *   Checks wheather vailid in range value is used for Basic Wind Speed
    **/
    static bool IsValidBasicWindSpeed(const int _iBasicWindSpeed);

    /**
    * Source : SANS 10160-3:2011; section 7.3.2.1; page 16
    *   Checks wheather vailid in range value is used for Terrain Categorie
    **/
    static bool IsValidTerrainCategory(const char _cTerrainCategory);

    /**
    * Source : SANS 10160-3:2011; section 7.3; page 15
    *
    *   vp(z) = Cr(z) x C0(z) x Vbpeak
    *         Vbpeak = Vb * 1.4
    *         Cr(z) = roughness factor
    *         C0(z) = topogaphy factor
    *
    *   _iFundamentalBasicWindSpeed   =  basic wind speed @ 10 meter
    *   _dProbabilityofExceedance     = probability 0.02 for 50 year return
    *   _dHeight                      =  meters above the terrain (m)
    *   _cTerrainCategory             = A, B ,C,D
    *   _dTopogaphyactor              = 1.0
    **/
    static double PeakWindSpeed(const int _iFundamentalBasicWindSpeed,
                                const double _dProbabilityofExceedance,
                                const int _iHeight,
                                const char _cTerrainCategory,
                                const double _dTopogaphyactor);

    /**
    * Source : SANS 10160-3:2011; section 7.3.2; page 15
    *
    *   Cr(z) = 1.36 x (  (Z - Zo) / (Zg - Zc) ) ^ a
    *
    *     z   = heigth above ground level
    *     Z0  = height of referance plane
    *     Zg  = gradient height
    *     Zc  = height below which not reduction in wind speed is allowed
    *     a   = exponent
    *
    *   _dHeight            =  meters above the terrain (m)
    *   _cTerrainCategory   = A, B ,C,D
    *
    **/
    static double TerrainRoughness(const int _iHeight, const char _cTerrainCategory);

    /**
    * Source : SANS 10160-3:2011; section 7.2.2; page 13
    *
    *   Vb = Cprob x Vb0
    *       Vb0 = Fundamental Basic wind speed ... figure 1 page 14  *
    *       Cprob = ( (1 - K x ln(-ln(1-p)) ) / ( 1 - K x ln(-ln0.98) ) ) ^ n
    *
    * _iFundamentalBasicWindSpeed = meter per second
    *   _dProbabilityofExceedance = probability 0.02 for 50 year return
    **/
    static double BasicWindSpeed(const int _iFundamentalBasicWindSpeed, double _dProbabilityofExceedance);


  };
}

#endif  