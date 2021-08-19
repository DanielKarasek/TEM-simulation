#include "specmat.h"
#include "microscopeobject.h"
#include "ray.h"
#include "typedefs.h"
#include "lens.h"


Lens::Lens(double z, double focalLength, double xDeviation, double yDeviation)
:MicroscopeObject(z), m_xDeviation{xDeviation}, m_yDeviation(yDeviation)
{
  m_transMat.change2LensMat(focalLength);
}


Ray Lens::applyOptics(Ray &rayOut){
  return rayOut.applyTransformWithTranslation(m_transMat, m_xDeviation, m_yDeviation);
}


void Lens::updatefocalLength(double focalLength){
  m_transMat.change2LensMat(focalLength);
}

