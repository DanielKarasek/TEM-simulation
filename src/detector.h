#ifndef DETECTOR_H_INCLUDED
#define DETECTOR_H_INCLUDED

#include "specmat.h"
#include "microscopeobject.h"
#include "ray.h"
#include "typedefs.h"

#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
/*!
 * \brief Třída pro detektor
 * \author Daniel Karásek, xkaras38
 * \details
 * Třída reprezentující detektor.
 */
class Detector : public MicroscopeObject{
private:
  Bitmap m_bitmap; /*!< bitmapa výsledných hodnot */
  double m_absSize; /*!< Celková velikost vzorku */
  int m_relSize; /*!< rozliseni znacici mnozstvi ctvercu v ose (relSize x relSize) */

public:
  /*!
   * \brief Detektor
   * \param absSize velikost detektoru
   * \param relSize rozliseni(celkove rozliseni je pak relSize x relSize)
   */
  Detector(double absSize, int relSize);

  /*!
   * \brief applyOptics aplikuje optiku, primárním úkolem je zjištění zobrazení paprsku na detektor
   * \param rayOut modifikovaný paprsek
   * \return kopie zmodifikovaného paprsku
   */
  virtual Ray applyOptics(Ray &rayOut);

  /*!
   * \brief resetMap resetuje bitmapu na nuly
   */
  void resetMap();
  /*!
   * \brief resetMap vytvari novou prazdnou bitmapu
   */
  void initMap();

  void changeAbsSize(double absSize){m_absSize = absSize;}
  void changeRelSize(int relSize){m_relSize = relSize;initMap();}

  Bitmap &getResultRef(){return m_bitmap;}
  double getAbsSize(){return m_absSize;}
  int getRelSize(){return m_relSize;}

  void printBitmap();

};



#endif // DETECTOR_H_INCLUDED
