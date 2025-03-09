/**
 * "RUBIK"
 *
 * Drugi projekt z Wstępu Do Programowania.
 *
 * Program obraca kostką rubika.
 *
 * Wczytuje od uzytkownika instrukcje obracania kostki.
 * Porządkuje instrukcje, tak, by utworzyły poszczególne komendy.
 * Wykonuje komendy obrcania kostki. 
 * Gdy spotka komendę wypisania kostki: wypisuje ją.
 *
 * autorka: Kamila Godlewska <kp.godlewska@student.uw.edu.pl>
 * numer indeksu: 453 723
 * wersja: 1.0
 * data: 05.12.2024
 */
#include<stdio.h>
#include<stdlib.h>

#define DLG_KOM 3           //długość komendy (rozumianej jako trójka wartości: 1) ilości wierszy(W), 2)numru ściany(S), 3)kątu obrotu(K))
#define MAX_INSTRUKCJI 600  //maksymalna długość instrukcji, jaką może podać użytkownik
#define K6 6                //ilość scian w kostce
#ifndef N                   //długość ściany kostkis
#define N 5
#endif

typedef struct{             //tablica pamiętająca pojedynczą komendę
    int C[DLG_KOM];         //C[1] = W; C[2] = S; C[3] = K;
} komenda;

typedef struct{
    int A[N][N];
} sciana;

typedef struct{
    sciana B[K6];
} kostka;

/*usuwa komendy wpisane pomiędzy ostatnim '\n' a kropką;
w ten sposób zapobiega wykonywaniu komend, których użytkownik i tak nie zobaczy*/
long int zoptymalizuj_lb_komend(int A[], long int n){
    long int i = n - 1;

    while(i > 0 && A[i] != 10) i--;
    
    return (i + 1); //+1 dlatego, że ostatni '\n' również chcemy wykonać
}

/*wczytuje tekst użytkownika, dopóki nie zobaczy kropki.
Zwraca (zoptymalizowaną) ilość znaków podanych przez użytkownika*/
long int wczytaj_instrukcje (int A[]){
    int c;
    long int n = 0;

    while(n<MAX_INSTRUKCJI && (c=getchar()) != '.'){
        A[n] = c;
        n++;
    }

    return (zoptymalizuj_lb_komend(A, n));
}

/*sprawdza i zwraca, liczbę komend (rozumianych jako trójka: W S K) wpisanych w instrukcji.
Ilość komend rozpoznajemy po ilości liter zawartych w instrukcji*/
long int policz_komendy(int A[], long int n){
    long int w = 0;

    for(long int i = 0; i < n; i++)
        if(A[i] == 10 || (A[i] > 97 && A[i] < 122)) w++; //jeśli A[i] jest literą, zwiększ ilość komend 

     return w;
}

/*ustawia tablice komend na wartości domyślne: jedynki.
Jeśli użytkownik nie zadeklarował inaczej komenda jest poleceniem obrotu 1 wiersza o 1*90 stopni*/
void uzupelnij_jedynkami (komenda *kom, long int n){
    for(long int j = 0; j < n; j++) 
        for(int i = 0; i < DLG_KOM; i++) (kom[j]).C[i] = 1;
}

/*zamienia instrukcje użytkownika na poszczególne komendy*/
void sortuj_instrukcje(int A[], long int n, komenda *kom, long int lb_w){
    long int i = 0; //indeks tablicy znaków użytkownika
    long int w = 0; //indeks wskazujący na aktualnie przepisywaną komendę

    uzupelnij_jedynkami(kom, lb_w);

    while(i < n){
        if (A[i] == 10){        //A[i] to enter
            (kom[w]).C[0] = 0;
            w++;
        }
        else if(A[i] >= 97 && A[i] < 122){  //A[i] to litera
            (kom[w]).C[1] = A[i];
            w++;
        }
        else if(A[i] == 39) 
            (kom[w-1]).C[2] = 3; //A[i] to kąt 270
        else if(A[i] == 34) 
            (kom[w-1]).C[2] = 2;//A[i] oznacza kąt 180
        else {                                                 //ostatni przypadek jaki został czyli c= cyfra
            if(i > 0 && A[i-1] >= 48 && A[i-1] <= 57){         //sprawdzenie, czy W[w] miało być cyfrą czy liczbą
                (kom[w]).C[0] = ((kom[w]).C[0]) * 10 + A[i] - 48;  //ewentualne zwiększenie już wczytanej ilości wierszy (dotyczącej tej komendy)
            } else (kom[w]).C[0] = A[i] - 48;
        }
        i++;
    }
}

/*uzupełnia scianę w kostce jej "kolorem" czyli liczbą od 0 do K6-1*/
void pomaluj_sciane(sciana *d, int kol){
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++) d->A[i][j] = kol;
}

/*"maluje" każdą z ścian na odpowiedni "kolor"*/
void stworz_kostke(kostka *k){
    for(int i = 0; i < K6; i++)
        pomaluj_sciane(&(k->B[i]), i);
}

/*wypisuje spacje podczas wypisywania kostki*/
void wypisz_spacje(){
    for(int j = 0; j < N+1; j++) printf(" ");  
}

/*wypisuje wiersz numer i konkretnej ściany*/
void wypisz_wiersz_sciany(sciana *s, int i){
    for(int j = 0; j < N; j++) printf("%d", s->A[i][j]);
}

/*wypisuje aktualny wygląd kostki*/
void rysuj_kostke(kostka *k){
    printf("\n");

    for(int i = 0; i < N; i++){                 //góra rysunku kostki
        wypisz_spacje();                        //N+1 spacji
        wypisz_wiersz_sciany(&(k->B[0]), i);    //wiersz "i" ściany nr 0
        printf("\n");
    }
    for(int i = 0; i < N; i++){                     //środkowa część rysunku
        for(int j = 1; j < K6-2; j++){              //wypisanie wiersza "i" ścian nr 1, 2, 3 razem z "|" pomiędzy nimi
            wypisz_wiersz_sciany(&(k->B[j]), i);
            printf("|");
        }
        wypisz_wiersz_sciany(&(k->B[K6-2]), i);    //wypisanie wiersza "i" ściany 4
        printf("\n");
    }
    for(int i=0; i<N; i++){                     //dolna część rysunku
        wypisz_spacje();                        //N+1 spacji
        wypisz_wiersz_sciany(&(k->B[K6-1]), i); //wiersz "i" ściany 5
        printf("\n");
    }
}


/*poniżej znajdują się funckje przekazujące wiersze lub kolumny między dwoma ścianami*/

/*przepisuje w wierszy z tablicy b do a
zaczyna przepisywanie od sw (startowego wiersza) 
dzięki temu funkcja może przekazywać zarówno dolne jak i górne wiersze*/
void przekaz_w_w(sciana *a, sciana *b, int w, int sw){
    for(int i = 0; i < N; i++){
        for (int j = 0; j < w; j++)
            a->A[abs(sw - j)][i] = b->A[abs(sw - j)][i];
    }
}

/*przepisuje k kolumn z tablicy b do a
zaczyna przepisywanie od A[swa][ska] do B[swb][skb]
dzięki temu funkcja może przekazać kolumny z lewej, z prawej, w pierwotnym ułożeniu, bądź obrócone o 180 stopni
dzięki temu funkcja obsługuje wszystkie kombinacje przekazania wartości z kolumny do kolumny*/
void przekaz_k_k(sciana *a, sciana *b, int k, int swa, int ska, int swb, int skb){ 
    for(int i = 0; i < k; i++){
        for (int j = 0; j < N; j++)
            a->A[abs(swa - j)][abs(ska - i)] = b->A[abs(swb - j)][abs(skb - i)];
    }
}

/*obsługuje przekazanie wartości ścian między wierszami a kolumnami:
jeśli skad = 'w' jak 'wiersz' przekazuje w wierszy tablicy b do w kolumn tablicy a
jeśli skad jest inne przekazuje w kolumn tablicy b do wierzy tablicy a.
Podobnie jak poprzednie funkcje, uwzględnia wszystkie możliwe kombinacje tego, 
na której pozycji znajduje się/ powinien się znaleźć pierwszy element przekazywanego segmentu*/
void przekaz_k_w(char skad, sciana *a, sciana *b, int w, int swa, int ska, int swb, int skb){ 
    for(int i = 0; i < w; i++){
        for (int j = 0; j < N; j++){
            if(skad == 'w')
                a->A[abs(swa - i)][abs(ska - j)] = b->A[abs(swb - j)][abs(skb - i)];
            else
                a->A[abs(swa - j)][abs(ska - i)] = b->A[abs(swb - i)][abs(skb - j)];
        }  
    }
}

/*obkręca ścianę:
 w lewo, jeśli 'kat' (kąt) == 1; w prawo jeśli na odwrót.
 W tej funkcji jako jedynej kierunek skrętu nie jest realizowany przez rekurencję.
 Wynika to z stosunku zysku optymalizacji czasu działania kodu do skomplikowania kodu.
 W tym miejscu dzięki dodaniu dwóch dodatkowych linijek kodu, robimy 2-6 obrotów ściany mniej przy wykonywaniu każdej z komend przekręcających N wierszy*/
void obroc_sciane(sciana *s, int kat){
    sciana p;
    przekaz_w_w(&p, s, N, 0);
    if(kat == 1)
        przekaz_k_w('w', s, &p, N, 0, 0, N - 1, 0);
    else
        przekaz_k_w('w', s, &p, N, 0, 0, 0, N - 1);
}

/*Każda funkcja onazwie "rotacja_x" (gdzie x należy do {u, d, f, b, l, r}) obracają ścianę x o 90 stopni
 Jeśli urzytkownik chciał obrócić o wielokrotność 90*, funkcja wywołuje ponownie sama siebie.
 Rotocaja jest realizowana przez zapamiętanie 'w' wierszy/ kolumn na ścianie pomocniczej.
 Następnie przenoszone są w wiersze/ linijki na swoje docelowe pozycje (dla każdej ściany po kolei).
 W końcy przenoszona jest również linijka zapamiętana na ścianie pomocniczej na swoje docelowe miejsce.*/
void rotacja_b(kostka *k, int w, int kat){
    sciana p;
    przekaz_w_w(&p, &k->B[0], w, 0);
    przekaz_k_w('w', &k->B[0], &k->B[3], w, 0, 0, 0, N - 1);
    przekaz_k_w('k', &k->B[3], &k->B[5], w, 0, N - 1, N - 1, N - 1);
    przekaz_k_w('w', &k->B[5], &k->B[1], w, N - 1, N - 1, N - 1, 0);
    przekaz_k_w('k', &k->B[1], &p, w, N-1, 0, 0, 0);

    obroc_sciane(&k->B[4], 1);

    if(w == N)
        obroc_sciane(&k->B[2], 0);

    if (kat > 1)
        rotacja_b(k, w, kat - 1);
}

void rotacja_u(kostka *k, int w, int kat){ 
    sciana p;
    przekaz_w_w(&p, &k->B[1], w, 0);
    przekaz_w_w(&k->B[1], &k->B[2], w, 0);
    przekaz_w_w(&k->B[2], &k->B[3], w, 0);
    przekaz_w_w(&k->B[3], &k->B[4], w, 0);
    przekaz_w_w(&k->B[4], &p, w, 0);

    obroc_sciane(&k->B[0], 1);

    if(w == N)
        obroc_sciane(&k->B[5], 0);

    if (kat > 1)
        rotacja_u(k, w, kat - 1);
}

void rotacja_d(kostka *k, int w, int kat){ 
    sciana p;
    przekaz_w_w(&p, &k->B[1], w, N - 1);
    przekaz_w_w(&k->B[1], &k->B[4], w, N - 1);
    przekaz_w_w(&k->B[4], &k->B[3], w, N - 1);
    przekaz_w_w(&k->B[3], &k->B[2], w, N - 1);
    przekaz_w_w(&k->B[2], &p, w, N - 1);

    obroc_sciane(&k->B[5], 1);

    if(w == N)
        obroc_sciane(&k->B[0], 0);

    if (kat > 1)
        rotacja_d(k, w, kat - 1);
}

void rotacja_l(kostka *k, int w, int kat){ 
    sciana p;
    przekaz_k_k(&p, &k->B[5], w, 0, 0, 0, 0);
    przekaz_k_k(&k->B[5], &k->B[2], w, 0, 0, 0, 0);
    przekaz_k_k(&k->B[2], &k->B[0], w, 0, 0, 0, 0);
    przekaz_k_k(&k->B[0], &k->B[4], w, 0, 0, N - 1, N - 1);
    przekaz_k_k(&k->B[4], &p, w, N - 1, N - 1, 0, 0);

    obroc_sciane(&k->B[1], 1);

    if(w == N)
        obroc_sciane(&k->B[3], 0);

    if (kat > 1)
        rotacja_l(k, w, kat - 1);
}

void rotacja_r(kostka *k, int w, int kat){
    sciana p;
    przekaz_k_k(&p, &k->B[0], w, 0, N - 1, 0, N - 1);
    przekaz_k_k(&k->B[0], &k->B[2], w, 0, N - 1, 0, N - 1);
    przekaz_k_k(&k->B[2], &k->B[5], w, 0, N - 1, 0, N - 1);
    przekaz_k_k(&k->B[5], &k->B[4], w, 0, N - 1, N - 1, 0);
    przekaz_k_k(&k->B[4], &p, w, N - 1, 0, 0, N - 1);

    obroc_sciane(&k->B[3], 1);

    if(w == N)
        obroc_sciane(&k->B[1], 0);

    if (kat > 1)
        rotacja_r(k, w, kat - 1);
}

void rotacja_f(kostka *k, int w, int kat){ 
    sciana p;
    przekaz_w_w(&p, &k->B[0], w, N - 1);
    przekaz_k_w('w', &k->B[0], &k->B[1], w, N - 1, 0, N - 1, N - 1);
    przekaz_k_w('k', &k->B[1], &k->B[5], w, N - 1, N - 1, 0, N - 1);
    przekaz_k_w('w', &k->B[5], &k->B[3], w, 0, N - 1, 0, 0);
    przekaz_k_w('k', &k->B[3], &p, w, 0, 0, N - 1, 0);

    obroc_sciane(&k->B[2], 1);

    if(w == N)
        obroc_sciane(&k->B[4], 0);

    if (kat > 1)
        rotacja_f(k, w, kat - 1);
}

/*Na podstawie uwcześnie utworzonego ciągu komend fnkcja wskazuje:
- która ściana powinna się obrócić, 
- jak dużo wierszy będzie uczestniczyć w obrocie, 
- ile razy obrócić o 90 stopni ścianę
- czy należy wypisać kostkę*/
void wykonaj_instrukcje(komenda *kom, long int n){
    kostka k;

    stworz_kostke(&k);
    for(long int i = 0; i < n; i++){
        if(((kom[i]).C[0]) == 0) rysuj_kostke(&k);
        else{
            switch((kom[i]).C[1]){ //poniższe liczby odpowiadają literom x z nazw funkcji w bibliotece ASCII
                case 117:
                    rotacja_u(&k, (kom[i]).C[0], (kom[i]).C[2]);
                    break;
                case 100:
                    rotacja_d(&k, (kom[i]).C[0], (kom[i]).C[2]);
                    break;
                case 108:
                    rotacja_l(&k, (kom[i]).C[0], (kom[i]).C[2]);
                    break;
                case 114:
                    rotacja_r(&k, (kom[i]).C[0], (kom[i]).C[2]);
                    break;
                case 102:
                    rotacja_f(&k, (kom[i]).C[0], (kom[i]).C[2]);
                    break;
                case 98:
                    rotacja_b(&k, (kom[i]).C[0], (kom[i]).C[2]);
            }
        }
    }
}

/*Funkcja main wydaje polecenie wczytania instrukcji użytkownika i przerobienia jej na komendy.
Następnie wydaje rozkaz wykonania instrukcji*/
int main(){
    int A[MAX_INSTRUKCJI];
    long int n = wczytaj_instrukcje(A);
    long int lb_w = policz_komendy(A, n);

    komenda *K = (komenda*)malloc((long unsigned int)lb_w*sizeof(komenda));
   
    sortuj_instrukcje(A, n, K, lb_w);
    wykonaj_instrukcje(K, lb_w);

    free(K);

    return 0;
}