#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include "specmat.h"
#include "typedefs.h"

/*!
 * \brief Třída pro paprsek
 * \author Daniel Karásek, xkaras38
 * \details
 * Třída reprezentující paprsek. Obsahuje funkce nejen pro získání a modifikaci hodnoti, ale i pro aplikaci transformací
 */
class Ray{
private:
  SpecMat m_ray;
  double m_intensity;

public:
  /*!
   * \brief Vytvoří vzorek s hodnotamy
   * \param x pozice
   * \param y pozice
   * \param theta_x uhel
   * \param theta_y uhel
   * \param intensity
   */
  Ray(double x, double y, double theta_x, double theta_y, double intensity):
    m_ray{SpecMat::createRayMat(x, y, theta_x, theta_y)}, m_intensity{intensity}{}
  /*!
   * \brief Vytvoří nulový paprsek
   */
  Ray():Ray(0,0,0,0,0){}

  /*!
   * \brief aplikuje transformaci na paprsek
   * \param transMat transformační matice
   * \return kopie změněného paprsku
   */
  Ray applyTransformation(SpecMat &transMat);

  /*!
   * \brief aplikuje transformaci před kterou vykoná translaci jedním směrem a po ní zpět(simulace translace čoček)
   * \param transMat transformační matice
   * \param xTranslation
   * \param yTranslation
   * \return kopie změněného paprsku
   */
  Ray applyTransformWithTranslation(SpecMat &transMat, double xTranslation, double yTranslation);

  /*!
   * \brief Modifikiju intenzitu pomocí hodnoty propustnosti pocházející z vzorku
   * \param some_value propustnost
   */
  void modifyIntensity(double some_value);

  void setPosition(double x, double y){m_ray(0,0)=x;m_ray(0,1)=y;}
  void setIntensity(double intensity){m_intensity=intensity;}

  Position getPosition()const{return {m_ray(0,0), m_ray(0,1)};}
  Position getPosition(){return {m_ray(0,0), m_ray(0,1)};}
  Position getAngles()const{return {m_ray(1,0), m_ray(1,1)};}
  Position getAngles(){return {m_ray(1,0), m_ray(1,1)};}

  double getIntensity()const{return m_intensity;}

  friend std::ostream& operator<<(std::ostream &out, const Ray &ray);

};

#endif // RAY_H_INCLUDED
