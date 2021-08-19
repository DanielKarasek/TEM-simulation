#include "specmat.h"





SpecMat SpecMat::createLensMat(double focalLength){
  return {1,0,-1/focalLength,1};
}
SpecMat SpecMat::createDistMat(double len){
  return {1,len,0,1};
}
SpecMat SpecMat::createRayMat(double x, double y, double theta_x, double theta_y){
  return {x,y,theta_x,theta_y};
}

SpecMat SpecMat::createDiagMat(){
  return {1,0,0,1};
}

SpecMat SpecMat::createRotMat(double rotation){
  return {std::cos(rotation),-std::sin(rotation),
          std::sin(rotation),std::cos(rotation)};
}

void SpecMat::change2LensMat(double focalLength){
  this->m_matrix.assign({1,0,-1/focalLength,1});
}
void SpecMat::change2DistMat(double len){
  this->m_matrix.assign({1,len,0,1});
}
void SpecMat::change2RayMat(double x, double y, double theta_x, double theta_y){
  this->m_matrix.assign({x,y,theta_x,theta_y});
}

void SpecMat::change2DiagMat(){
  this->m_matrix.assign({1,0,0,1});
}

void SpecMat::change2RotMat(double rotation){
  this->m_matrix.assign({std::cos(rotation),-std::sin(rotation),
                         std::sin(rotation),std::cos(rotation)});
}





SpecMat SpecMat::applyTransformation(const SpecMat transMat){
  SpecMat &modifiedMat = *this;
  modifiedMat = transMat*modifiedMat;
  return modifiedMat;
}


SpecMat operator*(const SpecMat &firstMat,const SpecMat &secondMat){
  SpecMat tmpMat{0,0,0,0};
  //matrix multiplication implementation
  for(int rrr=0; rrr<2; rrr++){
    for (int ccc=0; ccc<2; ccc++){
      for (int addIndex=0; addIndex<2; addIndex++){
        tmpMat(rrr, ccc) += firstMat(rrr, addIndex)*secondMat(addIndex, ccc);
      }
    }
  }
  return tmpMat;
}

Position operator*(const SpecMat &transMat, const Position &pos){
  Position tmpPos{};
  //matrix vector multiplication
  tmpPos.first = transMat(0, 0)*pos.first + transMat(0, 1)*pos.second;
  tmpPos.second = transMat(1, 0)*pos.first + transMat(1, 1)*pos.second;
  return tmpPos;
}

SpecMat& SpecMat::operator=(const SpecMat& srcMat){
  if (this == &srcMat)
    return *this;
  this->m_matrix = srcMat.m_matrix;
  return *this;
}



double& SpecMat::operator()(int index1, int index2){
  return this->m_matrix[index1*2+index2];
}

const double& SpecMat::operator()(int index1, int index2)const{
  return this->m_matrix[index1*2+index2];
}




std::ostream& operator<<(std::ostream &out, const SpecMat &mat){
  out << "[";
  for(int rrr=0; rrr<2; rrr++){
    for (int ccc=0; ccc<2; ccc++){
      out << mat.m_matrix[rrr*2+ccc] << " ";
    }
    if (rrr == 1)
      out << "]";
    out << "\n";

  }
  return out;
}
