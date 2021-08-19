#include "microscopeobject.h"
#include "specimen.h"
#include "typedefs.h"
#include "detector.h"
#include "microscope.h"


using std::string;


Microscope::Microscope(Bitmap specBitmap, Bitmap intensityBitmap, double height,
              double specimenSize, double detectorAbsSize, int detectorRelSize)
:m_totalHeight{height}, m_intensityBitmap{intensityBitmap}
{
  std::unique_ptr<MicroscopeObject> firstLens(new Lens(height, 0, 0 ,0));
  std::unique_ptr<MicroscopeObject> specimen(new Specimen(height/2, specBitmap, specimenSize));
  std::unique_ptr<MicroscopeObject> detector(new Detector(detectorAbsSize, detectorRelSize));
  m_objects.push_back(std::move(firstLens));
  m_objects.push_back(std::move(specimen));
  m_objects.push_back(std::move(detector));
}



void Microscope::run(){
  auto objectCount = m_objects.size();
  Detector *det= dynamic_cast<Detector*>(m_objects[objectCount-1].get());
  det->resetMap();
  runRaytrace();
  runGetDigest();
}



void Microscope::runGetDigest(){
  // clear because there could be rays from last run
  m_keyRays.clear();
  m_heights.clear();
  std::vector<Ray> tmpRays;
  // set key rays origin
  tmpRays.push_back({-magic_base_diameter/2,0,0,0,0});
  tmpRays.push_back({0,magic_base_diameter/2,0,0,0});
  tmpRays.push_back({magic_base_diameter/2,0,0,0,0});
  tmpRays.push_back({0,-magic_base_diameter/2,0,0,0});
  // tmp transformation mat used for moving in z direction(z is height)
  SpecMat distMat = SpecMat::createDistMat(0);


  // bool used to check if rays crossed
  bool polarity = tmpRays[0].getPosition().first<tmpRays[2].getPosition().first;




  auto objectCount = m_objects.size();
  for (std::vector<std::unique_ptr<MicroscopeObject>>::size_type objectNum = 1; objectNum<objectCount; objectNum++){
    //push all key rays and heigh into result vectors
    m_keyRays.push_back(tmpRays);
    m_heights.push_back(m_objects[objectNum-1]->getZ());
    //move to next object
    for (Ray &ray: tmpRays){
      distMat.change2DistMat(m_objects[objectNum-1]->getZ()-m_objects[objectNum]->getZ());
      ray.applyTransformation(distMat);
    }
    // if crossing happened, find crossing point and add it to result vectors as well
    if ((tmpRays[0].getPosition().first<tmpRays[2].getPosition().first) == !polarity){
      auto rayZCoordPair = trackBack(tmpRays, 0);

//      std::cout << "\npos: "<< rayZCoordPair.second <<"\n";
      m_keyRays.push_back(rayZCoordPair.first);
      m_heights.push_back(m_objects[objectNum]->getZ() + rayZCoordPair.second);

      polarity = tmpRays[0].getPosition().first<tmpRays[2].getPosition().first;
    }
    // apply object optics
    for (Ray &ray: tmpRays){
      m_objects[objectNum].get()->applyOptics(ray);
    }
  }

  // push the last key ray quadruple to key rays result vector
  m_keyRays.push_back(tmpRays);

//  for (auto &rayVect : m_keyRays){
//    std::cout << "______________________________\n";
//    for (Ray &ray: rayVect){
//      std::cout << ray;
//    }
//  }
}


std::pair <std::vector<Ray>, double> Microscope::trackBack(const std::vector<Ray> &rays, int majorAxisNum){
  //get positions and angles of crossing rays
  Position firstPosition = rays[majorAxisNum].getPosition();
  Position secondPosition = rays[2+majorAxisNum].getPosition();
  Position firstAngles = rays[majorAxisNum].getAngles();
  Position secondAngles = rays[2+majorAxisNum].getAngles();

  Position tmpAngle;
  Position tmpPos;

  std::vector<Ray> newRays;
  newRays.clear();
  //solve equation based on major axis (x axis or y axis) to gain how far up crossing happened
  double z = 0;
  if(majorAxisNum==0){
    z = (firstPosition.first-secondPosition.first) / (firstAngles.first-secondAngles.first);
  }
  else if (majorAxisNum==1){
    z = (firstPosition.second-secondPosition.second) / (firstAngles.second-secondAngles.second);
  }
  else{
    //TODO handle error
    return {newRays, -1};
  }


  //calculate rays values based on crossing point
  for (const Ray &ray: rays){
    tmpAngle = ray.getAngles();
    tmpPos = ray.getPosition();
    newRays.push_back({tmpPos.first-(tmpAngle.first*z),tmpPos.second-(tmpAngle.second*z), tmpAngle.first, tmpAngle.second,0});
  }

  return {newRays, z};
}




int Microscope::getLensCount(){
  return static_cast<int>(m_objects.size())-3;
}

void Microscope::addLens(double z, double opticalPower, double xDev, double yDev){
  std::unique_ptr<MicroscopeObject> newLens(new Lens(z,opticalPower,xDev,yDev));

  if (z >= m_totalHeight){
    //TODO ERROR HANDLE
    return;
  }

  for (auto [counter, it] = std::tuple{0,m_objects.begin()}; it<m_objects.end(); it++, counter++ ){
    if ((*it).get()->getZ() < z){

      m_objects.insert(it, std::move(newLens));
      // if point of insertion is before specime update specimen position
      if (counter <= m_specimenPos){
        m_specimenPos++;
      }
      break;
    }
  }
}

void Microscope::updateLens(int i, double z, double focalLength, double xDev, double yDev){
  // count position given first implicit lens and if specimen is behind or after the lense
  if (i+1>=m_specimenPos){
    i+=2;
  }
  else{
    i++;
  }
  // error states that shouldn't be happening in our project but are here so it can be extended
  if (z >= m_totalHeight){
    //TODO ERROR HANDLE
  }
  if (z <= 0){
    //TODO ERROR HANDLE
  }


  Lens *lens = dynamic_cast<Lens*>(m_objects[i].get());

  double oldZ = lens->getZ();

  lens->updatefocalLength(focalLength);
  lens->updateXDeviation(xDev);
  lens->updateYDeviation(yDev);
  lens->updateZ(z);
  // Following moves all items between lens new position and its current position by one
  // in direction so objects in optical objects vector are still sorted by height
  // also updates specimen position if needed
  if (oldZ < z){
    auto it = m_objects.begin();
    int newPos = 0;
    for (;it<m_objects.begin()+i; it++, newPos++){
      if (it->get()->getZ() < z){
        break;
      }
    }
    std::rotate(it, m_objects.begin()+i, m_objects.begin()+i+1);
    if (i>m_specimenPos && newPos <= m_specimenPos){
      m_specimenPos++;
    }
  }
  else if(oldZ > z){
    auto it = m_objects.begin()+i+1;
    int newPos = i;
    for (;it<m_objects.end()-1; it++, newPos++){
      if (it->get()->getZ() <= z){
          break;
      }
    }
    if (i != newPos){
      std::rotate(m_objects.begin()+i, m_objects.begin()+i+1, it);
      if (i<m_specimenPos && newPos >= m_specimenPos){
        m_specimenPos--;
      }
    }
  }
}


void Microscope::removeLens(int i){
  // count position given first implicit lens and if specimen is behind or after the lense
  if (i+1>=m_specimenPos){
    i+=2;
  }
  else{
    i++;
    m_specimenPos--;
  }
  //posunout m_specimenPos
  m_objects.erase(m_objects.begin()+i);
}

LensInfo Microscope::getLens(int i){
  // count position given first implicit lens and if specimen is behind or after the lense
  if (i+1>=m_specimenPos){
    i+=2;
  }
  else{
    i++;
  }
  Lens *lens = dynamic_cast<Lens *> (m_objects[i].get());
  return {lens->getYDeviation(), lens->getXDeviation(), lens->getfocalLength(), lens->getZ()};
}

std::vector<LensInfo> Microscope::getLensVector(){
  std::vector<LensInfo> vect={};
  for(int i=0; i<getLensCount(); i++){
    vect.push_back(getLens(i));
  }

  return vect;
}

void Microscope::updateSpecimen(double z, double rotation, double absSize){

  if (z >= m_totalHeight){
      return;
  }

  Specimen *specimen = dynamic_cast<Specimen*>(m_objects[m_specimenPos].get());

  double oldZ = specimen->getZ();

  specimen->updateAbsSize(absSize);
  specimen->updateRotation(rotation);
  specimen->updateZ(z);
  //rotates all elements between spec old position and its new position by one
  //so objects are still sorted by its height position
  if (oldZ < z){
    auto it = m_objects.begin();
    int newPos = 0;
    for (;it<m_objects.begin()+m_specimenPos; it++, newPos++){
      if (it->get()->getZ() < z){
        break;
      }
    }
    std::rotate(it, m_objects.begin()+m_specimenPos, m_objects.begin()+m_specimenPos+1);
    m_specimenPos = newPos;
  }
  else if(oldZ > z){
    auto it = m_objects.begin()+m_specimenPos+1;
    int newPos = m_specimenPos;
    for (;it<m_objects.end()-1; it++, newPos++){
      if (it->get()->getZ() <= z){
        break;
      }
    }
    if (newPos != m_specimenPos){
      std::rotate(m_objects.begin()+m_specimenPos, m_objects.begin()+m_specimenPos+1, it);
      m_specimenPos = newPos;
    }
  }

}


SpecimenInfo Microscope::getSpecimen(){
  Specimen *spec = dynamic_cast<Specimen *>(m_objects[m_specimenPos].get());
  return {spec->getAbsSize(), spec->getRotation(), spec->getZ(), spec->getBitmap()};
}



void Microscope::updateDetector(double absSize, int relSize, double relZ){
  Detector *det = dynamic_cast<Detector *>(m_objects[m_objects.size()-1].get());
  //double relZ = m_totalHeight-zFromSrc;

  // moves all objects closer to detector or further from detector
  if (relZ < 0){
    for (auto it = m_objects.begin(); it < m_objects.end()-1; it++){
      it->get()->updateZ(it->get()->getZ()-relZ);
    }
  }
  else if(relZ > 0){
    if ((m_objects.end()-2)->get()->getZ() <= relZ){
      //TODO error handle musel bych mazat objekty
      return;
    }

    for (auto it = m_objects.begin(); it < m_objects.end()-1; it++){
      it->get()->updateZ(it->get()->getZ()-relZ);
    }
  }

  this->m_totalHeight = m_totalHeight-relZ;

  det->changeAbsSize(absSize);
  det->changeRelSize(relSize);
}


DetectorInfo Microscope::getDetector(){
  Detector *det = dynamic_cast<Detector *>(m_objects[m_objects.size()-1].get());
  return {det->getAbsSize(), det->getRelSize(), det->getResultRef()};
}



//void Microscope::updateMicroscopeHeight(double newHeight){
//  if (newHeight < m_objects[m_specimenPos].get()->getZ()){
//    //TODO ERROR HANDLE
//  }
//  dynamic_cast<Lens*>(m_objects[0].get())->updateZ(newHeight);

//  for (auto it = m_objects.begin()+1; it!=m_objects.end(); it++){
//    if ((*it).get()->getZ() > newHeight){
//      m_objects.erase(m_objects.begin()+1);
//    }
//    else {
//      break;
//    }
//  }
//}




const std::vector<std::vector<Ray>>& Microscope::getKeyRays(){
  return m_keyRays;
}


std::vector<double> Microscope::getHeights(){
  return m_heights;
}


double Microscope::getLowestPreDetector(){
  return m_objects[m_objects.size()-2].get()->getZ() ;
}

void Microscope::loadFromFile(string filename){
  std::ifstream inFile;

  inFile.open (filename, std::ios::in);
  if (!inFile.good()){
    // TODO ERROR HANDLE
    return;
  }

  int bitmapSize;
  inFile >> m_totalHeight;
  inFile >> bitmapSize;
  m_intensityBitmap.clear();
  double val;
  for (int i=0;i<bitmapSize;i++){
    inFile >> val;
    m_intensityBitmap.push_back(val);
  }
  double absSizeDet;
  int relSizeDet;
  inFile >> absSizeDet;
  inFile >> relSizeDet;
  updateDetector(absSizeDet,relSizeDet,0);
  double heightSpec, absSizeSpec, rotationRadians;
  inFile >> heightSpec;
  inFile >> absSizeSpec;
  inFile >> rotationRadians;
  updateSpecimen(heightSpec, rotationRadians, absSizeSpec);
  inFile >> bitmapSize;
  Specimen *specimen = dynamic_cast<Specimen*>(m_objects[m_specimenPos].get());
  specimen->loadBitmap(inFile, bitmapSize);

  int cnt = getLensCount();
  for (int i=0; i < cnt;i++){
    removeLens(0);
  }
  int totalLenses;
  inFile >> totalLenses;
  double focalLength, heightLens, xDev, yDev;
  for (int i=0; i<totalLenses; i++){
    inFile >> focalLength;
    inFile >> heightLens;
    inFile >> xDev;
    inFile >> yDev;
    addLens(heightLens, focalLength, xDev, yDev);
  }

  inFile.close();
}

void Microscope::save2file(string filename){
  std::ofstream outFile;
  outFile.open (filename, std::ios::out);
  if (!outFile.good()){
    // TODO ERROR HANDLE
    return;
  }

  outFile << m_totalHeight << std::endl;
  outFile << m_intensityBitmap.size() << std::endl;
  for (auto it=m_intensityBitmap.begin();it<m_intensityBitmap.end();it++){
    outFile << *it << " ";
  }

  DetectorInfo detInfo = getDetector();
  outFile << detInfo.absSize << " " << detInfo.relSize << std::endl;

  SpecimenInfo specInfo = getSpecimen();
  outFile << specInfo.height << " " << specInfo.absSize << " " << specInfo.rotationRadians << std::endl;
  outFile << specInfo.bitmap.size() << std::endl;
  outFile << std::endl;

  for (auto it=specInfo.bitmap.begin();it<specInfo.bitmap.end();it++){
    outFile << *it << " ";
  }

  LensInfo lensInfo;
  outFile << getLensCount()<<std::endl;
  for (int i=0; i<getLensCount(); i++){
     lensInfo = getLens(i);
     outFile << lensInfo.focalLength << " " << lensInfo.height
             << " " << lensInfo.xDev << " " << lensInfo.yDev << std::endl;
  }


  outFile.close();
}


void Microscope::loadRayBitmap(string filename){
  std::ifstream inFile;
  inFile.open (filename, std::ios::in);
  if (!inFile.good()){
    // TODO ERROR HANDLE
    return;
  }
  double tmp;
  m_intensityBitmap.clear();
  while(inFile>>tmp){
    tmp = std::max(0.0,std::min(tmp, 1.));
    m_intensityBitmap.push_back(tmp);
  }
  inFile.close();
}

void Microscope::loadSpecimenBitmap(string filename){
  Specimen *spec = dynamic_cast<Specimen *>(m_objects[m_specimenPos].get());
  spec->loadBitmap(filename);
}



void Microscope::runRaytrace(){
  std::vector<Ray> rays;
  rays.resize(m_intensityBitmap.size());

  SpecMat distMat = SpecMat::createDistMat(0);

  int rayIndex;
  int rayCountSquared = static_cast<int>(std::sqrt(m_intensityBitmap.size()));
  double mu = (static_cast<double>(rayCountSquared-1)/2);
  double scaleCoef = magic_base_diameter/mu;

  //sets rays into squares of different diameters with mean 0 and max radius
  //is max radius of beam of rays
  for (int rrr=0; rrr<rayCountSquared; rrr++){
    for (int ccc=0; ccc<rayCountSquared; ccc++){
      rayIndex = rrr*rayCountSquared+ccc;
      rays[rayIndex].setPosition((ccc-mu)*scaleCoef, (rrr-mu)*scaleCoef);
      rays[rayIndex].setIntensity(m_intensityBitmap[rayIndex]);
    }
  }

  // circlefies from squares to circles
  circlefy(rays);
//  for (Ray &ray: rays){
//    //std::cout << ray ;
//  }

  //simulets run for all rays -> apply objec optics->move to next object and again and again until all end of microscope is reached
  auto objectCount = m_objects.size();
  for (Ray &ray: rays){
    for (std::vector<std::unique_ptr<MicroscopeObject>>::size_type objectNum = 1; objectNum<objectCount; objectNum++){
      distMat.change2DistMat(m_objects[objectNum-1]->getZ()-m_objects[objectNum]->getZ());
      ray.applyTransformation(distMat);
      m_objects[objectNum].get()->applyOptics(ray);
    }
  }
  //dynamic_cast<Detector*>(m_objects[objectCount-1].get())->printBitmap();
}



void Microscope::circlefy(std::vector<Ray> &rays){
  Position oldPos;
  Position newPos;
  double maximum;
  for (Ray &ray : rays){
    oldPos = ray.getPosition();
    maximum = std::max(std::abs(oldPos.first), std::abs(oldPos.second));
    oldPos.first /= maximum;
    oldPos.second /= maximum;
    //here we use formula found on web to circlefy square
    newPos.first = oldPos.first*std::sqrt(1-oldPos.second*oldPos.second/2);
    newPos.second = oldPos.second*std::sqrt(1-oldPos.first*oldPos.first/2);
    //scale them back and then scale them to perimeter of whole beam
    newPos.first *= maximum/2;
    newPos.second *= maximum/2;
    ray.setPosition(newPos.first, newPos.second);
  }
}



