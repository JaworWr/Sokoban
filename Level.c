#include "Level.h"

#define RUCH 16
#define COFNIECIA 3

Kierunek ostatnie[COFNIECIA];

void wyczysc(Poziom* poziom)
{
    poziom->nazwa[0]='\0';
    poziom->rekord=-1;
    for(int i=0; i<20; i++)
        for(int j=0; j<20; j++)
            poziom->plansza[i][j]=' ';
}

void wyczysc_pakiet(Pakiet* pakiet)
{
    pakiet->ile=0;
}

void usun(char* pole)
{
    if(*pole=='@' || *pole=='$')
        *pole=' ';
    else
        *pole='.';
}

bool ruch(Poziom* poziom, Kierunek kierunek)
{
    char* p=&poziom->plansza[poziom->graczY][poziom->graczX];
    char* p1;
    char* p2;
    bool skrzynka=false;
    switch(kierunek)
    {
        case LEWO:
            if(poziom->graczX<=0)
                return false;
            p1=&poziom->plansza[poziom->graczY][poziom->graczX-1];
            if(poziom->graczX>1)
                p2=&poziom->plansza[poziom->graczY][poziom->graczX-2];
            else
                p2=NULL;
            break;
        case GORA:
            if(poziom->graczY<=0)
                return false;
            p1=&poziom->plansza[poziom->graczY-1][poziom->graczX];
            if(poziom->graczY>1)
                p2=&poziom->plansza[poziom->graczY-2][poziom->graczX];
            else
                p2=NULL;
            break;
        case PRAWO:
            if(poziom->graczX>=POLA-1)
                return false;
            p1=&poziom->plansza[poziom->graczY][poziom->graczX+1];
            if(poziom->graczX<POLA-2)
                p2=&poziom->plansza[poziom->graczY][poziom->graczX+2];
            else
                p2=NULL;
            break;
        case DOL:
            if(poziom->graczY>=POLA-1)
                return false;
            p1=&poziom->plansza[poziom->graczY+1][poziom->graczX];
            if(poziom->graczY<POLA-2)
                p2=&poziom->plansza[poziom->graczY+2][poziom->graczX];
            else
                p2=NULL;
            break;
    }
    switch(*p1)
    {
        case '#':
            return false;
        case '$':
        case '*':
            if(p2==NULL)
                return false;
            if(*p2==' ')
            {
                *p2='$';
                if(*p1=='*')
                    poziom->skrzynki++;
            }
            else if(*p2=='.')
            {
                *p2='*';
                if(*p1=='$')
                    poziom->skrzynki--;
            }
            else
                return false;
            usun(p1);
            if(*p1=='.')
                *p1='+';
            else
                *p1='@';
            usun(p);
            skrzynka=true;
            break;
        case ' ':
            *p1='@';
            usun(p);
            break;
        case '.':
            *p1='+';
            usun(p);
            break;
    }
    switch(kierunek)
    {
        case LEWO:
            poziom->graczX--;
            break;
        case GORA:
            poziom->graczY--;
            break;
        case PRAWO:
            poziom->graczX++;
            break;
        case DOL:
            poziom->graczY++;
            break;
    }
//    for(int y=0; y<20; y++)
//    {
//        for(int x=0; x<20; x++)
//            g_print("%c", poziom->plansza[y][x]);
//        g_print("\n");
//    }
//    g_print("\n");
    for(int i=COFNIECIA-1; i>=1; i--)
    {
        ostatnie[i]=ostatnie[i-1];
    }
    ostatnie[0]=kierunek;
    if(skrzynka)
        ostatnie[0]|=RUCH;
    return true;
}

bool cofnij_ruch(Poziom* poziom)
{
    if(ostatnie[0]==-1)
        return false;
    bool skrzynka=ostatnie[0]&RUCH;
    if(skrzynka)
        ostatnie[0]-=RUCH;
    char* p=&poziom->plansza[poziom->graczY][poziom->graczX];
    char* p1;
    char* p2;
    switch(ostatnie[0])
    {
        case LEWO:
            p1=&poziom->plansza[poziom->graczY][poziom->graczX-1];
            p2=&poziom->plansza[poziom->graczY][poziom->graczX+1];
            poziom->graczX++;
            break;
        case GORA:
            p1=&poziom->plansza[poziom->graczY-1][poziom->graczX];
            p2=&poziom->plansza[poziom->graczY+1][poziom->graczX];
            poziom->graczY++;
            break;
        case PRAWO:
            p1=&poziom->plansza[poziom->graczY][poziom->graczX+1];
            p2=&poziom->plansza[poziom->graczY][poziom->graczX-1];
            poziom->graczX--;
            break;
        case DOL:
            p1=&poziom->plansza[poziom->graczY+1][poziom->graczX];
            p2=&poziom->plansza[poziom->graczY-1][poziom->graczX];
            poziom->graczY--;
            break;
    }
    usun(p);
    if(skrzynka && (*p1=='$' || *p1=='*'))
    {
        usun(p1);
        if(*p==' ')
        {
            *p='$';
            if(*p1=='.')
                poziom->skrzynki++;
        }
        else
        {
            *p='*';
            if(*p1==' ')
                poziom->skrzynki--;
        }
    }
    if(*p2==' ')
        *p2='@';
    else
        *p2='+';
    for(int i=0; i<COFNIECIA-1; i++)
    {
        ostatnie[i]=ostatnie[i+1];
    }
    ostatnie[COFNIECIA-1]=-1;
    return true;
}

void wyczysc_hist()
{
    for(int i=0; i<COFNIECIA; i++)
        ostatnie[i]=-1;
}
