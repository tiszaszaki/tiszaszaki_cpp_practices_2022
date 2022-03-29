# Egyszerű játék

## A játék eredete

Egy Boulder Dash-típusú játék, ami ezenfelül Supaplex és Sokoban néhány elemeit ötvözi.

A játék még félkész állapotban van, jelenleg még nem játszható. Ismert hiányosságai a következők: játékos irányítása, bizonyos elemek fizikája, játszható pályacsomagok, parancssor helyett grafikus felület.

## Használata

### Példa futtatásra

`> egyszerujatek.exe elsopalya.txt`

vagy

`> egyszerujatek.exe`

A paraméter nélküli futtatáskor be kell írni a véletlenszerűen generálandó pálya paramétereit.

Például: `32 32 1 128 32 16 64 16 64 128`

A paraméterek leírása:
 1. a pálya sorainak a száma
 2. a pálya oszlopainak a száma
 3. a pálya határoló falának a vastagsága
 4. a pálya belső falelemeinek a száma
 5. a pályán lévő drágakövek száma
 6. a pályán lévő ellenfelek száma
 7. a pályán lévő kősziklák száma
 8. a pályán lévő bombák száma
 9. a pályán lévő "csillagok" száma
 10. a pályán lévő "levegő" mennyisége

A pályelemek leírása:
 * fal (`#`): áthatolhatatlan és megsemmisíthetetlen akadály
 * ember (`@`): a játéknak a játékos által irányított szereplője
 * levegő (`:`): nem-játékos számára áthatolhatatlan, a játékos által eltüntethető, a zuhanó tárgyaknak akadályt jelentő elem
 * drágakő (`*`): a játékos által gyűjthető, zuhanó elem
 * egyszerű ellenfél (`&`): a játéknak önállóan irányított, a játékos megsemmisítésére törekvő szereplője, a játékossal való érintkezés robbanással jár
 * könnyű fal (`|`): a falhoz hasonló, de robbanásra megsemmisíthető elem
 * kőszikla (`o`): áthatolhatatlan, zuhanó elem, az alatta lévő ember, ellenfél vagy bomba robbanását okozza
 * bomba (`+`): a kősziklához hasonló elem, de zuhanás után minden elem felett felrobban
 * kijárat (`E`): a játékos végső célja, előre beállított mennyiségű drágakő gyűjtése után átjárható
 * csillagok (`%`): különleges ellenfél a Supaplexből, amelynek robbanása esetén drágaköveket hagy el maga körül
 * láda (`=`): az első olyan elem, ami a Sokoban játékból ismert módon a játékos által tolható, egyébként áthatolhatatlan
 * rakodó (`_`): a láda célhelye, azaz szintén Sokoban-elem. Az összes rakodó feltöltése esetén kijárat elérhető. Ez a célkitűzés kiegészíti a drágakövek gyűjtésével kapcsolatos célt.