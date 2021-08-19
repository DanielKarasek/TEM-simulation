Tvůrci projektu jsou Daniel Karásek(xkaras38), Filip Osvald (xosval04)

Popis:
Projekt simuluje TEM se vstupem kolineárního souboru paprsků pomocí série čoček aproximované paraxialní optikou. 
Umí simulovat jak průběh celého svazku tak i průběh pro jednotlivé paprsky (jednoduchý ray-tracing).
Matice vzorků, paprsků a celá konfigurace mikroskopu lze načítat ze souborů. Matice vzorků jsou čísla od 0 do 1. 
Matice parpsků jsou čísla od 0 do 1 reprezentující intenzitu. Počet prvků v těchto maticích musí být odmocnitelný na celé číslo. Viz soubory raySetting a specimenSettings v složce examples.
Soubor konfigurace mikroskopu lze získat pouze uložením konfigurace vytvořené v aplikaci. Celý projekt pracuje v jednotkách centimetru.
Optické jevy způsobené posuny čoček při zobrazení paprsků aproximujeme natočením kužele což může při akumulaci čoček s extrémní hodnotou posuvu vést k neperfektně vypadajícímu mikroskopu.
Občas dochází k nevykreslení kužele nebo vykreslení čoček před ostatními předměty což vede k jejich neviditelnosti. Problém pochazí ze způsobu, kterým QT vykresluje. Při další změně mikroskopu by měl problém zmizet.
V GUI se nevypisuje pozice průchodu vzorkem a detektorem ani průměr svazku.
Ovládáni probíhá šipkami a myší. Kolečko pro vzdálenost a levé tlačítko a pohyb myší pro rotaci.

Předpoklady:
Předpokládáme verzi QT 5.15.2 jelikož ve verzi 5.5.1 neexistuje tvar kužele který je základním kamenem mikroskopu.

Příklady:
V examples se nachází 5 různých konfigurací s paprsky dopadajicími na detektor (setting1,2...). 
Tyto konfigurace byli vytvořeny expertem z firmy Thermo Fisher, ačkoliv nejsou plně realistické, jelikož skutečné zvětšení
by vypadalo v takovémto projektu nepřehledně. V příkladech 4 a 5 jsou ukázky odchylky (simulace vad mikroskopu). V ukázce 5
je vidět i výše zmíněný zvláštní vzhled mikroskopu, protože odchylka vzniká v bodě kdy je paprsek malý, a proto se tato vada zvětší.

