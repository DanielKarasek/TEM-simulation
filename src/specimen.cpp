#include "specmat.h"
#include "microscopeobject.h"
#include "ray.h"
#include "typedefs.h"
#include "specimen.h"

Specimen::Specimen(double z, Bitmap bitmap, double absSize)
:Specimen(z, bitmap, absSize, 0)
{}

Specimen::Specimen(double z, Bitmap bitmap, double absSize, double rotation)
:MicroscopeObject(z), m_bitmap{bitmap},
 m_absSize{absSize},m_rotation{rotation},
 m_rotMat{SpecMat::createRotMat(-rotation)},
 m_relSize{static_cast<int>(std::sqrt(m_bitmap.size()))}
{}


Ray Specimen::applyOptics(Ray &rayOut){

  //std::cout << "\n"<<  m_relSize<< "\n"  ;
  Position pos = rayOut.getPosition();

  //we need to translate position into int in range of 0-specimen size + rotated correctly
  //so we can index our specimen bitmap correctly - hence the following 7 lines
  pos = m_rotMat*pos;
  //std::cout << pos.first << ", " << pos.second << std::endl;
  pos.first = (pos.first*m_relSize)/m_absSize;
  pos.second = (pos.second*m_relSize)/m_absSize;

  //std::cout << pos.first << ", " << pos.second << std::endl;
  pos.first += (static_cast<double>(m_relSize)/2);
  pos.second += (static_cast<double>(m_relSize)/2);

  //std::cout << pos.first << ", " << pos.second << std::endl;

  int x_discrete = std::floor(pos.first);
  int y_discrete = std::floor(pos.second);
  if (x_discrete < 0 || x_discrete >= m_relSize || y_discrete < 0 || y_discrete >= m_relSize){
    return rayOut;
  }
  rayOut.modifyIntensity(m_bitmap[y_discrete*m_relSize + x_discrete]);
  return rayOut;
}


void Specimen::updateRotation(double newRotation){
  m_rotation = newRotation;
  m_rotMat.change2RotMat(m_rotation);
}


void Specimen::loadBitmap(string filename){
  std::ifstream inFile;
  inFile.open (filename, std::ios::in);
  if (!inFile.good()){
  // TODO ERROR HANDLE
    return;
  }
  double tmp;
  m_bitmap.clear();
  while(inFile>>tmp){
    tmp = std::max(0.0,std::min(tmp, 1.));
    m_bitmap.push_back(tmp);
  }
  m_relSize = static_cast<int>(std::sqrt(m_bitmap.size()));
  inFile.close();
}

void Specimen::loadBitmap(std::ifstream &inFile, int n){
  double tmp;
  m_bitmap.clear();
  for(int i=0; i<n; i++){
    inFile >> tmp;
    tmp = std::max(0.0,std::min(tmp, 1.));
    m_bitmap.push_back(tmp);
  }
}
