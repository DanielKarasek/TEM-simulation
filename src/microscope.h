#ifndef MICROSCOPE_H_INCLUDED
#define MICROSCOPE_H_INCLUDED

#include "microscopeobject.h"
#include "specimen.h"
#include "typedefs.h"
#include "detector.h"
#include "lens.h"

#include <string.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>

using std::string;
/*!
 * \brief Třída pro mikroskop
 * \author Daniel Karásek, xkaras38
 * \details
 * <a href="classMicroscope.html">Mikroskop</a> se skládá z optických objektů (<a href="classLens.html">čočka</a>, <a href="classDetector.html">detektor</a> ,<a href="classSpecimen.html">vzorek</a>),
 * které lze za běhu programu dynamicky modifikovat. Pro aproximaci chování těchto objektů na paprsky se vytváří paraxiální optika. Třída <a href="classSpecMat.html">SpecMat</a> obaluje matici o velikosti 2x2, která
 * se dá využít na transformační matici simulující tyto objektu, ale i pro posun ve výšce(můžeme tedy pracovat pouze ve dvou osách a osu výšky zpracovávat stranou dle potřeby). Tato třída se dá využít i pro
 * uložení matice reprezentující paprsek s hodnotami pozic v prvním řádku a uhlů v řádku druhém. Každý optický objekt implementuju svojí optickou metodu pracující s paprskem, která obsahuje využití třídy SpecMat.
 *
 * V případě, že je vstupem kružnice(viz zjednodušení na 2d výše) měla by být výstupem elipsa na všech objektech.
 * Navíc pokud přihlédneme k nezávislosti os x a y můžeme vybrat hlavní a vedlejší vrcholy vstupní kružnice(koukáme-li se na ni jako na elipsu) a sledovat pouze paprsky procházející těmito body. Ty mají poté dostatečnou
 * výpovědní hodnotu o celém průběhu paprsků. Toho model využívá a vypočítá průběh těchto paprsků a jejich pozice u jednotlivých objektů a ukládá je do vektoru klíčových pozic. Nebezpečím je v tomto přístupu překřížení,
 * které by se poté nezobrazovalo, jelikož bychom znali pouze hodnoty na objektech a ne místo překřížení. Proto je před každým objektem nejdříve zjištěno zda při posunu paprsku k objektu k překřížení došlo a případně kde.
 * Tyto hodnoty jsou přidány do vektoru s klíčovými pozicemi. Pro správné zobrazení je stranou udržován také vektor výšek těchto klíčových pozic.
 *
 * Raytracing je proveden podobně jako zjišťování obecného průběhu paprsku, akorát místo klíčových paprsků je zde n paprsků(podle množství zadaných paprsků) uložených do m kružnic
 * (o průměru nula až max průměr celého svazku paprsků, kde m je sqrt(n)).
 * Zde je podstatná třídá <a href="classRay.html">Ray</a>, která se užívá již výše, kde ale slouží pouze jako obal pro SpecMat reprezentující paprsek.
 * Zde se využívá navíc členská proměnná intensity paprsku. Optická metoda objektů <a href="classDetector.html">detektoru</a> a <a href="classSpecimen.html">vzorku</a> má jako hlavní úkol pracovat právě s intensitou.
 * Vzorek počítá, kterou částí vzorek probíhá a jak moc je v tom bodě propustný. Toto se využije k výpočtu nové intensity. Podobně počítá detektor, kterým bodem vzorek probíhá. Rozdílem je, že detektor intenzitu využívá
 * k získání výsledného zobrazení ve vektoru. Zde není potřeba nikde ukládat průběh paprsků, ale stačí nám konečný výsledek.
 *
 * Všechny výšky jsou počítány relativně od detektoru, který je ve výšce 0. Pokud se mění detektor tak se ve skutečnosti mění celý mikroskop a jen detektor zůstává na svém místě.
 *
 *
 */
class Microscope{
private:
  std::vector<std::unique_ptr<MicroscopeObject>> m_objects; /*!< Vektor všech objektů v mikroskopu */
  RaysVectDigest m_keyRays; /*!< Vektor klíčových paprsků v klíčových výškách */
  std::vector<double> m_heights; /*!< vektor klíčových výšek */

  int m_specimenPos{1}; /*!< Pozice vzorku */
  double m_totalHeight;

  Bitmap m_intensityBitmap; /*!< Bitmapa intensity paprsků */

  double magic_base_diameter{0.5}; /*!< kouzelná konstanta pro poloměr počátečního svazku */

  /*!
   * \brief trackBack pokud došlo k překřížení paprsků mezi objekty, tato funkce nalezne místo a stav paprsků v něm
   * \param rays paprsky ve výšce objektu následujícím po překřížení
   * \param majorAxisNum v jaké ose došlo ke skřížení
   * \return paprsky v bodě střetnutí a vzdálenost k bodu skřížení
   */
  std::pair <std::vector<Ray>, double> trackBack(const std::vector<Ray> &rays, int majorAxisNum);
  /*!
   * \brief circlefy, transformuje paprsky ležící na čtverci na paprsky ležící na kružnici
   * \param rays paprsky ležící na čtvercy
   */
  void circlefy(std::vector<Ray> &rays);

public:
  // specimen size->square size specmat
  /*!
   * \brief Microscope
   * \param specBitmap bitmapa vzorku
   * \param rayBitmap bitmapa paprsků
   * \param height velikost mikroskopu
   * \param specimenSize velikost hrany vzorku(vzorek je čtvercového tvaru)
   * \param detectorAbsSize velikost jedné z hran detektoru(detektor je také čtvercový)
   * \param detectorRelSize rozližení detektoru(rozlišení zachýtávaných pixelů=relSize x relSize)
   */
  Microscope(Bitmap specBitmap, Bitmap rayBitmap, double height,
              double specimenSize, double detectorAbsSize, int detectorRelSize);

  /*!
   * \brief Microscope prazná inicializace, použití pouze před loadFromFile
   */
  Microscope(){}

  Microscope(const Microscope &) = delete;

  /*!
   * \brief run spustí kompletní simulaci
   */
  void run();
  /*!
   * \brief runRaytrace spustí simulaci raytracingu
   */
  void runRaytrace();
  /*!
   * \brief runGetDigest spustí simulaci pro svazek paprsků jako celek
   */
  void runGetDigest();

  /*!
   * \brief getTotalHeight
   * \return klíčové výšky paprsků získané při simulaci obecného svazku
   */
  double getTotalHeight(){return m_totalHeight;}

  /*!
   * \brief getLensCount
   * \return pocet čoček
   */
  int getLensCount();

  void addLens(double z, double focalLength, double xDev, double yDev);
  void updateLens(int i, double z, double focalLength, double xDev, double yDev);

  void removeLens(int i);

  LensInfo getLens(int i);
  /*!
   * \brief getLensVector
   * \return vektor informací o všech čočkách
   */
  std::vector<LensInfo> getLensVector();

  void updateSpecimen(double z, double rotation, double absSize);
  SpecimenInfo getSpecimen();

  /*!
   * \brief updateDetector upraví detektor detailed popis pro infor proč je tato metoda jedinečná
   * \details
   * Jelikož jsou všechny výšky ve vztažné sousatvě z detektor, a posouvat se sám vůči sobě by bylo
   * nemožné(aspoň z mého pohledu fyzika amatéra). Tak se zde používá vzdálenost od zdroje. A poté nedojde ke změně
   * výšky detektoru, nýbrž k posunutí celého mikroskopu kromě detektoru výše nebo níže
   */
  void updateDetector(double absSize, int relSize, double zFromSrc);
  DetectorInfo getDetector();

//  void updateMicroscopeHeight(double newHeight);

  /*!
   * \brief getHeights
   * \return vektor výšek v klíčových pozicích
   */
  std::vector<double> getHeights();
  /*!
   * \brief getKeyRays
   * \return reference na vektor čtveřic paprsků v klíčových pozicích
   */
  const RaysVectDigest& getKeyRays();
  /*!
   * \brief getLowestPreDetector
   * \return vrací vzdálenost k nejnižšímu objektu před detektorem
   */
  double getLowestPreDetector();

  /*!
   * \brief save2file uloží mikroskop do souboru
   * \param filename soubor
   */
  void save2file(string filename);
  /*!
   * \brief loadFromFile načte mikroskop ze souboru
   * \param filename soubor
   */
  void loadFromFile(string filename);
  /*!
   * \brief loadRayBitmap načte bitmapu intenzit paprsků ze souboru
   * \param filename soubor
   */
  void loadRayBitmap(string filename);
  /*!
   * \brief loadSpecimenBitmap načte bitmapu vzorku ze souboru
   * \param filename soubor
   */
  void loadSpecimenBitmap(string filename);

};



#endif // MICROSCOPE_H_INCLUDED
