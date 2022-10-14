#ifndef _GTK_DIALOGS_H
#define _GTK_DIALOGS_H

#include <gtk/gtk.h>
#include <stdbool.h>

void utworz_domyslna_nazwe(const char* sciezka);
bool wczytaj_dialog(GtkWidget* okno, char* sciezka);
bool zapisz_dialog(GtkWidget* okno, char* sciezka);

void wyswietl_blad(GtkWidget* okno, const char* wiadomosc);

#endif
