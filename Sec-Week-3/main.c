/*
 * C Security Practicum 3
 * Onderwerpen: Integer overflow, Format string, Memory dump
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

int main(void) {

    /* ============================================================
     *  INTEGER OVERFLOW
     * ============================================================ */

    printf("\n=== Integer Overflow 1: Two's complement ===\n");
    /*
     * Een signed int wordt opgeslagen als two's complement.
     * Voor -1 zie je in geheugen alleen maar FF FF FF FF staan,
     * dat is precies wat two's complement doet: alle bits 1.
     * Voor INT32_MIN zie je 00 00 00 80 (little endian),
     * dus bit 31 staat aan en de rest is 0.
     */
    int32_t a = -1;          /* in memory: FF FF FF FF */
    int32_t b = 1;           /* in memory: 01 00 00 00 */
    int32_t c = 0;           /* in memory: 00 00 00 00 */
    int32_t d = INT32_MIN;   /* in memory: 00 00 00 80 */
    printf("a=%d  b=%d  c=%d  d=%d\n", a, b, c, d);
    printf("kijk in Memory Window naar adres &a om de bits te zien\n");


    printf("\n=== Integer Overflow 2: signed getal max en min ===\n");
    /*
     * int16_t (16 bits, signed).
     * Bij n bits signed geldt:
     *   max =  2^(n-1) - 1
     *   min = -2^(n-1)
     * Dus voor 16 bits:  max = 32767, min = -32768
     */
    int16_t getal = 0;
    int16_t maxWaarde = 32767;    /*  2^15 - 1 */
    int16_t minWaarde = -32768;   /* -2^15     */
    printf("int16_t  max = %d   min = %d\n", maxWaarde, minWaarde);
    printf("ter controle via stdint.h: INT16_MAX=%d  INT16_MIN=%d\n",
        INT16_MAX, INT16_MIN);
    getal = maxWaarde; /* om gebruikt te zijn */


    printf("\n=== Integer Overflow 3: overflow voorbeeld ===\n");
    /*
     * Je begin op de max waarde en tel er 1 bij op.
     * Resultaat is niet 32768 maar -32768 (klapt om naar min).
     * Dit komt door two's complement: de bit-patronen lopen rond.
     */
    int16_t over = INT16_MAX;
    printf("voor  overflow: %d\n", over);
    over = over + 1;
    printf("na    overflow: %d  (verwacht zou 32768 zijn)\n", over);


    printf("\n=== Integer Overflow 4: underflow voorbeeld (signed) ===\n");
    /*
     * Andersom: begin bij INT16_MIN en trek er 1 vanaf.
     * Resultaat klapt om naar INT16_MAX = 32767.
     */
    int16_t under = INT16_MIN;
    printf("voor  underflow: %d\n", under);
    under = under - 1;
    printf("na    underflow: %d  (verwacht zou -32769 zijn)\n", under);


    printf("\n=== Integer Overflow 5: underflow unsigned ===\n");
    /*
     * Een unsigned getal kan niet negatief worden.
     * Als je 0 met 1 verlaag, dan rolt hij naar het maximum.
     * Voor uint16_t is dat 65535.
     */
    uint16_t u = 0;
    printf("voor  underflow: %u\n", u);
    u = u - 1;
    printf("na    underflow: %u  (rolt naar UINT16_MAX)\n", u);

    printf("\n=== Integer Overflow 8: casting zonder problemen ===\n");
    /*
     * Als je van een klein type naar een groter type cast,
     * past de waarde altijd. Hier gaat int8_t naar int32_t.
     * Dit kan nooit fout gaan want het bereik van de bron
     * past volledig in het doel.
     */
    int8_t klein = 100;
    int32_t groot = (int32_t)klein;
    printf("int8_t %d  ->  int32_t %d  (past altijd)\n", klein, groot);


    printf("\n=== Integer Overflow 9: casting met soms problemen ===\n");
    /*
     * Andersom is wel link: van groot naar klein type.
     * Als de waarde past gaat het goed, anders niet.
     */
    int32_t veilig = 100;             /* past wel in int8_t */
    int32_t onveilig = 300;           /* past NIET in int8_t (max 127) */
    int8_t r1 = (int8_t)veilig;
    int8_t r2 = (int8_t)onveilig;
    printf("zonder probleem:  %d  ->  %d\n", veilig, r1);
    printf("met    probleem:  %d  ->  %d  (afgekapt!)\n", onveilig, r2);


    /* ============================================================D
     *  FORMAT STRING
     * ============================================================ */

    printf("\n=== Format String 1: veilige en kwetsbare variant ===\n");
    /*
     * laat de gebruiker een naam invoeren.
     * - Veilige variant: gebruikt "%s" als format string.
     * - Kwetsbare variant: stopt de gebruikersinvoer rechtstreeks
     *   in de format string. Als de gebruiker dan %s of %x invoert
     *   gaat printf in het geheugen lopen lezen / schrijven.
     */
    char naam[64];
    printf("voer je naam in: ");
    scanf("%63s", naam);

    printf("\n-- veilig --\n");
    printf("Hallo %s!\n", naam);  /* netjes, naam is alleen data */

    printf("\n-- kwetsbaar --\n");
    /*
     * Als je hier %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s invoert
     * gaat printf waardes van de stack lezen en kan crashen.
     */
    printf(naam);                  /* <-- format string bug */
    printf("\n");
    
    /* ============================================================
     *  MEMORY DUMP
     * ============================================================ */

    printf("\n=== Memory Dump 1: input in buffer terugvinden ===\n");
    /*
     * lees een string in en zet die op de stack.
     * Daarna kan je via Debug -> Windows -> Memory in Visual Studio
     * naar het adres van buffer kijken en zie je letterlijk
     * mijn ingevoerde tekst in het geheugen staan.
     * Met de Binary Editor kan je hetzelfde doen op een .exe of dump.
     */
    char buffer[64];
    printf("typ een geheime tekst: ");
    scanf("%63s", buffer);
    printf("ingevoerd: %s\n", buffer);
    printf("kijk in Memory Window op adres &buffer \n");


    printf("\n=== Memory Dump 2: memset wissen, verschil debug/release ===\n");
    /*
     * Nu maak je de buffer leeg met memset zodat de tekst
     * weg is uit het geheugen. Daarna gebruik je de buffer
     * niet meer.
     *
     * Verschil:
     *  - Debug mode: de compiler doet geen agressieve optimalisaties,
     *    dus de memset wordt netjes uitgevoerd. Tekst is weg.
     *    MAAR: in debug staan vaak nog kopieen van de tekst elders
     *    op de stack (oude frames, niet opgeruimd), dus daar kan
     *    je het soms nog vinden.
     *  - Release mode: de optimizer ziet dat de buffer na de memset
     *    niet meer gebruikt wordt, en gooit de memset gewoon weg
     *    ("dead store elimination"). Daardoor staat de tekst er
     *    nog wel in! Dit is een bekend security probleem,
     *    daarom bestaat memset_s / SecureZeroMemory die de
     *    compiler niet mag weg-optimaliseren.
     */
    printf("buffer adres: %p\n", (void*)buffer);                //Release Mode
    memset(buffer, 0, sizeof(buffer));
    printf("buffer is gewist met memset\n");
    printf("kijk nu opnieuw in Memory Window naar &buffer\n");
    printf("vergelijk Debug build met Release build\n");

    return 0;
}
