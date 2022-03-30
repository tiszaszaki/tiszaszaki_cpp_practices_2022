# CYK-algoritmus példaprogram

## Forrás

http://aszt.inf.elte.hu/~hunlaci/gyakorlat2011t/gy08H.pdf

## Használata

### A program paraméterei
 1. nyelvtan-fájl
 2. felismerendő szó
 3. kimeneti fájl (opcionális)

### Példa futtatásra

`> CYK_algoritmus_2021.exe CF_nyelv_3.txt 128+384 CF_nyelv_3.log`

`> CYK_algoritmus_2021.exe REG_nyelv.txt 128`

## A paraméterek leírása

### A nyelvtan-fájl

A nyelvtant definiáló fájl elérési útvonala.

Példa:
```
S

SAB SFB
BCA BCF BCS

C+ C- C* C/

ADF DDF AEF DEF
SDF SEF

F0 F1 F2 F3 F4 F5 F6 F7 F8 F9
E1 E2 E3 E4 E5 E6 E7 E8 E9
S0 S1 S2 S3 S4 S5 S6 S7 S8 S9
```

A nyelvtan-fájlban a paraméterek szabadon tördelhetők.
Az első kifejezés a kezdőszimbólum.
Utána pedig következnek a levezetési szabályok. Ezek csak kettő vagy három betűből állhatnak. Ezek ún. Chomsky-normálformának megfelelően vannak átalakítva, azaz vagy egy terminális vagy két nemterminális szimbólumot vezetnek le.

### A felismerendő szó

Egy karakterlánc, amire a levezetési fát felépítjük a megadott nyelvtan alapján. Mivel rögzített ábécé nincsen, ezért tetszőleges karakter szerephet benne. A CYK-algormitmus esetén előfordulhat, hogy a szó egy karakterére megáll, mert nem talál olyan szabályt a nyelvtanból, amelynek a jobb oldalán szerepelne. Ilyen esetben elutasítja az adott szót.

### A kimeneti fájl

A kimeneti fájl elérési útvonala. A fájl elején a nyelvtan-fájl feldolgozásának a megjelenítése történik. A második felében a levezetés ún. CYK-táblában (a fájlban mátrixként) jelenik meg, amelynek a balfelső cellája tartalmazza az eldöntéshez szükséges szimbólumok halmazát Rész-szavak felismeréséhez tárolja a mátrix többi cellájában is a levezetéskor felfedezett nemterminálisokat is. A végén pedig kiírja a felismerés eredményét, azaz "elfogadva" vagy "elutasitva". Utána pedig kiírja a felismert szót, esetleg azokat a rész-szavakat, amelyek kiolvashatóak a kitöltött mátrixból. Azon cellákból, amelyekben a kezdőszimbólum szerepel, az indexei segítségével a rész-szó eleje és vége meghatározható. Csak a bemeneti szó elfogadása esetén lesznek kiíratva a felismert rész-szavak is.