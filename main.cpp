#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <locale>

using namespace std;

#define SZEROKOSC_PLANSZY 10                 // przypisujemy stale wartosci, ktore kompilator wklei w miejsca tam gdzie napotka ten wyraz ( nie zajmuje miejsca w pamieci )
#define WYSOKOSC_PLANSZY 5                  // kod dzieki temu jest latwy do modyfikacji ( wszystko jest w jednym miejscu)

#define SYMBOL_GRANICY 'X'
#define SYMBOL_PUSTEGO_MIEJSCA ' '
#define SYMBOL_WILKA 'W'
#define SYMBOL_OWCY 'O'

#define ILOSC_OWIEC 10


// zastepuje smieci ktore byly w planszy
void generuj_granice_planszy(
        char plansza[SZEROKOSC_PLANSZY][WYSOKOSC_PLANSZY]) {           // odwiedza kazda komorke i gdy spelnia warunki wstawia symbol granicy
    for (int x = 0; x < SZEROKOSC_PLANSZY; x++) {
        for (int y = 0; y < WYSOKOSC_PLANSZY; y++) {
            if (x == 0 || x == SZEROKOSC_PLANSZY - 1 || y == 0 || y == WYSOKOSC_PLANSZY - 1) {
                plansza[x][y] = SYMBOL_GRANICY;
            }
            else {                                             // jesli ta komorka nie jest komorka na brzegu, to zawiera spacje
                plansza[x][y] = SYMBOL_PUSTEGO_MIEJSCA;
            }

        }


    }
}


void rysuj_plansze(char plansza[SZEROKOSC_PLANSZY][WYSOKOSC_PLANSZY], int najedzenie) {
    //
    for (int y = 0; y < WYSOKOSC_PLANSZY; y++) {
        for (int x = 0; x < SZEROKOSC_PLANSZY; x++) {
            cout << plansza[x][y];
        }
        cout << endl;
    }
    cout<<"Najedzenie wilka: " << najedzenie << endl;

}// coutownaie ilosci najedzenia , 2 parametr to wilk

class Owca {

public :


    int x;
    int y;
    bool czy_zyje;

    Owca() {

        x = (rand() % (SZEROKOSC_PLANSZY - 2)) + 1;
        y = (rand() % (WYSOKOSC_PLANSZY - 2)) + 1;
        czy_zyje=true;

    }


    void poruszanie_owcy(int &najedzenie,int &zabite_owce ,int x_wilka, int y_wilka) { //p[rzekazujemy oryginal zeby owca mogla zmodyfikowac wilka najedzenie itd

        int stary_x = x;
        int stary_y = y;

        int kierunek = rand() % 4;

#define LEWO 0
#define PRAWO 1
#define GORA 2
#define DOL 3

        switch (kierunek) {

            case GORA:
                y--;
                break;
            case DOL:
                y++;
                break;
            case LEWO:
                x--;
                break;
            case PRAWO:
                x++;
                break;

            default:
                break;
        }

        if(x==0 || x == SZEROKOSC_PLANSZY - 1 || y == 0 || y == WYSOKOSC_PLANSZY - 1){
            x = stary_x;
            y = stary_y;
        }

        if(x_wilka == x && y_wilka == y){
            czy_zyje = false;
            zabite_owce++;
            najedzenie += 100;
        }
    }

};

class Wilk {

public:    //dzieki temu moze byc bez problemu wywolywany w mainie

    int najedzenie;
    int x;
    int y;


    Wilk()                                //konstruktor
    {
        x = SZEROKOSC_PLANSZY / 2;
        y = WYSOKOSC_PLANSZY / 2;
        najedzenie = 100;

    }


    void poruszanie_wilka(Owca owce[ILOSC_OWIEC],int &zabite_owce, char plansza[SZEROKOSC_PLANSZY][WYSOKOSC_PLANSZY]) {

        int stary_x = x;
        int stary_y = y;

        char c = 0;

        //c = getchar();
        cin>>c;
        switch (c) {
            case 'w':
                y--;
                break;
            case 's':
                y++;
                break;
            case 'a':
                x--;
                break;
            case 'd':
                x++;
                break;

        }

        if(x==0 || x==SZEROKOSC_PLANSZY-1|| y==0 || y==WYSOKOSC_PLANSZY-1){

            x=stary_x;
            y=stary_y;
        }else if(c == 'w' || c == 's' || c == 'a' || c == 'd')
        {
            najedzenie-=10;
        }

        for(int i=0; i<ILOSC_OWIEC ; i++)
        {
            if(x==owce[i].x && y==owce[i].y && owce[i].czy_zyje)  {

                owce[i].czy_zyje=false;
                zabite_owce++;
                najedzenie+=100;
            }
        }




    }
};



int main() {

    srand(time(NULL));

    int zabite_owce=0;

    Owca owce[ILOSC_OWIEC];

    for(int i=0; i<ILOSC_OWIEC; i++)
    {
        owce[i] =  Owca();
    }

    Wilk wilk = Wilk();        // definicja objektu klasy Wilk i wywołanie konstruktora ktory przypisze nam paczatkowe zmienne


    char plansza[SZEROKOSC_PLANSZY][WYSOKOSC_PLANSZY];  // deklaracja tablicy dwu wymiarowej
    generuj_granice_planszy(plansza);                   // wywołujemy funkcje, ktora wstawia symbol granicy na obrzezach tablic
    plansza[wilk.x][wilk.y] = SYMBOL_WILKA;// wstawiam w konkretna komorke tablicy symbol wilka// wywołanie funkcjii, która wyswietla kazda komorke ktora nalezy do planszy

    for(int i=0; i<ILOSC_OWIEC; i++) {


        plansza[owce[i].x][owce[i].y] = SYMBOL_OWCY;
    }


    rysuj_plansze(plansza, wilk.najedzenie);
    cout<<"Witaj w grze \"Wilk i owce\" . Uzywaj klawiszy: W, S, A, D + enter by poruszać wilkiem"<<endl;

    while (wilk.najedzenie > 0) {


        //printf("\033[2J\033[1;1H");



        // 2. usun ze straych pozycji na planszy)
        plansza[wilk.x][wilk.y] = SYMBOL_PUSTEGO_MIEJSCA;
        for(int i=0; i<ILOSC_OWIEC; i++)
        {
            plansza[owce[i].x][owce[i].y] = SYMBOL_PUSTEGO_MIEJSCA;
        }

        // 3. porusz kazdym organizmem // 4. wstaw nowe pozycje do planszy
        wilk.poruszanie_wilka(owce,zabite_owce,plansza);
        for(int i=0; i<ILOSC_OWIEC; i++) {
            if(owce[i].czy_zyje==true) {
                owce[i].poruszanie_owcy(wilk.najedzenie,zabite_owce,wilk.x, wilk.y);
                plansza[owce[i].x][owce[i].y] = SYMBOL_OWCY;
            }
        }
        plansza[wilk.x][wilk.y] = SYMBOL_WILKA;

        // 4. wyczysc ekran
        system("clear");

        // 5. odswiez plansze na ekran
        rysuj_plansze(plansza, wilk.najedzenie);
        cout<<"Współrzędne wilka -> x: " << wilk.x << " y: " <<wilk.y<<endl;
        cout<<"Wspolrzędne owcy -> x: " << owce[0].x << " y: " <<owce[0].y<<endl;
        //cout<<"Wspolrzędne owcy -> x: " << owce[1].x << " y: " <<owce[1].y<<endl;
        //cout<<"Wspolrzędne owcy -> x: " << owce[2].x << " y: " <<owce[2].y<<endl;
        //cout<<"Wspolrzędne owcy -> x: " << owce[3].x << " y: " <<owce[3].y<<endl;
        //sleep(1);

        if(zabite_owce==ILOSC_OWIEC)
        {
            break;
        }

    }
    cout<<"koniec gry!";

    return 0;
}
