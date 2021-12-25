---
title: "Huffman"
course: DA3
year: 2021-2022
---

# Opgave project DA3

Dit jaar onderzoeken we in het project of het een goed idee is om in een
adaptive huffman compressieboom meerdere tekens per blad toe te laten. Zo zou er
bijvoorbeeld een blad kunnen zijn dat de tekensequentie "`oe`" voorstelt. We
zullen het algoritme implementeren en kijken naar de performantie in tijd en
ruimte van implementaties met meer of minder tekens per blad.

## Functionele vereisten programma

Schrijf een programma `persen` dat bestanden van arbitraire grootte kan
comprimeren en decomprimeren op een streaming manier.

### Comprimeren

Om te comprimeren krijgt je programma de vlag `-c` mee. Het leest dan alle
standaard invoer in en schrijft een gecomprimeerde versie naar de standaard
uitvoer. Aan de vlag `-c` word ook een waarde toegekend: twee cijfers gescheiden
door een komma ("`-c LEN,MEM`").

1. `LEN`: Het maximale aantal karakters per blad.

2. `MEM`: De geheugenlimiet, een cijfer van 0 tot en met 9 (dus grenzen
   inbegrepen). Dit cijfer geeft de maximale totale hoeveelheid geheugen aan die
   jouw programma mag gebruiken tijdens het comprimeren. Ongeacht de hoeveelheid
   data die gecomprimeerd moet worden, moet het geheugenverbruik binnen deze
   limiet blijven. Men zou verwachten dat je met een hogere geheugenlimiet een
   betere compressieratio kunt bekomen.

   |     | Geheugenlimiet |
   | --: | :------------- |
   |   0 | 50 KiB         |
   |   1 | 75 KiB         |
   |   2 | 100 KiB        |
   |   3 | 200 KiB        |
   |   4 | 1 MiB          |
   |   5 | 20 MiB         |
   |   6 | 50 MiB         |
   |   7 | 100 MiB        |
   |   8 | 500 MiB        |
   |   9 | 1 GiB          |

Als we in deze opgave spreken over een karakter dan bedoelen we altijd 8 bit.
Sommige tekens die in teksten kunnen voorkomen zijn langer dan 1 karakter. Zo is
de panda emoji (🐼) eigenlijk vier karakters lang:
`11110000 10011111 10010000 10111100`. Het tsu teken (ツ) is er 3 lang.
Natuurlijk moet jouw programma ook kunnen omgaan met deze tekens, emoji en zelfs
binaire bestanden.

```sh
# Comprimeer "cringe" met max 1 karakter per blad en max 1 GiB geheugen
echo "cringe" | persen -c 1,9 > output

# Comprimeer "¯\_(ツ)_/¯" met max 3 karakters en max 1 GiB geheugen
echo "¯\_(ツ)_/¯" | persen -c 3,9 > output

# Comprimeer "¯\_(ツ)_/¯" met max 3 karakters per blad en max 200 KiB geheugen
echo "¯\_(ツ)_/¯" | persen -c 3,2 > output

# Comprimeer "¯\_(ツ)_/¯" met max 3 karakters per blad en max 500 MiB geheugen
echo "¯\_(ツ)_/¯" | persen -c 3,8 > output

# Comprimeer een bestand met maximaal 5 karakters per blad en max 20 MiB geheugen
cat HP_and_the_Sorcerers_Stone.txt | persen -c 5,5 > output
```

### Decomprimeren

Decomprimeren gebeurt met de `-d` vlag. Als die vlag wordt meegegeven leest je
programma gecomprimeerde tekst en schrijft het de gedecomprimeerde versie naar
standaard uitvoer. Voor het decomprimeren heb je geen extra vlaggen nodig.

```sh
# Decomprimeer het bestand output
cat output | persen -d > voltekst.txt
```

### Encoderingen

De output van het Huffman algoritme is een rij van bits. Natuurlijk zullen wij
die niet uitschrijven als de strings `"1"` en `"0"`. We zouden dan nodeloos acht
keer te veel output produceren. We schrijven gewoon de bits uit.

Het adaptive Huffman algoritme in de cursus verstuurt nieuwe tekens door eerst
het pad naar het "nog niet gezien" blad uit te schrijven gevolgd door de binaire
representatie van het nieuw toe te voegen karakter. Omdat we voor dit project
meer dan één karakter per blad kunnen hebben, zullen wij het volgende
uitschrijven als we een nieuw teken willen toevoegen:

1. Het binaire pad naar "nog niet gezien" (`nng`)
2. Eén byte die de aantal karakters die we willen toevoegen encodeert
3. De binaire representatie van de tekens die we willen toevoegen

Als het pad naar `nng` bijvoorbeeld `01011` is en we nu de karaktersequentie
"YOLO" willen toevoegen aan de boom dan schrijven we het volgende bij het
comprimeren:

```txt
010110000010001011001010011110100110001001111
[nng][   4  ][   Y  ][   O  ][   L  ][   O  ]
```

Voor tekens die wel in de boom zitten, schrijven we natuurlijk gewoon het
binaire pad in de boom op. Als we "DA3" willen comprimeren en dat zit al in de
boom als `01001011110` dan schrijven we gewoon `01001011110` naar standaard
uitvoer.

Na het comprimeren van het laatste teken schrijf je het pad naar "nog niet
gezien" (`nng`) gevolgd door acht nul bits (`00000000`). Je vult de laatste byte
van de output nog op met nullen tot die vol is.

Bij het encoderen moet je gelijk welk bestand kunnen aanvaarden. Voor het het
decoderen mag je er vanuit gaan dat de input geëncodeerd is zoals hierboven
beschreven, je kunt stoppen zodra je een pad naar `nng` gevolgd door een
null-byte inleest.

## Specificaties

### Programmeertaal

In de opleidingscommissie informatica (OCI) werd beslist dat, om meer ervaring
in het programmeren in C te verwerven, het project van Algoritmen en
Datastructuren 3 in C geïmplementeerd moet worden. Het is met andere woorden de
bedoeling je implementatie in C uit te voeren. Je implementatie moet voldoen aan
de ANSI-standaard. Je mag hiervoor dus gebruikmaken van de laatste features in
C18, voor zover die ondersteund worden door `gcc` versie 10.3.0 of 11.1.0 op
SubGIT.

Voor het project kun je de standaard libraries gebruiken, externe libraries zijn
echter niet toegelaten. Het spreekt voor zich dat je normale, procedurele C-code
schrijft en geen platformspecifieke APIs (zoals bv. de Win32 API) of features
uit C++ gebruikt. Op Windows bestaat van enkele functies zoals `qsort` een
"safe" versie (in dit geval `qsort_s`), maar om je programma te kunnen
compileren op een Unix-systeem kun je die versie dus niet gebruiken.

**Wat je ontwikkelingsplatform ook mag zijn, controleer geregeld de output van
SubGIT om te verifiëren dat je programma ook bij ons compileert en slaagt voor
de minimale testen!**

### Input-Output

Voor de in- en uitvoer gebruiken we de standaard `stdin` en `stdout` streams.
Bij maximaal 1 karakter per blad moet je programma equivalent zijn aan adaptive
Huffman. Zorg ervoor dat je de output binair uitschrijft.

Omdat je zowel de compressor als de decompressor schrijft moet je programma als
volgt kunnen werken:

```sh
$ echo "waterstof" | ./persen -c 2,7 | ./persen -d
waterstof
```

### Exit code

Je programma sluit bij een correcte werking af met exitcode 0. Gaat er iets
fout, dan mag je programma niet crashen (bv segfault) maar moet het stoppen met
een exitcode die niet nul is.

### Limieten

Jouw programma moet een bestand van 1 MiB kunnen comprimeren met maximaal 3
karakters per blad in 60 seconden. Deze limieten zijn zeer ruim gerekend, je
implementatie is waarschijnlijk niet goed als je er net onder zit. Het is niet
toegestaan om bestanden aan te maken of sockets te openen vanuit je programma,
je hebt enkel de stdout en stdin filedecriptors nodig. Je programma moet single
threaded zijn en dus niet forken.

Natuurlijk mag je bij het comprimeren niet meer geheugen gebruiken dan
aangegeven met het tweede argument aan `-c`. We zullen je geheugen meten met
behulp van de `massif` tool in `valgrind`:

```sh
cat "A_melanoleuca.data" | \
   valgrind --tool=massif --stacks=yes --massif-out-file=memory.ms \
   -- \
   ./persen -c 3,9 > output
```

De bovenstaande lijn zal jouw programma (`./persen`) uitvoeren met de
[Valgrind Massif](https://www.valgrind.org/docs/manual/ms-manual.html) tool, die
zal een `memory.ms` bestand genereren die het geheugengebruik beschrijft. Je
kunt dit bestand gewoon openen met een teksteditor of je kunt `ms_print`
gebruiken om een ASCII art grafiekje te krijgen die het geheugengebruik doorheen
de tijd toont. Met de
[massif-visualizer](https://github.com/KDE/massif-visualizer) tool kun je een
mooiere weergave krijgen (met kleuren en grafieken waar je op kunt klikken).

_Tips_:

- Compileer je programma met `-g` om debuginformatie aan te zetten zodat deze
  tools mooi kunnen tonen waar er geheugen gealloceerd is.
- De dumpbestanden van Valgrind Massif gebruiken de notatie `kB` voor 1024 byte,
  een correctere notatie zou kiB zijn.
- Valgrind heeft ook een
  [memcheck](https://www.valgrind.org/docs/manual/mc-manual.html) tool waarmee
  je memory leaks kunt opsporen.
- Je programma kan tot 100 keer trager worden tijdens het runnen in `valgrind`.

## Onderzoek

Voor dit project onderzoeken we wat de beste waarden zijn voor de parameters die
je meegeeft aan `-c` in termen van:

- Compressietijd,
- Compressieratio,
- Geheugenverbruik bij compressie,
- Geheugenverbruik bij decompressie en
- Decompressietijd.

_Tip:_ Als je tijdsmetingen doet, compileer je natuurlijk met zo veel mogelijk
optimalisaties aan (compileer met `-O3`)

## Verslag

Schrijf een bondig verslag over je werk met volgende componenten:

- [ ] De ontwerpbeslissingen die je nam voor de implementatie;
- [ ] Het principe waarmee je bepaalt om meerdere tekens per blad te nemen en
      een vergelijking van de verschillende methoden die je geprobeerd hebt;
- [ ] Hoe je precies meer of minder geheugen gebruikt;
- [ ] Korte analyse van de tijdscomplexiteit van jouw programma in functie van
      de relevante parameters;
- [ ] Studie van de optimale waarde voor het comprimeren en decomprimeren.
- [ ] Leg uit welke parameters je best kunt gebruiken voor het comprimeren van:

  1. korte chatberichten,
  1. alle tweets van een bekende politicus,
  1. de broncode van het Linux-besturingssysteem,
  1. random data;

Maak je verslag niet onnodig lang.

## Indienen

### Directorystructuur

Je indiening moet volgende structuur hebben:

- `src/` bevat alle broncode (inclusief je eigen Makefiles, _exclusief
  gecompileerde bestanden_).

- `tests/` alle testcode (je moet testcode hebben, push geen grote datafiles
  naar SubGIT, plaats een scriptje die deze genereert en/of download).

- `extra/verslag.pdf` bevat de elektronische versie van je verslag. In deze map
  kun je ook eventueel extra bijlagen plaatsen.

- `assignment/` bevat de laatste versie van de opgave zoals je die binnen haalt
  van `git@SubGIT.UGent.be:2021-2022/DA3/project-assignment`. Je past best niets
  aan in deze map.

- `gcc-version` een tekstbestand met daarin de tekst "10.3.0" of "11.1.0"
  afhankelijk van welke versie van GCC je gebruikt

- `sources` is een tekstbestand dat de `.c` bronbestanden voor je project
  oplijst, 1 per lijn relatief t.o.v. de `src` directory. Bijvoorbeeld:

      main.c
      huffman/decompress.c
      huffman/compress.c

Je directory structuur ziet er dus ongeveer zo uit:

    |
    |-- extra/
    |   `-- verslag.pdf
    |-- src/
    |   `-- je broncode
    |-- tests/
    |   `-- je testcode
    |-- assignment/
    |   `-- de opgave
    |-- sources
    `-- gcc-version

Al je bestanden moeten als UTF-8 opgeslagen zijn.

### Compileren

De code zal bij het indienen gecompileerd worden met (ruwweg) onderstaande
opdracht door SubGIT met GCC-versie 10.3.0 of 11.1.0 (afhankelijk van de inhoud
van `gcc-version`) in je `src` map.

```sh
gcc -std=c18 -O3 -Wall -Werror -lm $(cat ../sources)
```

De `Dockerfile` en bijhorende bronbestanden die SubGIT gebruikt, kunnen gepulld
worden van `git@SubGIT.UGent.be:2021-2022/DA3/project-dockerfile`. Je kunt deze
Docker ook onmiddellijk van Dockerhub halen met volgende commando's:

```sh
docker pull beardhatcode/da3-project-2021-2022:latest
docker run -it --rm --mount type=bind,source={PAD},destination=/submission,readonly beardhatcode/da3-project-2021-2022:latest
```

Waarbij `{PAD}` vervangen dient te worden door het absolute pad naar de hoofdmap
(niet `src`) van je code.

### SubGIT

Het indienen gebeurt via het [SubGIT](https://SubGIT.ugent.be/) platform. Indien
je hier nog geen account op hebt, dien je deze aan te maken.

#### Repository afhalen

```bash
git clone git@SubGIT.ugent.be:2021-2022/DA3/project/{studentnr} projectDA3
```

#### Opgave als `upstream` instellen

Je kunt de opgave en boilerplate voor het project afhalen door de opgave
repository als upstream met volgende commando's in de `projectDA3` map:

```bash
git remote add upstream git@SubGIT.UGent.be:2021-2022/DA3/project-assignment
git pull upstream master
```

(Je kunt niet pushen naar de upstream, alleen de lesgevers kunnen dat.)

#### Feedback

Als je pusht naar SubGIT, zul je in je terminal te zien krijgen of je code
voldoet aan de minimumvereisten. In dat geval krijg je bij het pushen de melding
dat het pushen geslaagd is:

```
remote: Acceptable submission
```

Je kunt geen code pushen naar de `master` branch als die niet compileert of niet
aan de minimale IO vereisten voldoet. Je kunt echter wel pushen naar andere
branches en daar zal je push wel aanvaard worden.

#### De `master` branch

De `master` branch op SubGit stelt jouw indiening voor. Je kunt voor de deadline
zoveel pushen als je wilt. Zorg ervoor dat je voor de deadline zeker je finale
versie naar de **`master`** branch hebt gepusht.

#### Controleren of je zeker goed hebt ingediend

Je kunt jouw indiening bekijken door jouw repository nog eens te clonen in een
andere map

```bash
cd eenAndereMap
git clone git@SubGIT.ugent.be:2021-2022/DA3/project/{studentnr} projectDA3Controle
```

### Deadlines en belangrijke data

Zorg ervoor dat je een tussentijdse versie hebt gepusht naar een branch[^1] op
SubGIT voor **2021-11-28 om 13:37:42**. De versie die je hier upload zal niet
bekeken worden, we vragen je dit te doen om er zeker van zijn dat iedereen kan
indienen. Als je niets pusht voor deze deadline verlies je al je punten voor het
project.

De code die op **2021-12-08 om 13:37:42** op de `master` branch staat is je
finale code indiening. Enkel code die op dat moment de `master` branch staat
wordt verbeterd. **Als er geen code op je `master` branch staat krijg je nul op
vier voor het project.** Je kunt na deze deadline nog steeds pushen om
wijzigingen aan te brengen aan het verslag.

Na **2021-12-15 om 13:37:42** kun je ook helemaal geen wijzigingen meer
aanbrengen aan je repo. Het verslag dat aanwezig is als PDF op de `master`
branch in `extra/verslag.pdf` is je finaal verslag.

## Algemene richtlijnen

- Schrijf efficiënte code, maar ga niet over-optimaliseren: **geef de voorkeur
  aan elegante, goed leesbare code**. Kies zinvolle namen voor methoden en
  variabelen en voorzie voldoende commentaar.
- Op Ufora staat een Dockerfile waarmee je een Linux container kunt bouwen die
  jou code compileert en minimale testen uitvoert. Als alle testen slagen zal
  dit programma `AANVAAARD` uitschrijven als laatste regel en stoppen met exit
  code 0. Code die hier niet aan voldoet, zal geweigerd worden door SubGIT en
  levert geen punten op (0/4 voor het project).
- Het project wordt gequoteerd op **4** van de 20 te behalen punten voor dit
  vak, en deze punten worden ongewijzigd overgenomen naar de tweede
  examenperiode.
- Projecten die ons niet (via de `master` branch op SubGIT) bereiken voor de
  deadline worden niet meer verbeterd: dit betekent het verlies van alle te
  behalen punten voor het project.
- Dit is een individueel project en dient dus door jou persoonlijk gemaakt te
  worden. Het is uiteraard toegestaan om andere studenten te helpen of om ideeën
  uit te wisselen, maar **het is ten strengste verboden code uit te wisselen**,
  op welke manier dan ook. Het overnemen van code beschouwen we als fraude (van
  **beide** betrokken partijen) en zal in overeenstemming met het
  examenreglement behandeld worden. Op het internet zullen ongetwijfeld ook
  (delen van) implementaties te vinden zijn. Het overnemen of aanpassen van
  dergelijke code is echter **niet toegelaten** en wordt gezien als fraude. (Als
  je je code op GitHub/Bitbucket/sourcehut/Gitlab/… plaatst moet je die privaat
  houden tot de bekendmaking van de eerstezits examenresultaten van het vak)
- Essentiële vragen worden **niet** meer beantwoord tijdens de laatste week voor
  de deadline.

## Vragen

Op Ufora zal een lijst van veelgestelde vragen staan.

Als je vragen hebt over de opgave of problemen ondervindt, dan kun je je vraag
stellen tijdens het practicum op dinsdagvoormiddag. Alternatief kun je ook
terecht op het forum op Ufora. Contacteer je ons per mail, stuur dan ook je
studentennummer en de branch waarop de code met het probleem zit mee. Zo kunnen
we onmiddellijk je code pullen. Stuur geen screenshots van code of
foutboodschappen.

[^1]: Niet noodzakelijk `master`, moet zelfs niet compileren.
