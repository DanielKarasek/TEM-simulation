#ifndef PRESENTER_H
#define PRESENTER_H
#include "scene.h"
#include "microscope.h"


#include <QObject>
#include <QVector>
#include <QEntity>


#include <QPainter>
#include <QPaintedTextureImage>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <QDiffuseSpecularMaterial>

#include <Qt3DExtras/QDiffuseMapMaterial>
/*!
 * \brief Třída pro prezentéra v model-presenter-view modelu
 * \author Daniel Karásek, xkaras38
 * \details
 * <a href="classPresenter.html">Prezentér</a> funguje jako most mezi modelem a view. Obsahuje sloty, které se starají o ošetření vstupů z view a dále modifikaci dat v modelu. Dále sloty, které iniciuí získání dat
 * z modelu a jejich následné předání do správného slotu view. Nakonec obsahují funkce pro vytvoření jednotlivých entit mikroskopu i celého mikroskopu. Mikroskop je zobrazen pomocí válců(čočky + zdroj),
 * ploch(vzorek, detektor, zobrazení výsledků) a komolých kuželů(svazek paprsků). Jelikož QT pouze základní komolé kužely, tak nemůžeme aproximovat elipsy v zobrazení(pouze v raytracingu u výsledků to je možné).
 * I vychýlení paprsků není přesné, ale pouze aproximované nahnutím kužele. V případě užití extrémních hodnot čoček může být zobrazení poněkud zavádějící.
 *
 * Slotové funkce odpovídájí jejich názvu :D. Jejich účelem je propojení modelu a view. Blíže jsou specifikovány pouze zajímavější
 */
class Presenter : public QObject
{
  Q_OBJECT

public:
  explicit Presenter();

public slots:

  void updateMicroVector(Qt3DCore::QEntity *root, QVector<Qt3DCore::QEntity *> &microEntities);
  void loadRays(string filename);
  void loadSpecimen(string filename);
  void loadMicro(string filename);
  void saveMicro(string filename);

  /*!
   * \brief updateSpecimen
   * \param rotation
   * \param z
   * \param absSize
   */
  void updateSpecimen(double rotation, double z, double absSize);
  void updateDetector(double absSize, int relSize, double zFromSrc);
  void addLens(double z, double focalLength, double xDev, double yDev);
  void updateLens(int i, double z, double focalLength, double xDev, double yDev);
  void removeLens(int i);

  void addLensViewChosen();
  void lensChosen(int i);
  void specimenChosen();
  void detectorChosen();
  void emitLensCount();
signals:
  void microChanged();
  void chosenLensInfo(double zRef, double opticalPowerRef, double xDevRef, double yDevRef, double maxHeightHandle);
  void specimenInfo(double rotationRef, double zRef, double absSizeRef, double maxHeightHandle);
  void detectorInfo(double zRef, double absSizeRef, int relSizeRef, double minHeightHandle, double maxHeightHandle);
  void addLensInfo(double maxHeightHandle);
  void lensCount(int count);

private:

  Microscope m_micro;

  /* slots:
   *  loadRays - reaguje na signal stisknuti tlacitka -> filename
   *  loadSpecimen - reaguje na signal stistknuti tlacitka -> filename
   *  loadMicro - reaguje na signal stistknuti tlacitka -> filename
   *
   *  updateSpecimen - reaguje na signal stisknuti tlacitka -> rotation, newHeight
   *  updateDetector - reaguje na signal stistknuti tlacit -> newHeight
   *  addLens - reaguje na signal stistknuti tlacit -> foc, x, y, height
   *  updateLens - reaguje na signal stistknuti tlacitka -> foc, x, y, height, i
   *  RemoveLens - reaguje na signal stistknuti tlacitka -> i
   *
   *  lensChosen - reference na hodnoty foc, x,y, height, maxHeight, - reaguje na signal stistknuti tlacit + zmena combo boxu
   *  specimenChosen - reference na hodnoty rot, abs_size, height, maxHeight - reaguje na signal stistknuti tlacit
   *  detectorChosen - reference na hodnoty vzdalenost curr, abs_size, rel_size, minHeight - reaguje na signal stistknuti tlacit
   *
   * signals:
   *
   *  microChanged -> bez params
  */
  /*!
   * \brief deleteMicroEntities
   * \param microEntities vektor entit k smazání
   */
  void deleteMicroEntities(QVector<Qt3DCore::QEntity *> &microEntities);
  /*!
   * \brief deleteEntity smaže entitu
   */
  void deleteEntity(Qt3DCore::QEntity *);
  /*!
   * \brief createPlaneBitmap vytvoří planinu s danými transformacemi a odpovídající bitmapovou texturou
   * \param parent rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param translation translace planini
   * \param rotation rotace plani
   * \param scale zvětšení planiny
   * \param width šířka planiny
   * \param height výška planiny
   * \param turnedDown zdali je planin otočená vzhůru nohama(pro vykreslení planiny z obou stran musíme vykreslit 2)
   * \param bitmap bitmapa užitá jako textura
   */
  void createPlaneBitmap(Qt3DCore::QEntity *parent,
                         QVector<Qt3DCore::QEntity *> &microEntities,
                         QVector3D translation,
                         QVector3D rotation,
                         double scale,
                         double width,
                         double height,
                         bool turnedDown,
                         Bitmap bitmap
                         );
  /*!
   * \brief createRayCone vytvoří jeden kužel dle parametrů
   * \param root rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param topWidth šířka vršku kužele
   * \param botWidth šířska spodku kužele
   * \param coneLength délka kužele
   * \param coneTop výška vrcholu kužele
   * \param xOffset vychýlení kužele v ose x
   * \param yOffset vychýlení kužele v ose y
   * \param xAngle uhel kužele v ose x
   * \param yAngle uhel kužele v ose y
   */
  void createRayCone(Qt3DCore::QEntity* root,
                     QVector<Qt3DCore::QEntity *> &microEntities,
                     double topWidth,
                     double botWidth,
                     double coneLength,
                     double coneTop,
                     double xOffset,
                     double yOffset,
                     double xAngle,
                     double yAngle);
  /*!
   * \brief createRayCones vytvoří všechny kužele(celý průběh svazku paprsků) mikroskopu
   * \param root rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param keyRays vektor čtveřic paprsků v klíčových bodech
   * \param heights vektor výšek klíčových bodů
   */
  void createRayCones(Qt3DCore::QEntity* root,
                      QVector<Qt3DCore::QEntity *> &microEntities,
                      const RaysVectDigest& keyRays,
                      const std::vector<double> &heights);
  /*!
   * \brief createLenses vytvoří všechny čočky v mikroskopu
   * \param root rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param lenses odkaz na vektor metadat všech čoček
   */
  void createLenses(Qt3DCore::QEntity *root,
                    QVector<Qt3DCore::QEntity *> &microEntities,
                    const std::vector<LensInfo> &lenses);
  /*!
   * \brief createSource vytvoře zdroj
   * \param root rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param microHeight celková výška mikroskopu
   */
  void createSource(Qt3DCore::QEntity* root,
                    QVector<Qt3DCore::QEntity *> &microEntities,
                    double microHeight);
  /*!
   * \brief createMicroPlanes vytvoří plochy značící detektor a vzorek
   * \param root rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param specInfo metadata o vzorku
   * \param detInfo metadata o detektoru
   */
  void createMicroPlanes(Qt3DCore::QEntity *root,
                         QVector<Qt3DCore::QEntity *> &microEntities,
                         SpecimenInfo specInfo,
                         DetectorInfo detInfo);
  /*!
   * \brief createShowOffPlane vytvoří plochu pro zobrazování výsledků
   * \param root rodičovská entita
   * \param microEntities vektor entit mikroskopu
   * \param detInfo metadata o detektoru
   */
  void createShowOffPlane(Qt3DCore::QEntity *root,
                          QVector<Qt3DCore::QEntity *> &microEntities,
                          DetectorInfo detInfo);
};


#endif // PRESENTER_H
