#ifndef LENS_H_INCLUDED
#define LENS_H_INCLUDED

#include "specmat.h"
#include "microscopeobject.h"
#include "ray.h"
#include "typedefs.h"

#include <cmath>
#include <vector>
#include <utility>
/*!
 * \brief Třída pro čočku
 * \author Daniel Karásek, xkaras38
 * \details
 * Třída reprezentující čočku.
 */
class Lens : public MicroscopeObject{
private:
  double m_xDeviation;
  double m_yDeviation;

public:
  /*!
   * \brief Lens
   * \param z výška čočky v mikroskopu
   * \param focalLength optická vzdálenost čočky
   * \param xDeviation posunuti čočky v x
   * \param yDeviation posunutí čočky v y
   */
  Lens(double z, double focalLength, double xDeviation, double yDeviation);
  /*!
   * \brief applyOptics funkce aplikující transformaci čočky na paprsek(hlavně změna uhlů paprsku)
   * \param rayOut modifikovaný paprsek
   * \return kopie zmodifikovaného paprsku
   */
  virtual Ray applyOptics(Ray &rayOut);


  void updatefocalLength(double focalLength);
  void updateXDeviation(double xDeviation){m_xDeviation = xDeviation;}
  void updateYDeviation(double yDeviation){m_yDeviation = yDeviation;}

  double getfocalLength(){return -1/m_transMat(1,0);}
  double getXDeviation(){return m_xDeviation;}
  double getYDeviation(){return m_yDeviation;}


};



#endif // LENS_H_INCLUDED
