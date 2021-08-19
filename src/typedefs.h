#ifndef TYPEDEFS_H_INCLUDED
#define TYPEDEFS_H_INCLUDED

#include <utility>
#include <vector>
#include <memory>
/*!
 *\author Daniel Karásek, xkaras38
 */

class Ray;
/*!
 * \brief pozice x,y (nebo uhel x,y)
 */
typedef std::pair<double, double> Position;

/*!
 * \brief vektor ktery se reprezenuje jako 2D bitmapa
 */
typedef std::vector<double> Bitmap;


/*!
 * \brief vektor ktery se reprezenuje jako 2D bitmapa
 */
typedef std::vector<std::vector<Ray>> RaysVectDigest;



/*!
 * \brief metadata cocky
 */
typedef struct TLensInfo{
  double yDev; /*!< odchylka cocky v y ose */
  double xDev; /*!< odchylka cocky v x ose*/
  double focalLength; /*!< opticka mohutnost cocky */
  double height; /*!< vyska cocky v mikroskopu */
}LensInfo;

/*!
 * \brief metadata vzorku
 */
typedef struct TSpecimenInfo{
  double absSize;  /*!< velikost vzorku */
  double rotationRadians; /*!< rotace vzorku v radianech */
  double height; /*!< výšková pozice vzorku v mikroskopu */
  Bitmap &bitmap; /*!< reference na bitmapu vzorku */

}SpecimenInfo;


typedef struct TDetectorInfo{
  double absSize; /*!< velikost detektoru */
  int relSize; /*!< rozliseni na detektoru (rozliseni je relSize x relSize) */
  Bitmap &bitmap; /*!< reference na zobrazeni (bitmapu) na detektoru */
}DetectorInfo;
#endif
