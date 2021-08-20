# TEM simulátor 
## Popis
Projekt simuluje TEM mikroskop se vstupem kolineárního souboru paprsků pomocí série čoček aproximovaných paraxialní optikou (předpoklad nízkého uhlu dopadu). Umí simulovat jak průběh celého vstupního svazku tak i průběh pro jednotlivé paprsky (ray-tracing). Celý projekt je dělán v jednotkách centimetrů. 

Matice vzorků, paprsků a celá konfigurace mikroskopu lze načítat ze souborů. Matice vzorku jsou čísla od 0 do 1 značící propustnost (0&nbsp;-&nbsp;paprsek je plně potlačen, 1&nbsp;-&nbsp;propuštěn). Matice parpsků jsou čísla od 0 do 1 reprezentující intenzitu (víc == víc přirozeně). Počet prvků v těchto maticích musí být odmocnitelný na celé číslo. Tedy musí jít o matici reprezentovatelnou jako čtvercová, i když je zapsána na jednom řádku - v opačném případě je vzato jen tolik prvků aby z nich šel vytvořit čtverec. Příkladem jsou soubory raySetting a specimenSettings v složce examples. Soubor konfigurace mikroskopu lze získat pouze uložením konfigurace vytvořené v aplikaci (nepoužíváme boost pickle, takže to je plain text, ale příliš komplikovaný). 

Optické jevy způsobené posuny čoček při zobrazení paprsků aproximujeme natočením kužele, což může při akumulaci čoček nenulovou hodnotou posuvu vést k zvláštně vypadajícímu mikroskopu - zde jsme omezení nedostatkem složitějších tvarů než je kužel nebo natočený kužel. 

Občas dochází k nevykreslení kužele nebo vykreslení čoček před ostatními předměty což vede k jejich neviditelnosti. Problém pochazí ze způsobu, kterým QT vykresluje. Experimentoval jsem z různými strategiemi vykreslení (od zadu do  předu a naopak dle hloubky, sekvenčně dle přidáváni objektů etc.) i jejich konfigurací, ale to nevedlo k žádné změně (z nějakého důvodu někdy alpha, i když byla užita pro všechny prvky stejná vertex shading a fragment shading strategie, nefunguje jako průhlednost a jindy ano). Při další změně mikroskopu problém někdy(!) zmizí (např. 2 zmáčknutí apply u změny čočky - zůstane stejná ale zbavíme se chyby, tbh QT 3D mě dost mate).

## Spuštění
Aplikace vyžaduje qmake pro QT 5.15 (používali sme 5.15.2, ale minor verze by neměla vadit) a c++17. Aplikace je přeložená příkazem make v kořenovém souboru adresáře. Tímto je vygenerován spustitelný soubor TEM.

## Ovládání
Ovládání 3D scény probíhá šipkami a myší. Šipkami se pohybuje scéna. Kolečko scénu přibližuje a oddaluje a levé tlačítko dohromady s pohybem myši scénu rotuje.


## S čím jde hýbat
Aplikace je interaktivní a jde tedy upravit mnoho různých vlastností mikroskopu. 
Čočky:
* Lze přidávat čočku, které nastavíme vlastnosti fokální vzdálenost (pokud mezi -0.5 až 0.5, tak se nastaví na -0.5 nebo 0.5, aby se nerozbil mikroskop nekonečně silnou čočkou), vychýlení v osách x a y a nakonec vzdálenost od detektoru (spodek mikroskopu, výška je v celém projektu osa Z). Čočku lze vybrat a tyto vlastnosti buď upravit nebo ji celou smazat 

Detektor:
* U detektoru lze nastavit změna ve výšce (Z osa). Mimo to také absolutní velikost (velikost detektoru v cm) a relativní velikost (množství políček, na které je detektor rozdělen - pokud 150 tak je rozlišení detektoru 150x150).

Specimen:
* U vzorku lze nastavit jeho rotace, absolutní velikost a vzdálenost od detektoru. Relativní velikost je dána maticí vzorku. Ta musí být načtena ze souboru

Načítání a ukládání:
* Jak již bylo výše zmíněno, lze načíst matice paprsků a jejich intenzit (hodnoty 0-1, čtvercová matice), vzorek (hodnoty 0-1, čtvercová matice propustnosti) a mikroskop (soubor získaný uložením existující konfigurace v aplikaci). Aplikace neobsahuje žádné rozhraní pro vytváření paprsků ani vzorků, a proto není možné je uložit.

Ukázka vzhledu aplikace s prázdným mikroskopem - v pozadí je vidět na tabulce výsledek ray tracingu

![Base app view](https://github.com/DanielKarasek/TEM-simulation/blob/main/readme_imgs/basic_view.png)

## Příklady - ukázky
V examples se nachází 5 různých konfigurací s paprsky dopadajicími na detektor (setting1,2...). 
Tyto konfigurace byli vytvořeny expertem z firmy Thermo Fisher, ačkoliv nejsou plně realistické, mimo jiné proto že skutečné zvětšení by vypadalo v takovémto projektu nepřehledně. V příkladech 4 a 5 jsou ukázky odchylky (simulace vad mikroskopu). V ukázce 5 je vidět i výše zmíněný zvláštní vzhled mikroskopu, protože odchylka vzniká v bodě kdy je paprsek malý, a tato vada se zvětšením paprsku znásobí.

Ukázka funkčního mirkoskopu z příkladu 1

![Funkcni](https://github.com/DanielKarasek/TEM-simulation/blob/main/readme_imgs/working_settings.png)

Ukázka nefunkčního mikroskopu z příkladu 5

![Retardovany](https://github.com/DanielKarasek/TEM-simulation/blob/main/readme_imgs/settings_with_err.png)


## Autoři
Projekt byl vytvořen jako součást předmětu ICP na VUT FIT. Autory jsou Daniel Karásek (Backend, Prezentér, 3D scéna, menu napravo), Filip Osvald (menu napravo).

