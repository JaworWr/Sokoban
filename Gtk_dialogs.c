#include "Gtk_dialogs.h"
#include <string.h>

char obecny_zapis[1000]="";

GtkFileFilter* utworz_filtr()
{
    GtkFileFilter* filtr=gtk_file_filter_new();
    gtk_file_filter_add_pattern(filtr, "*.txt");
    gtk_file_filter_set_name(filtr, "Pliki tekstowe");
    return filtr;
}

const char* znajdz_ukosnik(const char* sciezka)
{
#ifdef _WIN32
    const char* ukosnik=strrchr(sciezka, '\\');
#else
    const char* ukosnik=strrchr(sciezka, '/');
#endif
    if(ukosnik==NULL)
        ukosnik=sciezka;
    else
        ukosnik++;
    return ukosnik;
}

void zmien_rozszerzenie(char* sciezka)
{
    int n=strlen(sciezka);
    if(sciezka[n-4]!='.' || sciezka[n-3]!='t' || sciezka[n-2]!='x' || sciezka[n-1]!='t')
    {
        sprintf(sciezka, "%s.txt", sciezka);
    }
}

void utworz_domyslna_nazwe(const char* sciezka)
{
    const char* ukosnik=znajdz_ukosnik(sciezka);
    int l=strlen(ukosnik)-4;
    strncpy(obecny_zapis, ukosnik, l);
    strcpy(obecny_zapis+l, "-zapis.txt");
}

bool wczytaj_dialog(GtkWidget* okno, char* sciezka)
{
    GtkWidget* dialog=gtk_file_chooser_dialog_new("Wybierz plik"
                                                , GTK_WINDOW(GTK_WINDOW(okno))
                                                , GTK_FILE_CHOOSER_ACTION_OPEN
                                                , "_Anuluj", GTK_RESPONSE_CANCEL
                                                , "_Otwórz", GTK_RESPONSE_ACCEPT
                                                , NULL);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), utworz_filtr());
    int odp=gtk_dialog_run(GTK_DIALOG(dialog));
    if(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog))!=NULL)
        strcpy(sciezka, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    gtk_widget_destroy(dialog);
    return odp==GTK_RESPONSE_ACCEPT;
}

bool zapisz_dialog(GtkWidget* okno, char* sciezka)
{
    GtkWidget* dialog=gtk_file_chooser_dialog_new("Wybierz plik"
                                                , GTK_WINDOW(GTK_WINDOW(okno))
                                                , GTK_FILE_CHOOSER_ACTION_SAVE
                                                , "_Anuluj", GTK_RESPONSE_CANCEL
                                                , "_Zapisz", GTK_RESPONSE_ACCEPT
                                                , NULL);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), utworz_filtr());
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), obecny_zapis);
    int odp=gtk_dialog_run(GTK_DIALOG(dialog));
    if(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog))!=NULL)
        strcpy(sciezka, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    gtk_widget_destroy(dialog);
    if(odp==GTK_RESPONSE_ACCEPT)
    {
        zmien_rozszerzenie(sciezka);
        strcpy(obecny_zapis, znajdz_ukosnik(sciezka));
    }
    return odp==GTK_RESPONSE_ACCEPT;
}

void wyswietl_blad(GtkWidget* okno, const char* wiadomosc)
{
    GtkWidget* dialog=gtk_message_dialog_new(GTK_WINDOW(okno)
                                            , GTK_DIALOG_DESTROY_WITH_PARENT
                                            , GTK_MESSAGE_ERROR
                                            , GTK_BUTTONS_CLOSE
                                            , "%s", wiadomosc);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
