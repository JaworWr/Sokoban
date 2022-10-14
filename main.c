#include "Level.h"
#include "Gtk_dialogs.h"
#include <stdio.h>

#define _dana(i) ((GtkWidget**)dane)[i]

enum Dane
{
    OKNO=0,
    RYS=1,
    NAZWA=2,
    NR=3,
    RUCHY=4,
    NAJMNIEJ=5,
    WCZYTAJ=6,
    ZAPISZ=7
};

Pakiet aktualny;
int aktualny_poziom=-1;

void zapisz_liczbe(GtkEntry* cel, int liczba)
{
    char tekst[10];
    sprintf(tekst, "%d", liczba);
    gtk_entry_set_text(cel, tekst);
}

void aktualizuj_nazwe(GtkEntry* cel)
{
    if(aktualny.poziomy[aktualny_poziom].nazwa[0]=='\0')
    {
        char nazwa[30];
        sprintf(nazwa, "Poziom %d", aktualny_poziom+1);
        gtk_entry_set_text(cel, nazwa);
    }
    else
        gtk_entry_set_text(cel, aktualny.poziomy[aktualny_poziom].nazwa);
}

void ustaw_focus(GtkWidget* widget, GdkEvent* zdarzenie, gpointer dane)
{
    gtk_window_set_focus(GTK_WINDOW(dane), widget);
}

void otworz_pakiet(GtkWidget* widget, gpointer dane)
{
    char sciezka[1000];
    if(wczytaj_dialog(_dana(OKNO), sciezka))
    {
        if(wczytaj_pakiet(sciezka, &aktualny))
        {
            aktualny_poziom=0;
            zapisz_liczbe(GTK_ENTRY(_dana(NR)), aktualny_poziom+1);
            aktualizuj_nazwe(GTK_ENTRY(_dana(NAZWA)));
            wyczysc_hist();
            zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
            if(aktualny.poziomy[aktualny_poziom].rekord>-1)
                zapisz_liczbe(GTK_ENTRY(_dana(NAJMNIEJ)), aktualny.poziomy[aktualny_poziom].rekord);
            else
                gtk_entry_set_text(GTK_ENTRY(_dana(NAJMNIEJ)),"");
            gtk_widget_set_sensitive(GTK_WIDGET(_dana(ZAPISZ)), TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(_dana(WCZYTAJ)), TRUE);
            utworz_domyslna_nazwe(sciezka);
            gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
        }
        else
        {
            wyswietl_blad(_dana(OKNO), "Nieprawidłowy plik");
            gtk_widget_set_sensitive(GTK_WIDGET(_dana(ZAPISZ)), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(_dana(WCZYTAJ)), FALSE);
            aktualny_poziom=-1;
            aktualny.ile=0;
            gtk_entry_set_text(GTK_ENTRY(_dana(NAZWA)), "Nie wybrano pakietu");
            gtk_entry_set_text(GTK_ENTRY(_dana(NR)), "");
            gtk_entry_set_text(GTK_ENTRY(_dana(RUCHY)), "");
            gtk_entry_set_text(GTK_ENTRY(_dana(NAJMNIEJ)), "");
        }
    }
}

void wczytaj(GtkWidget* widget, gpointer dane)
{
    char sciezka[1000];
    if(wczytaj_dialog(_dana(OKNO), sciezka))
    {
        if(wczytaj_stan(&aktualny, sciezka))
        {
            gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
            wyczysc_hist();
            zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
        }
        else
            wyswietl_blad(_dana(OKNO), "Nieprawidłowy zapis dla tego pakietu");
    }
}

void zapisz(GtkWidget* widget, gpointer dane)
{
    char sciezka[1000];
    if(zapisz_dialog(_dana(OKNO), sciezka))
    {
        if(!zapisz_stan(&aktualny, sciezka))
            wyswietl_blad(_dana(OKNO), "Podczas zapisywania wystąpił błąd");
    }
}

void wcisn_klawisz(GtkWidget* widget, GdkEventKey* klawisz, gpointer dane)
{
    if(aktualny_poziom<0 || aktualny.poziomy[aktualny_poziom].skrzynki==0)
        return;
    bool czy_ruch=false;
    switch(klawisz->keyval)
    {
        case GDK_KEY_Left:
        case GDK_KEY_a:
        case GDK_KEY_A:
            czy_ruch=ruch(&aktualny.poziomy[aktualny_poziom], LEWO);
            break;
        case GDK_KEY_Up:
        case GDK_KEY_w:
        case GDK_KEY_W:
            czy_ruch=ruch(&aktualny.poziomy[aktualny_poziom], GORA);
            break;
        case GDK_KEY_Right:
        case GDK_KEY_d:
        case GDK_KEY_D:
            czy_ruch=ruch(&aktualny.poziomy[aktualny_poziom], PRAWO);
            break;
        case GDK_KEY_Down:
        case GDK_KEY_s:
        case GDK_KEY_S:
            czy_ruch=ruch(&aktualny.poziomy[aktualny_poziom], DOL);
            break;
        default:;
    }
    if(czy_ruch)
    {
        gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
        aktualny.poziomy[aktualny_poziom].ruchy++;
        zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
        if(aktualny.poziomy[aktualny_poziom].skrzynki==0 && (aktualny.poziomy[aktualny_poziom].rekord==-1 ||
            aktualny.poziomy[aktualny_poziom].rekord>aktualny.poziomy[aktualny_poziom].ruchy))
        {
            aktualny.poziomy[aktualny_poziom].rekord=aktualny.poziomy[aktualny_poziom].ruchy;
            zapisz_liczbe(GTK_ENTRY(_dana(NAJMNIEJ)), aktualny.poziomy[aktualny_poziom].rekord);
            if(!zapisz_rekord(&aktualny.poziomy[aktualny_poziom]))
                wyswietl_blad(_dana(OKNO), "Podczas zapisywania rekordu wystąpił błąd. Wczytaj pakiet ponownie.");
        }
    }
}

void resetuj(GtkWidget* widget, gpointer dane)
{
    if(aktualny_poziom<0)
        return;
    resetuj_poziom(&aktualny, aktualny_poziom);
    gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
    wyczysc_hist();
    aktualny.poziomy[aktualny_poziom].ruchy=0;
    zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
}

void poprz_poz(GtkWidget* widget, gpointer dane)
{
    if(aktualny_poziom<=0)
        return;
    aktualny_poziom--;
    gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
    zapisz_liczbe(GTK_ENTRY(_dana(NR)), aktualny_poziom+1);
    aktualizuj_nazwe(GTK_ENTRY(_dana(NAZWA)));
    wyczysc_hist();
    zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
    if(aktualny.poziomy[aktualny_poziom].rekord>-1)
        zapisz_liczbe(GTK_ENTRY(_dana(NAJMNIEJ)), aktualny.poziomy[aktualny_poziom].rekord);
    else
        gtk_entry_set_text(GTK_ENTRY(_dana(NAJMNIEJ)),"");
}

void nast_poz(GtkWidget* widget, gpointer dane)
{
    if(aktualny_poziom<0 || aktualny_poziom>=aktualny.ile-1)
        return;
    aktualny_poziom++;
    gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
    zapisz_liczbe(GTK_ENTRY(_dana(NR)), aktualny_poziom+1);
    aktualizuj_nazwe(GTK_ENTRY(_dana(NAZWA)));
    wyczysc_hist();
    zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
    if(aktualny.poziomy[aktualny_poziom].rekord>-1)
        zapisz_liczbe(GTK_ENTRY(_dana(NAJMNIEJ)), aktualny.poziomy[aktualny_poziom].rekord);
    else
        gtk_entry_set_text(GTK_ENTRY(_dana(NAJMNIEJ)),"");
}

void cofnij(GtkWidget* widget, gpointer dane)
{
    if(aktualny_poziom<0)
        return;
    if(cofnij_ruch(&aktualny.poziomy[aktualny_poziom]))
    {
        gtk_widget_queue_draw(GTK_WIDGET(_dana(RYS)));
        aktualny.poziomy[aktualny_poziom].ruchy--;
        zapisz_liczbe(GTK_ENTRY(_dana(RUCHY)), aktualny.poziomy[aktualny_poziom].ruchy);
    }
}

static void rysuj(GtkWidget* widget, cairo_t* cr)
{
    int szer=gtk_widget_get_allocated_width(widget);
    int wys=gtk_widget_get_allocated_height(widget);
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);
    for(int i=1; i<20; i++)
    {
        cairo_move_to(cr, RAMKA+i*30, 0);
        cairo_rel_line_to(cr, 0, wys);

        cairo_move_to(cr, 0, RAMKA+i*30);
        cairo_rel_line_to(cr, szer, 0);
    }
    cairo_stroke(cr);
    cairo_rectangle(cr, RAMKA/2, RAMKA/2, szer-RAMKA, wys-RAMKA);
    if(!gtk_widget_has_focus(widget))
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_set_line_width(cr, RAMKA+2);
    cairo_stroke(cr);
    if(aktualny_poziom>=0)
        rysuj_poziom(cr, aktualny.poziomy+aktualny_poziom);
}

int main(int argc, char* argv[])
{
    aktualny.ile=0;
    gtk_init(&argc, &argv);

    if(!wczytaj_tekstury())
    {
        wyswietl_blad(NULL, "Błąd: Brakujące pliki");
        return 1;
    }

    GtkWidget* dane[8];

    GtkWidget* okno=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    dane[OKNO]=okno;
    gtk_window_set_title(GTK_WINDOW(okno), "Sokoban");
    gtk_window_set_position(GTK_WINDOW(okno), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(okno), 0);

    GtkWidget* krata=gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(okno), krata);

    GtkWidget* rys=gtk_drawing_area_new();
    dane[RYS]=rys;

    GtkWidget* pasek=gtk_menu_bar_new();

    GtkWidget* plik_mi=gtk_menu_item_new_with_mnemonic("_Plik");
    GtkWidget* plik_menu=gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(plik_mi), plik_menu);

    GtkWidget* z_pliku_mi=gtk_menu_item_new_with_mnemonic("_Wczytaj poziomy");
    gtk_menu_shell_append(GTK_MENU_SHELL(plik_menu), z_pliku_mi);
    g_signal_connect(G_OBJECT(z_pliku_mi), "activate", G_CALLBACK(otworz_pakiet), (gpointer)dane);

    GtkWidget* wczytaj_stan_mi=gtk_menu_item_new_with_mnemonic("W_czytaj stan gry");
    gtk_menu_shell_append(GTK_MENU_SHELL(plik_menu), wczytaj_stan_mi);
    gtk_widget_set_sensitive(wczytaj_stan_mi, FALSE);
    dane[WCZYTAJ]=wczytaj_stan_mi;
    g_signal_connect(G_OBJECT(wczytaj_stan_mi), "activate", G_CALLBACK(wczytaj), (gpointer)dane);

    GtkWidget* zapisz_stan_mi=gtk_menu_item_new_with_mnemonic("Za_pisz stan gry");
    gtk_menu_shell_append(GTK_MENU_SHELL(plik_menu), zapisz_stan_mi);
    gtk_widget_set_sensitive(zapisz_stan_mi, FALSE);
    dane[ZAPISZ]=zapisz_stan_mi;
    g_signal_connect(G_OBJECT(zapisz_stan_mi), "activate", G_CALLBACK(zapisz), (gpointer)dane);

    GtkWidget* zakoncz_mi=gtk_menu_item_new_with_mnemonic("_Zakończ");
    gtk_menu_shell_append(GTK_MENU_SHELL(plik_menu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(plik_menu), zakoncz_mi);
    g_signal_connect(G_OBJECT(zakoncz_mi), "activate", G_CALLBACK(gtk_main_quit), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(pasek), plik_mi);
    gtk_grid_attach(GTK_GRID(krata), pasek, 0, 0, 12, 1);

    gtk_widget_set_size_request(rys, SZEROKOSC+2*RAMKA, WYSOKOSC+2*RAMKA);
    gtk_grid_attach(GTK_GRID(krata), rys, 0, 1, 12, 1);
    gtk_widget_set_can_focus(rys, TRUE);
    gtk_widget_add_events(rys, GDK_KEY_PRESS_MASK|GDK_BUTTON1_MASK);
    g_signal_connect(G_OBJECT(rys), "draw", G_CALLBACK(rysuj), NULL);
    g_signal_connect(G_OBJECT(rys), "key_press_event", G_CALLBACK(wcisn_klawisz), (gpointer)dane);
    g_signal_connect(G_OBJECT(rys), "button_press_event", G_CALLBACK(ustaw_focus), okno);
    g_signal_connect(G_OBJECT(rys), "focus_in_event", G_CALLBACK(gtk_widget_queue_draw), NULL);
    g_signal_connect(G_OBJECT(rys), "focus_out_event", G_CALLBACK(gtk_widget_queue_draw), NULL);

    GtkWidget* nazwa_poz=gtk_entry_new();
    dane[NAZWA]=nazwa_poz;
    gtk_grid_attach(GTK_GRID(krata), nazwa_poz, 0, 2, 5, 1);
    gtk_editable_set_editable(GTK_EDITABLE(nazwa_poz), FALSE);
    gtk_widget_set_can_focus(nazwa_poz, FALSE);
    gtk_entry_set_text(GTK_ENTRY(nazwa_poz), "Nie wczytano pakietu");

    GtkWidget* poprz_prz=gtk_button_new_with_label("<");
    gtk_grid_attach(GTK_GRID(krata), poprz_prz, 5, 2, 1, 1);
    g_signal_connect(G_OBJECT(poprz_prz), "clicked", G_CALLBACK(poprz_poz), (gpointer)dane);

    GtkWidget* nr_poz=gtk_entry_new();
    dane[NR]=nr_poz;
    gtk_grid_attach(GTK_GRID(krata), nr_poz, 6, 2, 1, 1);
    gtk_editable_set_editable(GTK_EDITABLE(nr_poz), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(nr_poz), 3);
    gtk_entry_set_width_chars(GTK_ENTRY(nr_poz), 3);
    gtk_entry_set_alignment(GTK_ENTRY(nr_poz), 0.5);
    gtk_widget_set_can_focus(nr_poz, FALSE);

    GtkWidget* nast_prz=gtk_button_new_with_label(">");
    gtk_grid_attach(GTK_GRID(krata), nast_prz, 7, 2, 1, 1);
    g_signal_connect(G_OBJECT(nast_prz), "clicked", G_CALLBACK(nast_poz), (gpointer)dane);

    GtkWidget* cofnij_prz=gtk_button_new_with_label("Cofnij");
    gtk_grid_attach(GTK_GRID(krata), cofnij_prz, 8, 3, 2, 1);
    g_signal_connect(G_OBJECT(cofnij_prz), "clicked", G_CALLBACK(cofnij), (gpointer) dane);

    GtkWidget* reset_prz=gtk_button_new_with_label("Resetuj");
    gtk_grid_attach(GTK_GRID(krata), reset_prz, 10, 3, 2, 1);
    g_signal_connect(G_OBJECT(reset_prz), "clicked", G_CALLBACK(resetuj), (gpointer)dane);

    GtkWidget* ruchy_et=gtk_label_new("Ruchy:");
    gtk_grid_attach(GTK_GRID(krata), ruchy_et, 8, 2, 1, 1);
    gtk_label_set_xalign(GTK_LABEL(ruchy_et), 1);

    GtkWidget* ile_ruchow=gtk_entry_new();
    dane[RUCHY]=ile_ruchow;
    gtk_grid_attach(GTK_GRID(krata), ile_ruchow, 9, 2, 1, 1);
    gtk_editable_set_editable(GTK_EDITABLE(ile_ruchow), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(ile_ruchow), 6);
    gtk_entry_set_width_chars(GTK_ENTRY(ile_ruchow), 6);
    gtk_entry_set_alignment(GTK_ENTRY(ile_ruchow), 0.5);
    gtk_widget_set_can_focus(ile_ruchow, FALSE);

    GtkWidget* najmniej_et=gtk_label_new("Najmniej:");
    gtk_grid_attach(GTK_GRID(krata), najmniej_et, 10, 2, 1, 1);
    gtk_label_set_xalign(GTK_LABEL(najmniej_et), 1);

    GtkWidget* ile_najmniej=gtk_entry_new();
    dane[NAJMNIEJ]=ile_najmniej;
    gtk_grid_attach(GTK_GRID(krata), ile_najmniej, 11, 2, 1, 1);
    gtk_editable_set_editable(GTK_EDITABLE(ile_najmniej), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(ile_najmniej), 6);
    gtk_entry_set_width_chars(GTK_ENTRY(ile_najmniej), 6);
    gtk_entry_set_alignment(GTK_ENTRY(ile_najmniej), 0.5);
    gtk_widget_set_can_focus(ile_najmniej, FALSE);

    g_signal_connect(G_OBJECT(okno), "destroy", G_CALLBACK(zamknij_pakiet), NULL);
    g_signal_connect(G_OBJECT(okno), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(okno);

    gtk_main();

    return 0;
}
