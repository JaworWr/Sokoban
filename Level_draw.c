#include "Level.h"

cairo_surface_t* skrzynka1=NULL;
cairo_surface_t* skrzynka2=NULL;
cairo_surface_t* gracz=NULL;
cairo_surface_t* sciana=NULL;

bool sprawdz_tekstury()
{
    return cairo_surface_status(skrzynka1)==CAIRO_STATUS_SUCCESS
        && cairo_surface_status(skrzynka2)==CAIRO_STATUS_SUCCESS
        && cairo_surface_status(gracz)==CAIRO_STATUS_SUCCESS
        && cairo_surface_status(sciana)==CAIRO_STATUS_SUCCESS;
}

bool wczytaj_tekstury()
{
    skrzynka1=cairo_image_surface_create_from_png("Skrzynka1.png");
    skrzynka2=cairo_image_surface_create_from_png("Skrzynka2.png");
    gracz=cairo_image_surface_create_from_png("Gracz.png");
    sciana=cairo_image_surface_create_from_png("Sciana.png");
    return sprawdz_tekstury();
}

void rysuj_poziom(cairo_t* cr, Poziom* poziom)
{
    if(!sprawdz_tekstury())
        return;
    for(int i=0; i<20; i++)
    {
        for(int j=0; j<20; j++)
        {
            switch(poziom->plansza[i][j])
            {
                case '$':
                    cairo_set_source_surface(cr, skrzynka1, RAMKA+1+j*30, RAMKA+1+i*30);
                    cairo_paint(cr);
                    break;
                case '*':
                    cairo_set_source_surface(cr, skrzynka2, RAMKA+1+j*30, RAMKA+1+i*30);
                    cairo_paint(cr);
                    break;
                case '@':
                case '+':
                    cairo_set_source_surface(cr, gracz, RAMKA+1+j*30, RAMKA+1+i*30);
                    cairo_paint(cr);
                    break;
                case '#':
                    cairo_set_source_surface(cr, sciana, RAMKA+1+j*30, RAMKA+1+i*30);
                    cairo_paint(cr);
                    break;
                case '.':
                    cairo_rectangle(cr, RAMKA+10+j*30, RAMKA+10+i*30, 10, 10);
                    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
                    cairo_fill(cr);
                    break;
                default:;
            }
        }
    }
    if(poziom->skrzynki==0)
    {
        cairo_set_font_size(cr, 30);
        cairo_text_extents_t wymiary;
        cairo_text_extents(cr, "Poziom ukończony!", &wymiary);
        cairo_rectangle(cr, SZEROKOSC/2+RAMKA-wymiary.width/2-2, WYSOKOSC/2+RAMKA-wymiary.height/2+-2, wymiary.width+4, wymiary.height+4);
        cairo_set_source_rgba(cr, 0, 0.2, 0, 0.85);
        cairo_fill(cr);
        cairo_move_to(cr, SZEROKOSC/2+RAMKA-wymiary.width/2-wymiary.x_bearing, WYSOKOSC/2+RAMKA-wymiary.height/2-wymiary.y_bearing);
        cairo_text_path(cr, "Poziom ukończony!");
        cairo_set_source_rgba(cr, 1, 1, 1, 0.85);
        cairo_fill(cr);
    }
}
