#ifndef SPECMAT_H
#define SPECMAT_H

#include "typedefs.h"

#include <iostream>
#include <vector>
#include <cmath>

/*!
 * \brief Třída obalující matici 2x2
 * \author Daniel Karásek, xkaras38
 * \details
 * Tato třída obaluje vektor reprezentující matici 2x2. Dále umožňuje tyto matice násobit mezi sebou pomocí operatoru *
 * ale i s 'vektorem' typu <a href=classPosition.html>Position</a>. Obsahuje statické funkce na tvorbu
 * matice SpecMat konkrétního typu(paprsku,rotační,transformační pro paraxiální optiku na dálku/čočku etc.), ale i členské funkce
 * modifikující existující matici na matici daného typu z danými parametry.
 * Dále taky funkci apply transformation, která na matici aplikuje transformaci
 * a vrátí její kopii.
 */
class SpecMat{
private:
  std::vector<double> m_matrix;

public:
  /*!
   * \brief SpecMat vytvori matici s hodnotamy a,b,c,d
   * \param a
   * \param b
   * \param c
   * \param d
   */
  SpecMat(double a, double b, double c, double d)
  :m_matrix{a,b,c,d}{}
  /*!
   * \brief SpecMat vytvori nulovou matici
   */
  SpecMat()
  :m_matrix{0,0,0,0}{}

  /*!
   * \brief SpecMat copy construktor
   */
  SpecMat(const SpecMat& specMat)
  :m_matrix{specMat.m_matrix}{}

  static SpecMat createDiagMat();
  static SpecMat createLensMat(double focalLength);
  static SpecMat createDistMat(double len);
  static SpecMat createRayMat(double x, double y, double theta_x, double theta_y);
  static SpecMat createRotMat(double rotation);




  void change2DiagMat();
  void change2LensMat(double focalLength);
  void change2DistMat(double len);
  void change2RayMat(double x, double y, double theta_x, double theta_y);
  void change2RotMat(double rotation);
  SpecMat applyTransformation(const SpecMat transMat);

  friend SpecMat operator*(const SpecMat &firstMat, const SpecMat &secondMat);
  friend Position operator*(const SpecMat &transMat, const Position &position);

  SpecMat& operator=(const SpecMat& srcMat);

  double& operator()(int index1, int index2);
  const double& operator()(int index1, int index2) const;

  friend std::ostream& operator<<(std::ostream &out, const SpecMat &mat);
};

#endif // SPECMAT_H
