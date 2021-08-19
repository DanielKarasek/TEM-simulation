#include "specmat.h"
#include "ray.h"
#include "typedefs.h"



void Ray::modifyIntensity(double some_value){
  m_intensity *= some_value;
}


Ray Ray::applyTransformation(SpecMat &transMat){
  m_ray = transMat * m_ray;
  return *this;
}

Ray Ray::applyTransformWithTranslation(SpecMat &transMat, double xTranslation, double yTranslation){
  m_ray(0, 0) -= xTranslation;
  m_ray(0, 1) -= yTranslation;

  m_ray = transMat * m_ray;

  m_ray(0, 0) += xTranslation;
  m_ray(0, 1) += yTranslation;

  return *this;

}

std::ostream& operator<<(std::ostream &out, const Ray &ray){
  out << "Ray with intensity " <<  ray.m_intensity << " has matrix:\n";
  out << ray.m_ray << "\n";
  return out;
}

