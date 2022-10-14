#include "Level.h"
#include <stdio.h>

FILE* plik;

char plansze[MAX_POZ][POLA][POLA];

void nastepny_wiersz(FILE* plik)
{
    char c=fgetc(plik);
    while(c!=EOF && c!='\n')
    {
        c=fgetc(plik);
    }
}

bool wczytaj_poziom(Poziom* poziom)
{
    char c=fgetc(plik);
    bool wczytywanie=true, sprawdzanie=false;
    int x=0, y=0, roznica=0;
    poziom->skrzynki=poziom->ruchy=0;
    while(wczytywanie && c!=EOF)
    {
        switch(c)
        {
        case ';':
        case '\'':
            wczytywanie=false;
            break;
        case '\n':
            if(sprawdzanie)
                wczytywanie=false;
            else
            {
                sprawdzanie=true;
                x=0;
                y++;
            }
            break;
        case '@':
        case ' ':
        case '$':
        case '#':
        case '*':
            sprawdzanie=false;
            if(c=='@')
            {
                poziom->graczX=x;
                poziom->graczY=y;
            }
            else if(c=='$')
            {
                poziom->skrzynki++;
                roznica--;
            }
            poziom->plansza[y][x++]=c;
            break;
        case '.':
        case '+':
            sprawdzanie=false;
            if(c=='+')
            {
                poziom->graczX=x;
                poziom->graczY=y;
            }
            roznica++;
            poziom->plansza[y][x++]=c;
            break;
        default:
            return false;
        }
        c=fgetc(plik);
    }
    if(c==EOF)
    {
        fprintf(plik, "\n");
        fseek(plik, -1, SEEK_CUR);
    }
    else
        fseek(plik, -2, SEEK_CUR);
    return roznica==0;
}

bool wczytaj_pakiet(char* sciezka, Pakiet* pakiet)
{
    plik=fopen(sciezka, "r+");
    if(plik==NULL)
        return false;
    char c=fgetc(plik);
    if(c>='0' && c<='9')
    {
        zamknij_pakiet();
        return false;
    }
    int poziom=0;
    wyczysc(&pakiet->poziomy[0]);
    bool wczytywanie=false, rekordy=false;
    while(c!=EOF && poziom<100)
    {
        switch(c)
        {
        case '\n':
            if(wczytywanie)
            {
                poziom++;
                if(poziom<100)
                    wyczysc(&pakiet->poziomy[poziom]);
                wczytywanie=false;
            }
            break;
        case '\'':
            if(wczytywanie)
            {
                wczytywanie=false;
                poziom++;
                if(poziom<100)
                    wyczysc(&pakiet->poziomy[poziom]);
            }
            int poz_nazwa=0;
            c=fgetc(plik);
            while(poz_nazwa<29 && c!='\'')
            {
                pakiet->poziomy[poziom].nazwa[poz_nazwa]=c;
                poz_nazwa++;
                c=fgetc(plik);
            }
            pakiet->poziomy[poziom].nazwa[poz_nazwa]='\0';
            nastepny_wiersz(plik);
            break;
        case ';':
            rekordy=true;
//            if(wczytywanie)
//            {
//                wczytywanie=false;
//                poziom++;
//                if(poziom<100)
//                    wyczysc(&pakiet->poziomy[poziom]);
//            }
//            for(int i=0; i<poziom; i++)
//            {
//                pakiet->poziomy[i].rekord_poz=ftell(plik);
//                if(fscanf(plik, "%d\n", &pakiet->poziomy[i].rekord)<1)
//                {
//                    fseek(plik, -2, SEEK_CUR);
//                    c=fgetc(plik);
//                    if(c!='\n' && c!=';')
//                        fprintf(plik, "\n");
//                    pakiet->poziomy[i].rekord_poz=ftell(plik);
//                    fprintf(plik, "0000\n");
//                }
//                else if(pakiet->poziomy[i].rekord==0)
//                    pakiet->poziomy[i].rekord=-1;
//            }
            long int poz=ftell(plik);
            int poprawne=poziom;
            for(int i=0; i<poziom; i++)
            {
                bool blad=false;
                for(int j=0; j<4; j++)
                {
                    c=fgetc(plik);
                    if(c<'0' || c>'9')
                    {
                        j=10;
                        blad=true;
                    }
                }
                if(blad)
                {
                    poprawne=i;
                    i=poziom;
                    fseek(plik, poz, SEEK_SET);
                }
                else
                {
                    c=fgetc(plik);
                    g_print("%d.%c.\n", i, c);
                    if(c!='\n')
                    {
                        fseek(plik, -1, SEEK_CUR);
                        fputc('\n', plik);
                    }
                    fseek(plik, poz, SEEK_SET);
                    pakiet->poziomy[i].rekord_poz=poz;
                    fscanf(plik, "%d\n", &pakiet->poziomy[i].rekord);
                    if(pakiet->poziomy[i].rekord==0)
                        pakiet->poziomy[i].rekord=-1;
                    poz=ftell(plik);
                }
            }
            for(; poprawne<poziom; poprawne++)
            {
                pakiet->poziomy[poprawne].rekord_poz=ftell(plik);
                pakiet->poziomy[poprawne].rekord=-1;
                fprintf(plik, "0000\n");
            }
            break;
        case '@':
        case '+':
        case '$':
        case '*':
        case '.':
        case ' ':
        case '#':
            wczytywanie=true;
            fseek(plik, -1, SEEK_CUR);
            if(!wczytaj_poziom(&pakiet->poziomy[poziom]))
            {
                zamknij_pakiet();
                return false;
            }
            break;
        default:
            zamknij_pakiet();
            return false;
        }
        c=fgetc(plik);
    }
    if(!rekordy)
    {
        fprintf(plik, ";");
        for(int i=0; i<poziom; i++)
        {
            pakiet->poziomy[i].rekord_poz=ftell(plik);
            fprintf(plik, "0000\n");
        }
    }
    pakiet->ile=poziom;
    for(int k=0; k<poziom; k++)
        for(int i=0; i<POLA; i++)
            for(int j=0; j<POLA; j++)
                plansze[k][i][j]=pakiet->poziomy[k].plansza[i][j];
    return true;
}

bool zapisz_rekord(Poziom* poziom)
{
    fseek(plik, poziom->rekord_poz, SEEK_SET);
    if(fprintf(plik, "%.4d", poziom->rekord<10000?poziom->rekord:9999)<4)
        return false;
    return true;
}

bool zapisz_stan(Pakiet* pakiet, char* sciezka)
{
    FILE* stan=fopen(sciezka, "w");
    if(stan==NULL)
        return false;
    fprintf(stan, "%d\n", pakiet->ile);
    for(int p=0; p<pakiet->ile; p++)
    {
        for(int i=0; i<POLA; i++)
        {
            for(int j=0; j<POLA; j++)
                fputc(pakiet->poziomy[p].plansza[i][j], stan);
            fputc('\n', stan);
        }
        fprintf(stan, ";%.4d\n", pakiet->poziomy[p].ruchy);
    }
    fclose(stan);
    return true;
}

bool wczytaj_stan(Pakiet* pakiet, char* sciezka)
{
    FILE* stan=fopen(sciezka, "r");
    if(stan==NULL)
        return false;
    int p, x, y, skrzynki=0, graczX=0, graczY=0;
    x=fscanf(stan, "%d", &y);
    fgetc(stan);
    if(x<1 || y!=pakiet->ile)
        return false;
    char c=fgetc(stan);
    char pom[POLA][POLA];
    for(int i=0; i<POLA; i++)
        for(int j=0; j<POLA; j++)
            pom[i][j]=' ';
    p=x=y=0;
    while(c!=EOF)
    {
        switch(c)
        {
        case ';':
            fscanf(stan, "%d", &pakiet->poziomy[p].ruchy);
            fgetc(stan);
            if(p>pakiet->ile)
            {
                fclose(stan);
                return false;
            }
            for(int i=0; i<POLA; i++)
                for(int j=0; j<POLA; j++)
                {
                    pakiet->poziomy[p].plansza[i][j]=pom[i][j];
                    pom[i][j]=' ';
                }
            pakiet->poziomy[p].skrzynki=skrzynki;
            pakiet->poziomy[p].graczX=graczX;
            pakiet->poziomy[p].graczY=graczY;
            x=y=skrzynki=0;
            p++;
            break;
        case '\n':
            y++;
            x=0;
            break;
        case '#':
            if(pakiet->poziomy[p].plansza[y][x]!='#')
            {
                fclose(stan);
                return false;
            }
            pom[y][x++]='#';
            break;
        case '.':
        case '+':
        case '*':
            if(c=='+')
            {
                graczX=x;
                graczY=y;
            }
            if(pakiet->poziomy[p].plansza[y][x]!='*' && pakiet->poziomy[p].plansza[y][x]!='+' && pakiet->poziomy[p].plansza[y][x]!='.')
            {
                fclose(stan);
                return false;
            }
            pom[y][x++]=c;
            break;
        case '@':
        case '$':
        case ' ':
            if(c=='@')
            {
                graczX=x;
                graczY=y;
            }
            else if(c=='$')
                skrzynki++;
            if(pakiet->poziomy[p].plansza[y][x]!=' ' && pakiet->poziomy[p].plansza[y][x]!='$' && pakiet->poziomy[p].plansza[y][x]!='@')
            {
                fclose(stan);
                return false;
            }
            pom[y][x++]=c;
            break;
        default:
            fclose(stan);
            return false;
        }
        c=fgetc(stan);
    }
    fclose(stan);
    return true;
}

void resetuj_poziom(Pakiet* pakiet, int nr)
{
    Poziom* poziom=&pakiet->poziomy[nr];
    poziom->skrzynki=poziom->ruchy=0;
    for(int i=0; i<POLA; i++)
        for(int j=0; j<POLA; j++)
        {
            poziom->plansza[i][j]=plansze[nr][i][j];
            if(poziom->plansza[i][j]=='@' || poziom->plansza[i][j]=='+')
            {
                poziom->graczY=i;
                poziom->graczX=j;
            }
            else if(poziom->plansza[i][j]=='$')
                poziom->skrzynki++;
        }
}

void zamknij_pakiet()
{
    if(plik!=NULL)
    {
        fclose(plik);
        plik=NULL;
    }
}
