#ifndef MICROSCOPEOBJECT_H
#define MICROSCOPEOBJECT_H

#include "ray.h"
#include "specmat.h"
/*!
 * \brief Třída pro obecný objekt v mikroskopu
 * \author Daniel Karásek, xkaras38
 * \details
 * Třída reprezentující obecný objekt v mikroskopu. Jedná se vlastně o určitou formu "virtuální třídy", která ale obsahuje
 * základní nic nedělající implementace (lze tedy využít i bez subclassovaní, ale vlastně nic nedělá).
 */
class MicroscopeObject{
protected:
  SpecMat m_transMat{SpecMat::createDiagMat()}; /*!< transformacni matice */
  double m_z; /*!< vyska objektu v mikroskopu */

public:
  /*!
   * \brief MicroscopeObject
   * \param z vyska v mikroskopu
   */
  MicroscopeObject(double z):m_z{z}{}

  /*!
   * \brief applyOptics je predpis pro aplikaci optiky obecneho objektu, implementace v teto třídě aplikuje diagonalni matici(takze nic)
   * \param rayOut modifikovany paprsek
   * \return kopize zmodifikovaného paprsku
   */
  virtual Ray applyOptics(Ray &rayOut);

  double getZ(){return m_z;}
  void updateZ(double z){m_z = z;}
};


#endif
