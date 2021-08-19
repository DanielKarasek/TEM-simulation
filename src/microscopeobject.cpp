#include "ray.h"
#include "specmat.h"
#include "microscopeobject.h"

Ray MicroscopeObject::applyOptics(Ray &rayOut){
  return rayOut.applyTransformation(m_transMat);
}
