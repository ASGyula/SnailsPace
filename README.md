# Snail's Pace

Ez egy horror-kalandjáték, amely vizuális novella elemekkel és innovatív tájékozódási mechanikákkal kísérletezik.

---

## FIGYELMEZTETÉS
A játék hirtelen ijesztéseket és erős, hirtelen hanghatásokat tartalmaz.

---

## Alapkoncepció és Játékmechanikák

A játék a hagyományos látásmódoktól eltérő, sötét világba taszítja a játékost, ahol a túlélés és a tájékozódás egyedi mechanikákon alapul:

*   **Vizuális Novella és Döntések:** A történet vizuális novella formájában bomlik ki. A dialógusokon keresztül megismerjük a főszereplőt és a rejtélyes Dealert, és még pár karaktert.
*   **Bat Vision:** A szerződés megkötése után minden már csak *hanggal* látszódik. A kiáltások (`Space`) és a lépések zaja (`WASD`) hanghullámokat generál, amelyek rövid időre felvázolják a környező drótvázas világot.
*   **Vape Mechanika:** A játékos a bat vision pályarészen használhat e-cigit. A kifújt gőz közelebb hozza a ködöt, részecskefelhőt hoz létre és lassítja az Immortal Snailt is.
*   **LiDAR:** Ha a játékos túl kíváncsi, és nem maradna Mitával, akkor egy pontfelhőből felépített folyosón kell megtalálnia a kivezető Monstrum energiaitalt. De felvedezhet valakit a sötétben...
*   **Immortal Snail:** Nem érdemes olyan szerződést kötni, amit nem olvastunk el. A szerződéskötés után a bat vision pályán (Helsie gameplay tippe után) elkezdi üldözni valami a játékost. De vajon tényleg a Dealer tette volna ezt a játékossal? Mi célja lett volna ezzel?
---

## Irányítás

*   **WASD:** Mozgás
*   **Egér:** Nézelődés
*   **BEG:** Dialógusok léptetése / UI gombok megnyomása
*   **Space:** Kiáltás
*   **Numpad + / Numpad -:** Az aura fényerősség növelése és csökkentése (a LightAuraModel-eken és Vapezés közben `V`)
*   **ESC:** Kilépés

---

## Fejlesztői beállítások

A projekt lefordításához és futtatásához szükségesek az **SDL2** könyvtárak. A beállítás jelenleg Windows környezetre van optimalizálva.

### Windows
1.  **Könyvtárak elhelyezése:** A függőségeket az alábbi útvonalon keresi a rendszer hardcodeolva: `C:/libs/`
    * Szükséges modulok: **SDL2, SDL2_image, SDL2_ttf, SDL2_mixer**
2. **Beállítás:** A futtatáshoz szükséges `asseteket`, `könyvtárakat` és `.dll` fájlokat [innen lehet letölteni](https://moonlitcode.hu/starlight/snails_pace). A dll fájlokat a CMake automatikusan a helyükre teszi őket, az asseteket a gyökér könyvtárba kell másolni (`assets/`).

## Hibaelhárítás és Fejlesztői megjegyzések

### Belépési pont módosítása
Amennyiben a program indításkor nem a főmenübe tölt be, a kezdő jelenet manuálisan felülbírálható. A `src/game_manager.c` fájl 50. sorát kell módosítani az alábbira:

`scene_switch(&game, MAIN_MENU);`

### Technikai paraméterek
* **Grafika:** A szoftver egyedi részecskerendszert használ a ködeffektekhez és a vizuális visszacsatoláshoz.
* **Ütközésdetektálás:** Fal- és trigger alapú ütközésvizsgálat.
* **Kamera:** Invertált Y-tengely opció elérhető a főmenüben.
* **AI:** Üldözések és Tic-Tac-Toe AI implementálása.
* **Különleges hatások**: Chromatic effect jelenik meg a MITA_SAVES_PLAYER jelenetben pár dialógus elejéig, ha a játékos nem viszi vissza Mitának az ollót, és a hang után kutakodik.