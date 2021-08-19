#include "presenter.h"
#include "microscope.h"

#include "mytexture.h"


//ovladani kamery/zminit ze jsou to realne nastaveni


Presenter::Presenter():m_micro{{1,0,
                                0,1},
                              {0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99,
                               0.99, 0.99, 0.99, 0.99,0.99, 0.99, 0.99, 0.99}, 150, 8, 150, 150}{
  addLens(125,   3.00, 0, 0);
  addLens(105,   10.0, 0, 0);
  addLens(78,    2.55, 0, 0);
  addLens(72,    2.55, 0, 0);
  addLens(50,    3.00, 0, 0);
  addLens(35,    6.18, 0, 0);
  m_micro.run();
}

void Presenter::loadRays(string filename){
  m_micro.loadRayBitmap(filename);
  m_micro.run();
  emit microChanged();
}
void Presenter::loadSpecimen(string filename){
  m_micro.loadSpecimenBitmap(filename);
  m_micro.run();
  emit microChanged();
}
void Presenter::loadMicro(string filename){
  m_micro.loadFromFile(filename);
  m_micro.run();
  emit microChanged();
}
void Presenter::saveMicro(string filename){
  m_micro.save2file(filename);
}


void Presenter::updateSpecimen(double rotation, double z, double absSize){
  m_micro.updateSpecimen(z, (3.14*rotation)/180, absSize);
  m_micro.run();
  emit microChanged();
}
void Presenter::updateDetector(double absSize, int relSize, double zFromSrc){
  m_micro.updateDetector(absSize, relSize, zFromSrc);
  m_micro.run();
  emit microChanged();
}

void Presenter::addLens(double z, double focalLength, double xDev, double yDev){
  if (focalLength < 0.5 && focalLength > -0.5){
    if (focalLength > 0)
      focalLength = 0.5;
    else
      focalLength = -0.5; 
  }
  m_micro.addLens(z+0.000001, focalLength, xDev, yDev);
  m_micro.run();
  emit microChanged();
}
void Presenter::updateLens(int i, double z, double focalLength, double xDev, double yDev){
  if (m_micro.getLensCount() < i+1 || i == -1){
    return;
  }
  m_micro.updateLens(i, z+0.00001, focalLength, xDev, yDev);
  m_micro.run();
  emit microChanged();
}
void Presenter::removeLens(int i){
  if (m_micro.getLensCount() < i+1 || i == -1){
    return;
  }
  m_micro.removeLens(i);
  m_micro.run();
  emit microChanged();
}
void Presenter::addLensViewChosen(){
  emit addLensInfo(m_micro.getTotalHeight()-0.00001);
}

void Presenter::lensChosen(int i){
  LensInfo info = m_micro.getLens(i);
  emit chosenLensInfo(info.height, info.focalLength, info.xDev, info.yDev, m_micro.getTotalHeight()-0.0001);
}

void Presenter::emitLensCount(){
    emit lensCount(m_micro.getLensCount());
}

void Presenter::specimenChosen(){
  SpecimenInfo info = m_micro.getSpecimen();
  emit specimenInfo(info.rotationRadians*180/3.14, info.height, info.absSize, m_micro.getTotalHeight()-0.0001);
}
  void specimenInfo(double rotationRef, double zRef, double absSizeRef, double maxHeightHandle);

void Presenter::detectorChosen(){
  DetectorInfo info = m_micro.getDetector();
  emit detectorInfo(0, info.absSize, info.relSize,m_micro.getTotalHeight()-200, m_micro.getLowestPreDetector()-0.0001);
}

void Presenter::updateMicroVector(Qt3DCore::QEntity *root, QVector<Qt3DCore::QEntity *> &microEntities){
  deleteMicroEntities(microEntities);
  DetectorInfo detInfo = m_micro.getDetector();
  SpecimenInfo specInfo = m_micro.getSpecimen();

  createMicroPlanes(root, microEntities, specInfo, detInfo);

  const RaysVectDigest &keyRays = m_micro.getKeyRays();
  const std::vector<double> &heights = m_micro.getHeights();


  createRayCones(root, microEntities, keyRays, heights);





  double microHeight = m_micro.getTotalHeight();

  createSource(root, microEntities, microHeight);
  createShowOffPlane(root, microEntities, detInfo);
  const std::vector<LensInfo> &lenses = m_micro.getLensVector();

  createLenses(root, microEntities, lenses);
}

void Presenter::deleteEntity(Qt3DCore::QEntity *entity){
  auto components2del = entity->components();
  foreach(auto *component, components2del){
    entity->removeComponent(component);
    delete component;
    component = nullptr;
  }
  delete entity;
  entity = nullptr;
}

void Presenter::deleteMicroEntities(QVector<Qt3DCore::QEntity *> &microEntities){
  foreach(auto *entity, microEntities){
    deleteEntity(entity);
    entity = nullptr;
  }
  microEntities.clear();
}


void Presenter::createPlaneBitmap(Qt3DCore::QEntity *parent,
                       QVector<Qt3DCore::QEntity *> &microEntities,
                       QVector3D translation,
                       QVector3D rotation,
                       double scale,
                       double width,
                       double height,
                       bool turnedDown,
                       Bitmap bitmap
                       ){
  Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
  planeMesh->setWidth(width);
  planeMesh->setHeight(height);

  // Plane mesh transform
  Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
  planeTransform->setScale(scale);
  planeTransform->setRotationX(rotation[0]);
  planeTransform->setRotationY(rotation[1]);
  planeTransform->setRotationZ(rotation[2]);
  planeTransform->setTranslation(translation);

  Qt3DExtras::QDiffuseMapMaterial *planeMaterial = new Qt3DExtras::QDiffuseMapMaterial();

  TextureImage *text = new TextureImage(bitmap, turnedDown);
  QPainter *painter = new QPainter();
  text->paint(painter);

  planeMaterial->diffuse()->addTextureImage(text);
  planeMaterial->setShininess(1);


  delete (painter);
  // Plane
  Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(parent);
  entity->addComponent(planeMesh);
  entity->addComponent(planeMaterial);
  entity->addComponent(planeTransform);
  microEntities.append(entity);
}


void Presenter::createRayCone(Qt3DCore::QEntity* root,
                   QVector<Qt3DCore::QEntity *> &microEntities,
                   double topWidth,
                   double botWidth,
                   double coneLength,
                   double coneTop,
                   double xOffset,
                   double yOffset,
                   double xAngle,
                   double yAngle) {
  Qt3DExtras::QConeMesh *cone = new Qt3DExtras::QConeMesh();
  std::cout << "radius top: " << topWidth << "\n";
  cone->setTopRadius(topWidth/2);
  cone->setBottomRadius(botWidth/2);
  cone->setLength(coneLength);
  cone->setRings(50);
  cone->setSlices(20);

  // ConeMesh Transform
  Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
  coneTransform->setScale3D(QVector3D{1.0f, 1.0f, 1.0f});
  coneTransform->setRotationX(-yAngle*180/3.14);
  coneTransform->setRotationZ(xAngle*180/3.14);
  coneTransform->setTranslation(QVector3D(xOffset, coneTop-(coneLength/2), yOffset));

  Qt3DExtras::QDiffuseSpecularMaterial *coneMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();

  coneMaterial->setAmbient(QColor(255,20,20,255));
  coneMaterial->setDiffuse(QColor(255,40,40,254));
  coneMaterial->setSpecular(QColor(140,0,0,255));
  //coneMaterial->setAlphaBlendingEnabled(true);
  coneMaterial->setShininess(0.8);


  // Cone
  Qt3DCore::QEntity* coneEntity = new Qt3DCore::QEntity(root);
  coneEntity->addComponent(cone);
  coneEntity->addComponent(coneMaterial);
  coneEntity->addComponent(coneTransform);
  microEntities.append(coneEntity);
}

void Presenter::createRayCones(Qt3DCore::QEntity* root,
                    QVector<Qt3DCore::QEntity *> &microEntities,
                    const RaysVectDigest& keyRays,
                    const std::vector<double> &heights){
  int totalRays = static_cast<int>(keyRays.size());
  double topWidth;
  double botWidth;

  double yAngle;
  double xAngle;

  double xOffset;
  double yOffset;

  double coneLength;

  for (int i=0; i<totalRays-1; i++){
    topWidth = std::abs(keyRays[i][0].getPosition().first - keyRays[i][2].getPosition().first);
    botWidth = std::abs(keyRays[i+1][0].getPosition().first - keyRays[i+1][2].getPosition().first);

    xAngle = (keyRays[i][0].getAngles().first + keyRays[i][2].getAngles().first)/2;
    yAngle = (keyRays[i][1].getAngles().second + keyRays[i][3].getAngles().second)/2;

    coneLength = heights[i] - heights[i+1];

    xOffset = (std::cos(3.14/2-xAngle))*coneLength/2 + (keyRays[i][0].getPosition().first + keyRays[i][2].getPosition().first)/2;
    yOffset = (std::cos(3.14/2-yAngle))*coneLength/2 + (keyRays[i][1].getPosition().second + keyRays[i][3].getPosition().second)/2;

    std::cout << "angle: " << xAngle << "\n";

    std::cout << "offset: " << xOffset << "\n";

    std::cout << heights[i] << ", "<< heights[i] - heights[i+1] <<" "<<keyRays[i][0].getPosition().first << " "<<keyRays[i][0].getPosition().first << " "<< topWidth<<"\n";
    fflush(stdout);
    fflush(stdout);

    createRayCone(root, microEntities, topWidth, botWidth, coneLength, heights[i], xOffset, yOffset, xAngle, yAngle);
  }
}



void Presenter::createLenses(Qt3DCore::QEntity *root,
                  QVector<Qt3DCore::QEntity *> &microEntities,
                  const std::vector<LensInfo> &lenses){

  Qt3DExtras::QCylinderMesh *cylinder;
  Qt3DExtras::QDiffuseSpecularMaterial *cylinderMaterial;
  Qt3DCore::QTransform *cylinderTransform;
  Qt3DCore::QEntity* cylinderEntity;


  for (const auto &lensInfo: lenses){
    cylinder = new Qt3DExtras::QCylinderMesh();
    cylinder->setRadius(25);
    cylinder->setLength(0.2);
    cylinder->setRings(100);
    cylinder->setSlices(20);

    // CylinderMesh Transform
    cylinderTransform = new Qt3DCore::QTransform();
    cylinderTransform->setScale(1.f);
    cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
    cylinderTransform->setTranslation(QVector3D(0.0f, lensInfo.height+0.1, 0.0f));

    cylinderMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    cylinderMaterial->setDiffuse(QColor(80,100,150,220));
    cylinderMaterial->setAmbient(QColor(0,0,0,120));
    cylinderMaterial->setSpecular(QColor(0,0,0,200));
    cylinderMaterial->setShininess(0.0005);
    cylinderMaterial->setAlphaBlendingEnabled(true);


    // Cylinder
    cylinderEntity = new Qt3DCore::QEntity(root);
    cylinderEntity->addComponent(cylinder);
    cylinderEntity->addComponent(cylinderMaterial);
    cylinderEntity->addComponent(cylinderTransform);

    microEntities.append(cylinderEntity);
  }
}

void Presenter::createSource(Qt3DCore::QEntity* root,
                  QVector<Qt3DCore::QEntity *> &microEntities,
                  double microHeight){
  Qt3DExtras::QCylinderMesh *cylinder;
  Qt3DExtras::QDiffuseSpecularMaterial *cylinderMaterial;
  Qt3DCore::QTransform *cylinderTransform;
  Qt3DCore::QEntity* cylinderEntity;

  cylinder = new Qt3DExtras::QCylinderMesh();
  cylinder->setRadius(3);
  cylinder->setLength(2);
  cylinder->setRings(100);
  cylinder->setSlices(20);

  // CylinderMesh Transform
  cylinderTransform = new Qt3DCore::QTransform();
  cylinderTransform->setScale(1.f);
  cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
  cylinderTransform->setTranslation(QVector3D(0.0f, microHeight+1, 0.0f));

  cylinderMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
  cylinderMaterial->setDiffuse(QColor(20,20,20,120));
  cylinderMaterial->setShininess(0.0005);
  cylinderMaterial->setAlphaBlendingEnabled(true);


  // Cylinder
  cylinderEntity = new Qt3DCore::QEntity(root);
  cylinderEntity->addComponent(cylinder);
  cylinderEntity->addComponent(cylinderMaterial);
  cylinderEntity->addComponent(cylinderTransform);

  microEntities.append(cylinderEntity);
}

void Presenter::createMicroPlanes(Qt3DCore::QEntity *root,
                       QVector<Qt3DCore::QEntity *> &microEntities,
                       SpecimenInfo specInfo,
                       DetectorInfo detInfo){
  // createSpecimenEntity root specInfo
  std::cout << "________________\n";
  createPlaneBitmap(root, microEntities,{0,static_cast<float>(specInfo.height),0},
                                     {0,static_cast<float>((specInfo.rotationRadians*180)/(2*3.14)),0}, 1, specInfo.absSize, specInfo.absSize, false,
                                     specInfo.bitmap);

  createPlaneBitmap(root, microEntities,{0,static_cast<float>(specInfo.height),0},
                                     {0,180+static_cast<float>((specInfo.rotationRadians*180)/(2*3.14)),180}, 1, specInfo.absSize, specInfo.absSize, true,
                                     specInfo.bitmap);


  std::cout << "________________\n";




  // createSpecimenEntity root detInfo
  createPlaneBitmap(root, microEntities,{0,-0.02,0},
                                     {0,0,0}, 1, detInfo.absSize, detInfo.absSize, false,
                                     detInfo.bitmap);
  createPlaneBitmap(root, microEntities,{0,-0.025,0},
                                   {0,180,180}, 1, detInfo.absSize, detInfo.absSize, true,
                                     detInfo.bitmap);
}


void Presenter::createShowOffPlane(Qt3DCore::QEntity *root,
                        QVector<Qt3DCore::QEntity *> &microEntities,
                        DetectorInfo detInfo){

  Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
  cuboid->setXExtent(47);
  cuboid->setYExtent(47);
  cuboid->setZExtent(1);


  // CuboidMesh Transform
  Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
  cuboidTransform->setScale(1.0f);
  cuboidTransform->setTranslation(QVector3D(120.f, 75.f, -115.0f));
  cuboidTransform->setRotationY(-35);


  Qt3DExtras::QDiffuseSpecularMaterial *cuboidMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
  cuboidMaterial->setDiffuse(QColor(0x111111));
  //Cuboid
  Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(root);
  cuboidEntity->addComponent(cuboid);
  cuboidEntity->addComponent(cuboidMaterial);
  cuboidEntity->addComponent(cuboidTransform);


  microEntities.append(cuboidEntity);



  createPlaneBitmap(root, microEntities, QVector3D(119.f, 75.f, -114.0f), {90,-35,0},1,45,45,false, detInfo.bitmap);
}
