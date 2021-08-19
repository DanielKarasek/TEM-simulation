#ifndef SPECIMEN_H
#define SPECIMEN_H

#include "specmat.h"
#include "microscopeobject.h"
#include "ray.h"
#include "typedefs.h"

#include <cmath>
#include <vector>
#include <utility>
#include <fstream>

using std::string;
/*!
 * \brief Třída pro vzorek
 * \author Daniel Karásek, xkaras38
 * \details
 * Třída reprezentující vzorek.
 */
class Specimen : public MicroscopeObject{
private:
  Bitmap m_bitmap; /*!< bitmapa vzorku */
  double m_absSize; /*!< Celková velikost vzorku */
  double m_rotation;
  SpecMat m_rotMat;

  int m_relSize; /*!< velikost ctverce reprezentujici bod bitmapy */



public:
  /*!
   * \brief Specimen
   * \param z výška ve které se vzorek nachází
   * \param bitmap bitmapa propustnosti
   * \param absSize velikost vzorku
   */
  Specimen(double z, Bitmap bitmap, double absSize);
  /*!
   * \brief Specimen
   * \param z výška ve které se vzorek nachází
   * \param bitmap bitmapa propustnosti
   * \param absSize velikost vzorku
   * \param rotation rotace v radianech
   */
  Specimen(double z, Bitmap bitmap, double absSize, double rotation);

  /*!
   * \brief applyOptics provede "optickou funkci" (modifikace intenzit) na paprsek
   * \param rayOut paprsek, ktery bude modifikovan
   * \return kopie zmodifikovaneho paprsku
   */
  virtual Ray applyOptics(Ray &rayOut);

  double getRotation(){return m_rotation;}
  double getAbsSize(){return m_absSize;}
  Bitmap &getBitmap(){return m_bitmap;}

  void updateRotation(double newRotation);
  void updateAbsSize(double newAbsSize){m_absSize = newAbsSize;}
  /*!
   * \brief loadBitmap nacte bitmapu ze souboru, v pripade chyby, nice nemeni
   * \param filename jmeno souboru
   */
  void loadBitmap(string filename);
  void loadBitmap(std::ifstream &inFile, int n);

};


#endif
