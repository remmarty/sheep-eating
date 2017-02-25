#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

#define SZEROKOSC_PLANSZY 30                 // przypisujemy stale wartosci, ktore kompilator wklei w miejsca tam gdzie napotka ten wyraz ( nie zajmuje miejsca w pamieci )
#define WYSOKOSC_PLANSZY  30               // kod dzieki temu jest latwy do modyfikacji ( wszystko jest w jednym miejscu)

#define SYMBOL_GRANICY 'X'
#define SYMBOL_PUSTEGO_MIEJSCA ' '
#define SYMBOL_WILKA 'W'
#define SYMBOL_OWCY 'O'

#define ILOSC_OWIEC 20


// zastepuje smieci ktore byly w planszy
void generuj_granice_planszy(char plansza[SZEROKOSC_PLANSZY][WYSOKOSC_PLANSZY]) {           // odwiedza kazda komorke i gdy spelnia warunki wstawia symbol granicy
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

}// coutownaie ilosci najedzenia

class Owca {                               // stworzenie klasy owcy

public :                                   // dostepne na zewnatrz kodu klasy , dzieki temu moze byc bez problemu wywolywany np .w mainie


    int x;
    int y;
    bool czy_zyje;

    Owca() {                     // konstruktor

        x = (rand() % (SZEROKOSC_PLANSZY - 2)) + 1;                // dane wejsciowe m.in. losowanie paczatkowych
        y = (rand() % (WYSOKOSC_PLANSZY - 2)) + 1;                 // wspolrzednych owiec
        czy_zyje=true;

    }


    void poruszanie_owcy(int &najedzenie,int &zabite_owce ,int x_wilka, int y_wilka) { // przekazujemy oryginal zeby owca mogla
                                                                                       // zmodyfikowac wspolrzedne, najedzenie wilka
        int stary_x = x;
        int stary_y = y;

        int kierunek = rand() % 4;                             // losowanie kierunku ruchu owcy

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

        if(x==0 || x == SZEROKOSC_PLANSZY - 1 || y == 0 || y == WYSOKOSC_PLANSZY - 1){  // gdy owca bedzie  chciala wejsc na granice
            x = stary_x;                                                                // to zostana im stare wspolrzedne
            y = stary_y;                                                                // bedzie losowac ruch jeszcze raz
        }

        if(x_wilka == x && y_wilka == y){                                               // kolizja->smierc owcy , zliczanie zabicia, wzrost najedzenia
            czy_zyje = false;
            zabite_owce++;
            najedzenie += 100;
        }
    }

};

class Wilk {

public:

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

        if(x==0 || x==SZEROKOSC_PLANSZY-1|| y==0 || y==WYSOKOSC_PLANSZY-1){   // wilk nie wyjdzie za granice planszy

            x=stary_x;
            y=stary_y;
        }else if(c == 'w' || c == 's' || c == 'a' || c == 'd')
        {
            if(najedzenie < 10)
                najedzenie = 0;
            else
                najedzenie-=10;                  // zmniejszanie najedzenia wraz z ruchem
        }else{
            najedzenie--;                    // inny wcisniety klawisz -> wilk przeczeka tracac tylko 1 pkt. energii
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

    srand(time(NULL));            // za kazdym razem na paczatku owce pojawia sie w nowym miejscu i beda sie inaczej ruszac

    int zabite_owce=0;

    Owca owce[ILOSC_OWIEC];

    for(int i=0; i<ILOSC_OWIEC; i++)                               // dla kazdego elementu stworzy sie nowy konstruktor
    {
        owce[i] =  Owca();
    }

    Wilk wilk = Wilk();        // definicja objektu klasy Wilk i wywołanie konstruktora ktory przypisze nam paczatkowe zmienne


    char plansza[SZEROKOSC_PLANSZY][WYSOKOSC_PLANSZY];  // deklaracja tablicy dwu wymiarowej
    generuj_granice_planszy(plansza);                   // wywołujemy funkcje, ktora wstawia symbol granicy na obrzezach tablic
    plansza[wilk.x][wilk.y] = SYMBOL_WILKA;// wstawiam w konkretna komorke tablicy symbol wilka // wywołanie funkcjii, która wyswietla kazda komorke ktora nalezy do planszy

    for(int i=0; i<ILOSC_OWIEC; i++) {

        plansza[owce[i].x][owce[i].y] = SYMBOL_OWCY;
    }


    rysuj_plansze(plansza, wilk.najedzenie);
    cout<<"Witaj w grze \"Wilk i owce\" . Uzywaj klawiszy: W, S, A, D + enter by poruszać wilkiem"<<endl;
    cout<<"Wciśnij dowolny inny klawisz, by przyczaić się na owce, tracąc tylko 1 pkt. energii"<<endl;


    while (wilk.najedzenie > 0 && zabite_owce<ILOSC_OWIEC) {              // petla gry

        // 1. usun ze starych pozycji na planszy)
        plansza[wilk.x][wilk.y] = SYMBOL_PUSTEGO_MIEJSCA;
        for(int i=0; i<ILOSC_OWIEC; i++)
        {
            plansza[owce[i].x][owce[i].y] = SYMBOL_PUSTEGO_MIEJSCA;
        }

        // 2. porusz kazdym organizmem // 3. wstaw nowe pozycje do planszy
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



    }

    if(zabite_owce==ILOSC_OWIEC)  // jesli gracz zabil wszytkie owce
    {
        cout<<"Gratulacje, udało Ci się zabić wszytkie owce!"<<endl;
    }else{

        cout<<"Wilk umarł z głodu..."<<endl;
    }


    return 0;
}
