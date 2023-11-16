#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/mouse.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <windows.h>
#include <math.h>

#define wysOkna 800 ///< zdefiniowanie stałej określającej wysokość okna
#define szerOkna 900 ///< zdefiniowanie stałej określającej szerokość okna
#define dlugoscBoku 100 ///< zdefiniowanie stałej określającej długość boku pola szachownicy
#define rozmiarCzcionki 32 ///< zdefiniowanie stałej określającej rozmiar czcionki
//#define iloscPol 70

/**
* @brief Funkcja odpowiadająca za zaokrąglanie współrzędnych x i y do równych setek ponieważ potrzebna nam tylko najniższa możliwa wartość dla danego pola
**/
int zaokraglijWDolDoStu(int wartosc)
{
    return (int)(wartosc / 100) * 100; ///< zwraca wartosc zaokrągloną do setek
}

/**
* @brief Funkcja odpowiadająca za inicjalizację allegro
**/
void inicializujAllegro()
{
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_install_audio();
    al_init_acodec_addon();
}

/*void wypiszLiczbe()
{
    char numer1 = '0';
    char numer2 = '1';
    int i;
    for (i = 0; i < iloscPol; i++)
    {
        if (numer2 <= '9' && numer1 == '0')
        {
            //printf("%c%c, ",numer1, numer2);
            al_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, numer2);
            numer2++;
        }
        else if (numer2 <= '9' && numer1 != '0')
        {
            //printf("%c%c, ", numer1, numer2);
            al_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, numer1);
            al_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, numer2);
            numer2++;
        }
        else if (numer2 > '9')
        {
            //printf("%c%c, ", numer1, numer2);
            numer1 += 1;
            numer2 = '0';
            all_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, numer1);
            all_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, numer2);
        }
    }
}*/

/*
void wypiszNumer(char *text, int x, int y)
{
    ALLEGRO_FONT *font = al_load_font("czcionka.ttf", rozmiarCzcionki, NULL);
    al_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, text);
}
*/
int szachownica[8][8]={{0}}; ///< globalne zaimplementowanie tablicy dwuwymiarowej
int rozmiar = 8; ///< globalne zaimplementowanie rozmiaru tablicy

/**
* @brief Funkcja odpowiadająca za działanie algorytmu konika szachowego
* @param x - zmienna x
* @param y - zmienna y
* @param nr - zmienna, która określa numer ruchu 
**/
int ruszSkoczka(int x, int y, int nr)
{ 
    int i,j; ///< inicjalizacja zmiennych, inicjalizacja możliwości ruchu
    int rx[8] = {1,2,2,1,-1,-2,-2,-1};
    int ry[8] = {-2,-1,1,2,2,1,-1,-2};
    int tmp_x, tmp_y;
    szachownica[x][y] = nr;
    if(nr == rozmiar * rozmiar) ///< instrukcja warunkowa, sprawdza czy znajdujemy się w obrębie tablicy
    {;
        for( i=0; i<rozmiar; i++)
        {
            for( j=0; j<rozmiar; j++)
                printf("%*d", 4, szachownica[j][i]); ///< wypisuje na ekran n-ty element, który spełnia warunek
            printf("\n\n");
        }
        return 1;
    }
    else
    {
        for( i=0; i<8; i++)
        {
            tmp_x = x + rx[i];
            tmp_y = y + ry[i];
            if((tmp_x >= 0) && (tmp_x < rozmiar) && (tmp_y >= 0) && (tmp_y < rozmiar) && (szachownica[tmp_x][tmp_y] == 0))
            {
                if(ruszSkoczka(tmp_x, tmp_y, nr+1) == 1) ///< rekurencja, program sprawdza czy kolejny element mieści się w tablicy
                {
                    return 1;
                }
            }
        }
        szachownica[x][y] = 0; ///< zerowanie miejsca w tablicy
    }

    return 0;
}

void zamiana(char znak[2], int liczba) {
    if (liczba < 10) 
    {
        znak[0] = ' ';
        znak[1] = liczba + 48;
    }
    else if (liczba >= 10)
    {
        int dzies = liczba / 10;
        int jedn = liczba % 10;

        znak[0] = dzies + 48;
        znak[1] = jedn + 48;
    }
}

/**
* @brief Główna funkcja main
**/
int main()
{

    int a, b, i, j; ///< stworzenie zmiennych
    inicializujAllegro(); ///< dodanie elementów biblioteki allegro dla prawidłowego działania i wyświetlania programu
    ALLEGRO_DISPLAY* okno = al_create_display(wysOkna, szerOkna);
    al_set_window_title(okno, "Algorytm konika szachowego");
    ALLEGRO_BITMAP* bitmapa1 = al_load_bitmap("start.png");
    ALLEGRO_BITMAP* bitmapa2 = al_load_bitmap("szachownica.png");
    al_draw_bitmap(bitmapa1, 0, 0, 0); ///<pasek u góry, zaczyna się od 0,0 pixeli a konczy na szerOkna, 100
    al_draw_bitmap(bitmapa2, 0, 100, 0); ///<szachownica, zaczyna się od 0, 100 pixeli a kończy na szerOkna, wysOkna
    ALLEGRO_EVENT zdarzenie;
    ALLEGRO_EVENT_QUEUE* kolejka = al_create_event_queue(); // event_queue
    al_register_event_source(kolejka, al_get_display_event_source(okno));
    al_register_event_source(kolejka, al_get_mouse_event_source());
    ALLEGRO_FONT* font = al_load_font("czcionka.ttf", rozmiarCzcionki, NULL);
    al_reserve_samples(1); ///<dodanie niezbędnych elementów potrzebnych do włączenia muzyki
    ALLEGRO_SAMPLE* soundEffect1 = al_load_sample("menu.wav");
    al_play_sample(soundEffect1, 0.01, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);

    //al_init_font_addon();
    //ALLEGRO_FONT *font = al_load_font("czcionka.ttf", rozmiarCzcionki, 0);
    //al_draw_text(font, al_map_rgb(124, 252, 0), 0, 100, ALLEGRO_ALIGN_CENTRE, "1");


    //wypiszNumer("1", 0, 100);
    //skoczek
    //int wsp_a, wsp_b;
    //wypiszLiczbe();
    //ruszSkoczka(0, 0, 1);
    int klik = 0; ///< stworzenie dwóch wartowników
    int wartownik = 0;
    //ruszSkoczka(0, 0, 1);///< wywołanie funkcji ruszSkoczka
    //zerowanie();
    char znak[2] = {NULL, NULL};

    while (true)
    {
        al_get_next_event(kolejka, &zdarzenie);
        if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE) ///< dodanie warunku
        {
            return 0;
        }

        if (zdarzenie.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && klik == 0) ///< warunek sprawdza czy został naciśnięty przycisk myszy, jest również wartownik, który sprawdza żeby przycisk nie był naciśnięty więcej niż jeden raz
        {
            int lGorny_x = zaokraglijWDolDoStu(zdarzenie.mouse.x); ///< przypisanie wartości do zmiennej
            int lGorny_y = zaokraglijWDolDoStu(zdarzenie.mouse.y); ///< przypisanie wartości do zmiennej
            al_draw_filled_rectangle(lGorny_x, lGorny_y, lGorny_x + 100, lGorny_y + 100, al_map_rgb(0, 255, 0)); ///< narysowanie zielonego kwadratu, oznacza ono pole, z którego startuje konik
            if (wartownik == 0) ///< instrukcja warunkowa i pętle sprawdzają a następnie przypisują do zmiennych a i b zaokrągloną do jedności wartość elementu szachownicy
            {
                            a = lGorny_x / 100;
                            b = lGorny_y / 100 - 1;
                            wartownik = 1;
                            //printf("%d, %d\n", a+1, b+1);
                            ruszSkoczka(a, b, 1);
            }

            klik = 1;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++) {
                    zamiana(znak, szachownica[j][i]);
                    al_draw_textf(font, al_map_rgb(255, 0, 127), j * 100 + 30, i * 100 + 130, 0, "%c%c", znak[0], znak[1]);
                }
            }
            
        }
        
        al_flip_display();
    }
    al_destroy_display(okno);
    al_destroy_sample(soundEffect1);
    //al_destroy_font(font);
    //ruszSkoczka(a, b, 1);
    

    return 0;
}
