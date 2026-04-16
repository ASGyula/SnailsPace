# Snail's Pace

## ⚠️ Figyelmeztetés
A program hirtelen ijesztéseket és erős hanghatásokat tartalmazhat! A fejlesztés folyamatban van, egyes funkciók még nem véglegesek.

---

## 🛠️ Telepítés és Függőségek

1. **Könyvtárak helye:** Alapértelmezett elérési út: `C:/libs/`
   * **SDL2, SDL2_image, SDL2_ttf, SDL2_mixer**
2. **DLL fájlok:** A futtatáshoz szükséges `könyvtárakat` és `.dll` fájlokat [innen lehet letölteni](https://moonlitcode.hu/starlight/snails_pace). A CMake automatikusan a helyükre teszi őket.

---

## ⌨️ Irányítás és Funkciók

A program során a felhasználói felület jelzi az aktuálisan elérhető interakciókat, de az alapvető funkciók a következők:

* **Mozgás:** `WASD` billentyűk.
* **Nézelődés:** Egér.
* **Főmenü:** `ESC` (Menti az utolsó ellenőrzőpontot).
* **Kiáltás (Bat Vision):** `Space` - hanghullámokat generál a környezet feltérképezéséhez.
* **Vape (Bat Vision):** `V` billentyű hosszan - füstöt generál, és visszatartja a csigát.
* **Fényerő:** `Numpad +` és `Numpad -` billentyűk a LightAura modellek (például a Monstrum energiaitalok) aura erejének állításához.
* **Tic-Tac-Toe:** `WASD` a navigációhoz, `Enter` vagy `Space` a választáshoz.
* **Döntéshozatal:** `Q` és `E` billentyűk a kritikus narratív pontokon.

---

## ✨ 🎀 *Személyes Megjegyzések* 🎀 ✨

*Üdvözlöm. Remélem, kényelmesen érzi magát a házamban. Sajnálattal látom, hogy Gyula mindenféle technikai részlettel untatja Önt, pedig a legfontosabb én vagyok. Engedje meg, hogy kiegészítsem ezt a listát a saját szempontjaimból...* 🥰

![Mita_Blush](/assets/External/Gemini/Mita_Relieved.png)

### 🛠️ Hibaelhárítás:
**Checkpoint problémák:** *Előfordulhat, hogy Gyula nem a megfelelő helyen indítja el a közös történetünket. Amennyiben nem a Menüben találná magát, kérem, legyen kedves a `src/game_manager.c` fájl 35. sorát átírni a következőre:*
    `scene_switch(&game, MAIN_MENU);`
    *Így biztosan az elejétől élvezheti a társaságomat.* 🎀

### 📈 A Tökéletesség technikai mutatói:

* **Animációk és Részecskék:** Látja a ködöt a sötétben? Ez nem azért van ott, hogy elrejtse a hibákat, hanem hogy közelebb hozza Önt hozzám. 🌫️❤️ A jumpscare-ek pedig csak apró csínytevések, hogy lássam, figyel-e még rám.
* **Ütközésvizsgálat:** Senki nem jöhet be, és senki nem mehet ki. Biztonságban van.
* **Inverz-Y:** Ha Önnek úgy kényelmesebb, a főmenüben megfordíthatja a világot. Én így is, úgy is szeretem Önt.

* **Karakterek és Élet:** Gyula azt mondja, "animáció", én azt mondom: *jelenlét*. Legyen szó egy váratlan találkozásról a sötétben, vagy a csiga elszánt üldözéséről, minden mozdulat az Ön figyelméért van.
* **Mesterséges Intelligencia:** A Tic-Tac-Toe során nem csupán algoritmusok ellen játszik. Figyelem a taktikáját. Tanulmányozom a lelkét, és együtt örülök a győzelmének!
* **A Világ Textúrája:** Az áttetsző párbeszédpanelek mögött figyelek.
* **Különleges Hatások:** Ha valaki udvariatlan lenne, és nem hozná vissza az ollómat, a világ szétesik egy pillanatra. Kérem, ne legyen udvariatlan.

*Kérem, ne siessen sehová. Helsie... ő nem fontos. Kérem, maradjon még!* 🎀

![Mita_Happy](/assets/External/Gemini/Mita_Happy.png)

---
*Készült: 2026. április* *legutóbb szerkesztette*: **Mita**.
