#include "WindActionLib.h"
using namespace std;

namespace WindAction_library
{

  /**
  * Source : SANS 10160-3:2011; section 7.4; page 19
  *
  *     qp(z) = 1/2 x p x vp^2(z)  
  *   
  *   _iAltitude          =  meters above sea level (m)
  *   _iFundamentalBasicWindSpeed    = basic wind speed @ 10 meter above ground (m/sec)
  *   _dProbabilityofExceedance     = probability 0.02 for 50 year return
  *   _dHeight            =  meters above the terrain (m)
  *   _cTerrainCategory   = A, B ,C,D
  *   _dTopogaphyactor    = 1.0
  *
  **/
  double WindAction::PeakWindSpeedPressure( const unsigned int _iAltitude = 0,
                                            const int _iFundamentalBasicWindSpeed = 28,
                                            const double _dProbabilityofExceedance = 0.02,
                                            const int _iHeight = 1, 
                                            const char _cTerrainCategory = 'A',
                                            const double _dTopogaphyactor = 1.0)
                                          
  {
    double dReturn = 0.0;

    if(IsValidBasicWindSpeed(_iFundamentalBasicWindSpeed)
      && IsValidTerrainCategory(_cTerrainCategory)
      )
    { 
      double dp = AirDensity(_iAltitude);
      double dVp = PeakWindSpeed(_iFundamentalBasicWindSpeed,
                                _dProbabilityofExceedance,
                                _iHeight,
                                _cTerrainCategory,
                                _dTopogaphyactor);
      dReturn = (1.0 / 2.0) * dp * pow(dVp, 2.0);
    }//if - Valid Input    

    return dReturn;
  }//PeakWindSpeedPressure()

  /**
  * Source : SANS 10160-3:2011; section 7.2; page 14
  *   Checks wheather vailid in range value is used for Basic Wind Speed
  *   basic wind speed @ 10 meter >> figure 1 page 14
  **/

  bool WindAction::IsValidBasicWindSpeed(const int _iFundamentalBasicWindSpeed)
  {
    bool bExists = std::find(std::begin(BASICWINDSPEED), std::end(BASICWINDSPEED), _iFundamentalBasicWindSpeed) != std::end(BASICWINDSPEED);
    if (!bExists)
    {
      throw std::invalid_argument("received Invaild Basic Wind Speed value");
    }//if not Valid
    else
    {
      return bExists;
    }//else Valid
  }//IsValidBasicWindSpeed()

   /**
   * Source : SANS 10160-3:2011; section 7.3.2.1; page 16
   *   Checks wheather vailid in range value is used for Terrain Categorie
   **/
  bool WindAction::IsValidTerrainCategory(const char _cTerrainCategory)
  {
    bool bExists = std::find(std::begin(TERRAINCATEGORIES), std::end(TERRAINCATEGORIES), _cTerrainCategory) != std::end(TERRAINCATEGORIES);
    if (!bExists)
    {
      throw std::invalid_argument("received Invaild Terrain Category value");
    }//if not Valid
    else
    {
      return bExists;
    }//else Valid
  }//IsValidTerrainCategory()

  /**
  * Source : SANS 10160-3:2011; section 7.4; page 20
  * 
  *   _iAltitude  = meters above sea level (m)
  *   AirDensity  =  kilograms per cubic meter (kg/m^3)
  **/
  double WindAction::AirDensity(const unsigned int _iAltitude)
  {
    double dM_slope = 0.0;
    double dReturn = 0.0;

    if (_iAltitude < 0)
    {
      throw std::invalid_argument("received Invaild Altitude value");
    }//if - negative
    else if(_iAltitude == 0)
    {
      dReturn = 1.20;

    }//if
    else if (_iAltitude > 0 && _iAltitude <= 500)// y=mx+c linear interpolation
    {
      dM_slope = (1.12 - 1.20) / (500 - 0);//(y2-y1)/(x2-x1)
      dReturn = dM_slope * _iAltitude + (1.20 - dM_slope * 0);//c = y1 - m*x1
    }//if 0 to 500
    else if (_iAltitude > 500 && _iAltitude <= 1000)
    {
      dM_slope = (1.06 - 1.12) / (1000 - 500);//(y2-y1)/(x2-x1)
      dReturn = dM_slope * _iAltitude + (1.12 - dM_slope * 500);//c = y1 - m*x1
    }//if 500 to 1000
    else if (_iAltitude > 1000 && _iAltitude <= 1500)
    {
      dM_slope = (1.0 - 1.06) / (1500 - 1000);//(y2-y1)/(x2-x1)
      dReturn = dM_slope * _iAltitude + (1.06 - dM_slope * 1000);//c = y1 - m*x1
    }//if 1000 to 1500
    else if (_iAltitude > 1500 && _iAltitude <= 2000)
    {
      dM_slope = (0.94 - 1.0) / (2000 - 1500);//(y2-y1)/(x2-x1)
      dReturn = dM_slope * _iAltitude + (1.0 - dM_slope * 1500);//c = y1 - m*x1
    }//if 1000 to 1500
    else if (_iAltitude > 2000)
    {
      dReturn = 0.94;
    }//else 

    return dReturn;
  }//AirDensity()
  

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
  double WindAction::PeakWindSpeed( const int _iFundamentalBasicWindSpeed,
                                    const double _dProbabilityofExceedance,
                                    const int _iHeight, 
                                    const char _cTerrainCategory,
                                    const double _dTopogaphyactor)
  {
    double dReturn = 0.0;
 
    double dVbpeak = 1.4 *  BasicWindSpeed(_iFundamentalBasicWindSpeed, _dProbabilityofExceedance);
    double dCr = TerrainRoughness(_iHeight,_cTerrainCategory);
    dReturn = dVbpeak * dCr * _dTopogaphyactor;    

    return dReturn;
  }//PeakWindSpeed()

  /**
  * Source : SANS 10160-3:2011; section 7.2.2; page 13
  *
  *   Vb = Cprob x Vb0
  *       Vb0 = Fundamental Basic wind speed ... figure 1 page 14  *
  *       Cprob = ( (1 - K x ln(-ln(1-p)) ) / ( 1 - K x ln(-ln0.98) ) ) ^ n
  *
  *   _iFundamentalBasicWindSpeed   =  basic wind speed @ 10 meter
  *   _dProbabilityofExceedance     = probability 0.02 for 50 year return
  **/
  double WindAction::BasicWindSpeed(const int _iFundamentalBasicWindSpeed, double _dProbabilityofExceedance)
  {
    double dReturn = 0.0;
    double dK = 0.2;
    double dn = 0.5;

    double dtemp = ( (1 - dK * std::log(-1 * std::log(1- _dProbabilityofExceedance)))/(1 - dK * std::log(-1 * std::log(0.98))) );
    double dCprob = pow(dtemp, dn);
    dReturn = dCprob * _iFundamentalBasicWindSpeed;

    return dReturn;
  }//BasicWindSpeed()


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
  **/
  double WindAction::TerrainRoughness(const int _iHeight, const char _cTerrainCategory)
  {

    double dReturn = 0.0;
    int iZ0 = 0;
    int iZg = 0;
    int iZc = 0;
    double dAExponent = 0.0;
    
    switch (_cTerrainCategory)
    {
        case TERRAINCATEGORY_A:
          iZ0 = 0;
          iZg = 250;
          iZc = 1;
          dAExponent = 0.070;
          break;
        case TERRAINCATEGORY_B:
          iZ0 = 0;
          iZg = 300;
          iZc = 2;
          dAExponent = 0.095;
          break;
        case TERRAINCATEGORY_C:
          iZ0 = 3;
          iZg = 350;
          iZc = 5;
          dAExponent = 0.120;
          break;
        case TERRAINCATEGORY_D:
          iZ0 = 5;
          iZg = 400;
          iZc = 10;
          dAExponent = 0.150;
          break;
        default:
          break;
    }//switch

    double dtemp = static_cast<double>((_iHeight - iZ0)) / static_cast<double>((iZg - iZc));
    dReturn = 1.36 * pow(dtemp, dAExponent);

    return dReturn;
  }//TerrainRoughness()
}//namespace WindAction_library