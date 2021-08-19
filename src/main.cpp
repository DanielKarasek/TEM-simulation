#include "scene.h"
#include "presenter.h"
#include "microscopeobject.h"
#include "lens.h"
#include "specimen.h"
#include "detector.h"
#include <QObject>
#include <QtWidgets/QApplication>
#include <Qt3DRender/QSortPolicy>
/*!
 * \brief Třída pro View
 * \author Filip Osvald, xosval04
 * \author Danie Karásek, xkaras38
 */

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  Scene *scene = new Scene();


  Qt3DRender::QSortPolicy *sortPolicy = new Qt3DRender::QSortPolicy(scene->getRoot());

  sortPolicy->setSortTypes(QVector<int>{sortPolicy->BackToFront});
  Presenter presenter = Presenter();


  // scene and connects:

  //scene change connects
  QObject::connect(&presenter, &Presenter::microChanged,
                       scene, &Scene::updateScene);
  QObject::connect(scene, &Scene::signalUpdateMicroVector,
                       &presenter, &Presenter::updateMicroVector);

  // add lens connects
  QObject::connect(scene, &Scene::signalAddLens,
                       &presenter, &Presenter::addLens);

  // load connects
  QObject::connect(scene, &Scene::signalLoadRays,
                   &presenter, &Presenter::loadRays);
  QObject::connect(scene, &Scene::signalLoadSpecimen,
                   &presenter, &Presenter::loadSpecimen);
  QObject::connect(scene, &Scene::signalLoadMicroscope,
                     &presenter, &Presenter::loadMicro);
  QObject::connect(scene, &Scene::signalSaveMicro,
                     &presenter, &Presenter::saveMicro);

  // lens update connects
  QObject::connect(scene, &Scene::signalUpdateLens,
                   &presenter, &Presenter::updateLens);
  QObject::connect(scene, &Scene::signalRemoveLens,
                   &presenter, &Presenter::removeLens);
  QObject::connect(scene, &Scene::signalGetLensCount,
                   &presenter, &Presenter::emitLensCount);
  QObject::connect(&presenter, &Presenter::lensCount,
                   scene, &Scene::fillCombox);
  QObject::connect(scene, &Scene::signalChosenLensChanged,
                   &presenter, &Presenter::lensChosen);
  QObject::connect(&presenter, &Presenter::chosenLensInfo,
                   scene, &Scene::presetLensUpdate);

  // detector connects
  QObject::connect(scene, &Scene::signalUpdateDetector,
                   &presenter, &Presenter::updateDetector);
  QObject::connect(scene, &Scene::signalGetDetectorInfo,
                   &presenter, &Presenter::detectorChosen);
  QObject::connect(&presenter, &Presenter::detectorInfo,
                   scene, &Scene::presetDetectorUpdate);

  //specimen connects
  QObject::connect(scene, &Scene::signalUpdateSpecimen,
                   &presenter, &Presenter::updateSpecimen);
  QObject::connect(scene, &Scene::signalGetSpecimenInfo,
                   &presenter, &Presenter::specimenChosen);
  QObject::connect(&presenter, &Presenter::specimenInfo,
                   scene, &Scene::presetSpecimenUpdate);

  //add lens info max height connects
  QObject::connect(scene, &Scene::signalAddLensViewChosen,
                   &presenter, &Presenter::addLensViewChosen);
  QObject::connect(&presenter, &Presenter::addLensInfo,
                   scene, &Scene::maxLensZ);

  scene->updateScene();
  return app.exec();
}



/**
 * \mainpage TEM
 *
 * \section Uvod Uvod do designového návrhu
 *
 * \tableofcontents
 *
 *
 * Jako návrhový vzor pro tento projekt je použitá adaptace model-presenter-view. Modelem je třída <a href="classMicroscope.html">mikroskopu</a>. Ten se skládá z jednotlivých
 * optických objektů(<a href="classLens.html">čočka</a>, <a href="classDetector.html">detektor</a> ,<a href="classSpecimen.html">vzorek</a>). Dále nabízí funkcionalitu pro spuštění
 * simulace a následné získání výsledků. Model také obsahuje celou sadu funkcí pro jeho úpravu(přídání-změna čoček, změna vzorku a detektoru). <a href="classPresenter.html">Prezentér</a>
 * obsluhuje komunikaci mezi view a modelem. V případě iniciace změny modelu z view zavolá příslušnou funkci modelu s ošetřenými vstupy a následně updatuje view. Model je v tomto případě
 * členskou funkcí prezentéru. Důvodem je že mikroskop se vykresluje celý znova po každé změně a posílat veškeré informace o mikroskopu by bylo zbytečně náročné. Dále reaguje pouze na akce
 * z viewu, které vykonává uživatel (nedochází k samovolným změnám například vlivem času), tudíž veškeré změný procházejí přes prezentéra a vytvářet desítky slotů a signálu mi přišlo zbytečné.
 * Poslední složkou je <a href="classScene">view</a>. Jde o velmi tenkou třídu, která vytvoří statické objekty(jednotlivé widgety pro změny optických objektů apd.). Dále obsahuje settery(sloty) pro
 * jejich hodnoty(abychom mohli například přednastavit hodnoty vybrané čočky) a jednoduchou logiku nevyžadující model na úrovni přepnutí widgetů v reakci na zmáčklé tlačítko.
 *
 *
 *
 * \section Model Mikroskop-modelová část
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
 * Model obsahuje funkce, které spustí výše zmíněné simulace a získají jejich výsledky. Dále obsahuje funkce pro modifikace jednotlivých objektů a získání jejich metadat.
 * \section Prezenter
 * <a href="classPresenter.html">Prezentér</a> funguje jako most mezi modelem a view. Obsahuje sloty, které se starají o ošetření vstupů z view a dále modifikaci dat v modelu. Dále sloty, které iniciuí získání dat
 * z modelu a jejich následné předání do správného slotu view. Nakonec obsahují funkce pro vytvoření jednotlivých entit mikroskopu i celého mikroskopu. Mikroskop je zobrazen pomocí válců(čočky + zdroj),
 * ploch(vzorek, detektor, zobrazení výsledků) a komolých kuželů(svazek paprsků). Jelikož QT pouze základní komolé kužely, tak nemůžeme aproximovat elipsy v zobrazení(pouze v raytracingu u výsledků to je možné).
 * I vychýlení paprsků není přesné, ale pouze aproximované nahnutím kužele. V případě užití extrémních hodnot čoček může být zobrazení poněkud zavádějící.
 *
 * \section View
 * Třída reprezentující View. Umožňuje uživateli zadávat změny pro mikroskop v uživatelském rozhraní. Obsahuje sloty a signály komunikující s třídou  Presenter pro aplikování změn
 * mikroskopu. Sloty reagující na signály z widgetu (tlačítek a sliderů). A jednotlivé widgety které se zobrazují podle toho kde se uživatel v menu nachází. Propojení s presenterem je zde v mainu.
 *
 */



