#include "specmat.h"
#include "microscopeobject.h"
#include "ray.h"
#include "typedefs.h"
#include "detector.h"




Detector::Detector(double absSize, int relSize)
:MicroscopeObject(0), m_absSize{absSize}, m_relSize{relSize}{
  initMap();
  std::cout << "bitmap size: "<< static_cast<int>(m_bitmap.size()) <<"\n";
}


Ray Detector::applyOptics(Ray &rayOut){
  Position pos = rayOut.getPosition();

  //std::cout << "\n"<<  m_relSize << "\n"  ;



  //we need to translate position into int in range of 0-detector size, so we
  //can index bitarray correcly hence following 6 lines
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
  m_bitmap[y_discrete*m_relSize + x_discrete] += rayOut.getIntensity();
  m_bitmap[y_discrete*m_relSize + x_discrete] = std::max(0.0,std::min(m_bitmap[y_discrete*m_relSize + x_discrete], 1.));
  return rayOut;
}


void Detector::resetMap(){
  for (double& val : m_bitmap){
    val = 0;
  }
}

void Detector::initMap(){
  m_bitmap.clear();
  m_bitmap.resize(m_relSize*m_relSize);
}

void Detector::printBitmap()
{
  for (int rrr=m_relSize-1;rrr>=0;rrr--){
    for (int ccc=0;ccc<m_relSize;ccc++){
      std::cout << m_bitmap[rrr*m_relSize +ccc] << " ";
    }
    std::cout << std::endl;
  }
}
