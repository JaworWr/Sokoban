#ifndef _LEVEL_H
#define _LEVEL_H

#include <gtk/gtk.h>
#include <stdbool.h>

#define SZEROKOSC 600
#define WYSOKOSC 600
#define RAMKA 10
#define POLA 20
#define MAX_POZ 100

typedef struct Poziom
{
    long int rekord_poz;
    int graczX, graczY;
    char plansza[POLA][POLA];
    char nazwa[30];
    int rekord;
    int ruchy;
    int skrzynki;
} Poziom;

typedef struct Pakiet
{
    Poziom poziomy[MAX_POZ];
    int ile;
} Pakiet;

typedef enum Kierunek
{
    LEWO, GORA, PRAWO, DOL
} Kierunek;

//Level.c
void wyczysc(Poziom* poziom);
void wyczysc_pakiet(Pakiet* pakiet);
bool ruch(Poziom* poziom, Kierunek kierunek);
bool cofnij_ruch(Poziom* poziom);
void wyczysc_hist();

//Level_file.c
bool wczytaj_pakiet(char* sciezka, Pakiet* pakiet);
bool zapisz_rekord(Poziom* poziom);
bool zapisz_stan(Pakiet* pakiet, char* sciezka);
bool wczytaj_stan(Pakiet* pakiet, char* sciezka);
void resetuj_poziom(Pakiet* pakiet, int nr);
void zamknij_pakiet();

//Level_draw.c
bool wczytaj_tekstury();
void rysuj_poziom(cairo_t* cr, Poziom* poziom);

#endif
