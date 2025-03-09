/**
 * "SOKOBAN"
 *
 * Trzeci projekt na zajęcia "Wstęp Do Programowania".
 *
 * Program symuluję grę w sokobana.
 *
 * Wczytuje od uzytkownika planszę i komendy.
 * Wykonuje komendy:
 * - wydruku aktualnego stanu planszy,
 * - przesunięcia skrzynki (po sprawdzeniu, że taki ruch jest możliwy),
 * - cofnięcia ostatniego udanego przesunięcia skrzynki.
 *
 * autorka: Kamila Godlewska <kp.godlewska@student.uw.edu.pl>
 * numer indeksu: 453 723
 * wersja: 1.0
 * data: 02.01.2025
 */
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define K_K 2 //ilość kolumn w tablicy komend
#define W_P 5//wartość początkowa zakresów planszy i komend
#define RDM 32//różńica między małymi a dużymi literami w ASCII

typedef struct{
    int k;  //lb kolumn tablicy
    int w;  //lb wierszy tablicy
}punkt;

/*uwalnia pamięć dynamicznie przypisaną tablicy typu int*/
void wyczysc(int*** tablica, int w){
    if(*tablica!=NULL && tablica!=NULL){
        for(int i=0; i<w; i++){
            free((*tablica)[i]);
            (*tablica)[i]=NULL;
        } 
    
        free((*tablica));
        (*tablica)=NULL;
        tablica=NULL;
    }
}
/*uwalnia pamięć dynamicznie przypisaną tablicy typu punkt*/
void wyczysc_punkty(punkt*** tablica, int w){
    if(*tablica!=NULL && tablica!=NULL){
        for(int i=0; i<w; i++){
            free((*tablica)[i]);
            (*tablica)[i]=NULL;
        } 
    
        free((*tablica));
        (*tablica)=NULL;
        tablica=NULL;
    }
}

/*relokuje ilość pamięci przypisanej do tablicy, bo zwiększyć lb. jej wierszy
zalokowane wiersze ustawiane są na zera*/
int dodaj_wiersze(int*** tablica, int w, punkt* t) {
    int** temp=realloc(*tablica, (size_t)(t->w)*sizeof(int*));
    assert(*tablica != NULL);   // tablica została poprawnie zaalokowana
     
    *tablica=temp;

    for(int i=w; i<t->w; i++){
        (*tablica)[i]=calloc((size_t)(t->k), sizeof(int));
        assert((*tablica)[w]!=NULL); // nowy wiersz został poprawnie zaalokowany
    }

    return 1; //Poprawnie zalokono nowe wiersze
}

/*zwiększa długość wierszy tablicy
nowopowsatłe kolumny (od k do t->k) uzupełnia zerami*/
int dodaj_kolumny(int*** tablica, int w, int k, punkt* t){
    int* temp;

    for(int i=w>0?w-1:w; i<t->w; i++){
        temp=realloc((*tablica)[i], (size_t)(t->k)*sizeof(int));
        assert(temp!=NULL);

        (*tablica)[i]=temp;
        for(int j=k; j<t->k; j++) (*tablica)[i][j]=0;
    }
    return 1; //zwiększenie długości wierszy przebiegło pomyślnie
}

/*wczytuje plansze dopóki nie ma dwóch enterów lub kropki*/
int wczytaj_plansze(int*** plansza, punkt** p){
    int c;
    int w=0;//numer aktualnego wiersza
    int k=0; //numer aktualnej kolumny
    int koniec=0;//zmienia wartość gdy spotka kropkę albo podwójny enter

    assert(dodaj_wiersze(plansza, w, *p)); //tworzy początkowe W_P wierszy planszy

    while(koniec<2 && (c=getchar())!='.'){//kropka kończy wszystko
        if(c=='\n'){            //nowy wiersz: 
            koniec++;           //jeśli to drugi enter pod rząd: koniec planszy
            if (koniec<2){      //jeśli to nie koniec planszy, przejdź do kolejnego wiersza
                w++;            
                k=0;

                if(w>=(*p)->w){ //jeśli trzeba, relokuj pamięć by dodać kolejne wiersze
                    (*p)->w=2*((*p)->w);  //zwiększ dwukrotnie ilość wierszy
                    assert(dodaj_wiersze(plansza, w, *p));
                } 
            }
        }
        else{                   //znak planszy: dodaj do aktualnej kolumny aktualnego wiersza
            koniec=0;
            assert((*plansza)[w]!=NULL);
            (*plansza)[w][k]=c;
            k++;                //przejdź do kolejnej (jeszcze pustej) kolumny
            if(k+1>=(*p)->k){   //jeśli trzeba, relokuj pamięć by dodać kolejne kolumny
                                //"k+1" dlatego, że ostatnia kolumna w wierszu ma pozostać zerem (żeby nie wyjść za zakres przy przesuzkiwaniu)
                (*p)->k=2*((*p)->k);    //zwiększ dwukrotnie ilość kolumn
                assert(dodaj_kolumny(plansza, w, k, *p));
            } 
        }
    }

    return(koniec<2); //jeśli koniec jest mniejszy od 2, to pętle przerwało pojawienie się kropki-należy przerwać cały program
}

/*wczytuje komendy użytkownika, dopóki nie zobaczy kropki*/
void wczytaj_komendy (int*** komendy, punkt** kom){
    int c;
    int w=0, k=0;

    assert(dodaj_wiersze(komendy, w, *kom));   //tworzy pierwsze W_P wierszy komend
    (*komendy)[0][0]=(int)'\n';       //pierwszy wiersz komendy zawsze zawiera instrukcję wypisania planszy
    w=1;

    while((c=getchar())!='.'){            //kropka kończy wszystko
        if(c==(int)'\n' || c==(int)'0'){   //komenda "niestandardowa"
            (*komendy)[w][0]=c;           //-> zapisz w pierwszej kolumnie wiersza 
            k=2;                          //zasygnalizuj chęć zmiany wiersza na kolejny
        }
        else{                             //komenda "standardowa" (składająca się z dwóch znaków)
            assert(k<K_K);                //zapamiętaj znak komendy
            (*komendy)[w][k]=c;           //zwiększ nr kolumny- następny znak zapisze się w następnej kolumnie/ wierszu
            k++;
        }
        if(k>=2){                          //jeśli wczytano już pełną komendę, przejdź do następnego wiersza
            k=0;
            w++;         
            if(w>=(*kom)->w){              //jeżeli wierszy jest za mało dodaj kolejne
                (*kom)->w=2*((*kom)->w);
                assert(dodaj_wiersze(komendy, w, *kom));
            }
        }
    }
    //zaznaczenie końca komend(żeby program nie przechodził przez zera, które nie zostały wpisane przez użytkownika)
    if(w<(((*kom)->w)-1))   //jeśli w ostatnim z alokowanych wierszy nie ma komendy użytkownika wpisz 1 do wiersza w
        (*komendy)[w][0]=1; //1 jest znakiem, żeby przestać zczytywać komendy
}

/*zarządza wczytaniem planszy i komend*/
int wczytaj_dane(int*** plansza, int*** komendy, punkt** p, punkt** k){
    assert(p!=NULL);
    //jeśli wczytanie planszy przebiegło prawidłowo oraz nie napotkano kropki, wczytaj komendy
    //jeśli nie, wyczysć planszę i zwróć 1
    if(wczytaj_plansze(plansza, p)){ 
        wyczysc(plansza, (*p)->w);
        return 0;
    }
    else wczytaj_komendy(komendy, k); 
    return 1;
}

/*tu się rozpoczyna część kodu związana z rozpoznawaniem i wykonywaniem komend użytkownika*/
/*wyszukuje współżędne symbolu "symbol" w aktualnej planszy, zapisuje współżędne w miejscu wskazanym przez "kordy"*/
void wyszukaj_kordy(int** plansza, punkt* p, int symbol, punkt** kordy){
    int w=0; //czy już znaleziono szukany symbol?
    int d=0; //druga wersja poszukiwanego symbolu
    int i=0, j=0;
    
    if(symbol=='@') d='*';//jeśli szukamy ludzika trzeba wziąć pod uwagę syt. gdy stoi na skrzynce
    else if(symbol>='a' && symbol<='z') d=symbol-RDM;//tak samo dla skrzynek: bierzemy pod uwagę duże i małe litery
    while(w==0 && i<p->w){ //przeszukuje planszę wiersz po wierszu, dopóki nie znajdzie symbolu
        assert(plansza[i]!=NULL);
        j=0;
        while(w==0 && j<p->k){
            if(plansza[i][j]==0) j=p->k; //0 oznacza koniec wiersza planszy- nie ma po co szukać dalej
            else if(plansza[i][j]==symbol || plansza[i][j]==d){ //jeśli znaleziono symbol zmień kordy
                (*kordy)->w=i;
                (*kordy)->k=j;
                w=1;
            }
            else j++;
        }
        i++;
    }
}

/*sprawdza, czy zadane "pole" jest wolne*/
int wolne_pole(int *pole){
    int w=1;
    //jeśli pole jest puste albo jeśli na polu stoi skrzynia lub ściana zwróć fałsz
    if(pole==NULL) w=0;
    else if((*pole)==0 || (*pole)==(int)'#'|| ((*pole)>=(int)'A' && (*pole)<=(int)'z')) w=0;
    return w;
}

/*funkcja powstała w celu wyznaczania pola, na które zostanie przesunięta skrzynia/ z którego musi zostać pchnięta*/
/*zmienia podane kordy "p" na przesunięte o 1 w stronę, którą wkazuje kierunek*/
void kordy_docelowe(punkt **p, int kierunek, int przed){
    int z=1;
    //"przed" jest przełącznikiem, który decyduje, czy wyznaczyć kordy przed skrzynką, czy za skrzynką (względem kierunku ruchu)
    if(!przed) z=-1; 

    switch(kierunek){
            case '2':
                (*p)->w-=z;
                break;
            case '8':
                (*p)->w+=z;
                break;
            case '4':
                (*p)->k+=z;
                break;
            case '6':
                (*p)->k-=z;
        }
}

/*sprawdzenie, czy istnieje trasa, którą można przejść drogę z punktu (w,k) do punktu "s"*/
/*funkcja jest rekurencją: kończy swoje działanie w momencie, 
gdy znaleziono ścieżkę prowadzącą do punktu "s", albo gdy sprawdzono wszystkie drogi*/
int znajdz_droge(int**odwiedzone, int**plansza, int w, int k, punkt*s, punkt*p){
    int z=0; //znaleziona
    if(w<p->w && k<p->k && w>=0 && k>=0){
        odwiedzone[w][k]=1; //zapamiętanie aktualnego kawałka drogi- żeby nie chodzić w kółko
        if (w==s->w && k==s->k) z=1; //jeśli aktualnie znajdujemy się w punkcie s: znaleziona, zatrzymanie rekurencji
        else{
            //jeśli na miejscu o jeden do dołu na planszy można stanąć (nie jest poza zakresem, nie ma przeszkody na nim, nie zostało już odwiedzone)
            //->idź o jeden w dół i sprawdź dalsze możliwości ruchów
            //jeśli w efekcie tego działania droga nie została znaleziona, powtórz to samo tylko idąc w innym kierunku
            if((k-1)>=0 && odwiedzone[w][k-1]==0 && wolne_pole(&plansza[w][k-1])){
                z=znajdz_droge(odwiedzone, plansza, w, --k, s, p);
                k=k+1;
            }
            if(z==0 && (w-1)>=0 && odwiedzone[w-1][k]==0 && wolne_pole(&plansza[w-1][k])){
                z=znajdz_droge(odwiedzone, plansza, --w, k, s, p);
                w=w+1;
            }
            if(z==0 && (k+1)<p->k && odwiedzone[w][k+1]==0 && wolne_pole(&plansza[w][k+1])){
                z=znajdz_droge(odwiedzone, plansza, w, ++k, s, p);
                k=k-1;
            }
            if(z==0 && (w+1)<p->w && odwiedzone[w+1][k]==0 && wolne_pole(&plansza[w+1][k])){
                z=znajdz_droge(odwiedzone, plansza, ++w, k, s, p);
                w=w-1;
            }
        }            
    }
    return z;
}

/*sprawdza, czy przesunięcie skrzyni jest możliwe do wykonania*/
int mozliwosc_ruchu(int** plansza, punkt* p, int kierunek, punkt* s, punkt* l){
    int r=0; //r=0 -> ruchu nie ma r=1 -> ruch jest możliwy
    punkt*k_s; //kopia s
    k_s=malloc(sizeof(punkt));
    //inicjalizacja tablicy "odwiedzone", na rzecz funkcji "znajdx_drogę":
    int**odwiedzone=calloc((size_t)((p->w)), sizeof(int*)); 
    for(int i=0; i<p->w; i++){
        odwiedzone[i]=calloc((size_t)((p->k)), sizeof(int));
        if(odwiedzone[i]==NULL) wyczysc(&odwiedzone, i);
    }
    
    //najpierw: sprawdzić czy jest miejsce wokół skrzyni na ludzika i przesunięcie (przyszłą pozycję srzyni):
    if(kierunek=='2' || kierunek=='8'){//musi być miejsce pod i nad skrzynią
        if(((s->w)+1<p->w && (wolne_pole(&plansza[s->w+1][s->k]))) && (s->w-1>=0 && (wolne_pole(&plansza[s->w-1][s->k]))))
            r=1;
    }
    else{      //musi być miejsce z lewej i prawej strony skrzyni
        if(((s->k)+1<p->k && (wolne_pole(&plansza[s->w][s->k+1]))) && (s->k-1>=0 && (wolne_pole(&plansza[s->w][s->k-1]))))
            r=1;
    }
    //jeśli przy skrzyni są wolne pola-> sprawdź, czy da się dojść do pola, z którego ludzik popchnie skrzynkę:
    if(r!=0){
        //1. ustal, gdzie jest miejsce, z którego ludzik będzie pchał skrzynkę
        k_s->w=s->w;
        k_s->k=s->k;
        kordy_docelowe(&k_s, kierunek, 1);

        //2. sprawdź, czy da się tam dojść
        r=znajdz_droge(odwiedzone, plansza, k_s->w, k_s->k, l, p);
    }
    wyczysc(&odwiedzone, p->w);
    free(k_s);

    return r;
}

/*wpisuje określony symbol na pole "gdzie"*/
void postaw_symbol(int*** plansza, punkt* gdzie, int symbol){
    int pole=(*plansza)[gdzie->w][gdzie->k];

    if(pole=='-' || pole=='@' || (pole>='a' && pole<='z')){  //jeśli pole jest zwykłym polem
        if(symbol=='@'|| symbol=='*') (*plansza)[gdzie->w][gdzie->k]='@';    //stawiamy zwykłego ludzika, lub:
        else if(symbol>='a' && symbol<='z') (*plansza)[gdzie->w][gdzie->k]=symbol; //mała litera zostaje mała
        else (*plansza)[gdzie->w][gdzie->k]=symbol+RDM;                            //dużą literę zamieniamy na małą
    }     
    else{                                                   //jeśli pole jest polem docelowym
        if(symbol=='@' || symbol=='*') (*plansza)[gdzie->w][gdzie->k]='*';    //stawiamy specjalnego ludzika, lub:
        else if(symbol>='a' && symbol<='z') (*plansza)[gdzie->w][gdzie->k]=symbol-RDM; //małą literę zamieniamy na dużą
        else (*plansza)[gdzie->w][gdzie->k]=symbol;                                    //duża zostaje dużą
    }
}

/*zmienia pozycje ludzika i skrzyni na planszy*/
void przesun_skrzynie(int*** plansza, int kierunek, punkt* l, punkt* s){
    punkt *k_s=malloc(sizeof(punkt)); //kopia współrzędnych skrzyni
    k_s->w=s->w;
    k_s->k=s->k;
    int skrzynia=(*plansza)[k_s->w][k_s->k]; //symbol skrzyni
    
    //1. usunięcie ludzika z dotychczasowego pola
    if((*plansza)[l->w][l->k]==(int)'@') (*plansza)[l->w][l->k]=(int)'-';   //ludzik stał na polu zwykłym
    else (*plansza)[l->w][l->k]=(int)'+';                                   //ludzik stał na polu docelowym
    //2. przesunięcie ludzika na skrzynkę
    postaw_symbol(plansza, k_s, (int)'@');
    //3.przesunięcie skrzyni
    kordy_docelowe(&k_s, kierunek, 0); //wyznaczenie miejsca, na które skrzynia ma być przesunięta
    postaw_symbol(plansza, k_s, skrzynia); //przesunięcie skrzyni na wyznaczone miejsce
    
    free(k_s);
}

/*cofa ostatnią udaną zmianę położenia ludzika i skrzyni*/
int cofnij_ruch(int*** plansza, int** komendy, int w, punkt** L, int l){
    int skrzynia;
    punkt*s_t=malloc(sizeof(punkt)); //skrzynia teraz
    int i=w-1;
    //wyszukaj ostatnie przesunięcie, które zostało wykonane
    while(i>=0 && (komendy[i][0]==(int)'#' || komendy[i][0]==(int)'0' || komendy[i][0]==(int)'\n')) i--; 
    if(i>=0){ //jeśli udało się znaleźć takie przesunięcie szukamy, gdzie teraz znajduje się skrzynia
        //skrzynia jest przesunięta o jeden w stronę kierunek od pozycji ludzika, więc:
        s_t->w=L[l]->w;
        s_t->k=L[l]->k;
        kordy_docelowe(&s_t, komendy[i][1], 0);
            
        //przesuń skrzynie, przesun ludzika:
        skrzynia=(*plansza)[s_t->w][s_t->k];//znajdź symbol aktualnej skrzyni
        //1.usunięcie skrzyni z 'teraźniejszego' pola
        if(skrzynia>='a' && skrzynia<='z') (*plansza)[s_t->w][s_t->k]=(int)'-';
        else (*plansza)[s_t->w][s_t->k]=(int)'+';
        //2. postawienie skrzyni na aktualnym miejscu ludzika
        postaw_symbol(plansza, L[l], skrzynia);
        //3. postawienie ludzika na poprzednim polu
        postaw_symbol(plansza, L[l-1], (int)'@');        
    }

    free(s_t);
    return i; //zwraca indeks komendy, którą trzeba zamienić na #
}

/*wypisuje aktualny stan planszy*/
void wypisz_plansze(int** plansza, punkt* p){
    int j, i=0, koniec=0;
    while(i<p->w && koniec==0){
        j=0;
        while(plansza[i]!=NULL && j<p->k && koniec==0){
            if(plansza[i][j]!=0){
                printf("%c", (char)plansza[i][j]);
                j++;
            }
            else if(j==0) koniec=1;
            else {
                j=p->k;
                printf("\n");
            }
        }
        i++;
    }
}

/*odczytuje kolejne komendy i wykonuje je*/
void wykonaj_komendy(int*** plansza, int*** komendy, punkt* p, punkt* k, punkt** Ludzik, punkt* kordy_skrzynki){
    int cofanie, i=0;
    int l=0;//indeks aktualnego położenia ludzika

    //kolejne odczytawanie komend i ich wykonywanie:
    while(i<k->w){
        if((*komendy)[i][0]==(int)'\n') //komenda wypisania planszy
            wypisz_plansze(*plansza, p);
        else if((*komendy)[i][0]=='0'){  //komenda cofnięcia ruchu
            if(l>0){ //jeśli jest co cofać- jeśli udało się wykonać jakiś ruch i ludzik zmienił położenie
                cofanie=cofnij_ruch(plansza, *komendy, i, Ludzik, l);
                
                Ludzik[l]->w=0; //usunięcie pamięci o cofniętym położeniu ludzika
                Ludzik[l]->k=0;
                l--;
                
                (*komendy)[cofanie][0]='#'; //usunięcie cofniętej komendy (żeby nie cofać jej ponownie)
            }
        }
        else if ((*komendy)[i][0]!=1){                      //komenda przesunięcia skrzynki
            wyszukaj_kordy(*plansza, p, (*komendy)[i][0], &kordy_skrzynki); //gdzie jest skrzynka, którą trzeba przesunąć
            
            //sprawdź, czy ruch da się wykonać, jeśli tak: zrób to
            if(mozliwosc_ruchu(*plansza, p, (*komendy)[i][1], kordy_skrzynki, Ludzik[l])){
                przesun_skrzynie(plansza, (*komendy)[i][1], Ludzik[l], kordy_skrzynki);
                l++;
                assert(l<k->w);
                Ludzik[l]->w=kordy_skrzynki->w; //zapisz nowe położenie ludzika
                Ludzik[l]->k=kordy_skrzynki->k;
            }
            else (*komendy)[i][0]=(int)'#'; //jeśli nie da się wykonaś komendy zaznacz, że komenda nie została wykonana
        }

        if((*komendy)[i][0]!=1) i++; //jeśli są jeszcze komendy do wykonania, przejdź do następnej
        else i=k->w; //w przeciwnym wypadku, zakończ pętlę
    }
}

/*zarządza wykonywaniem komend wczytanych przez użytkownika*/
/*jest funkcją pomocniczą dla wykonaj_komendy- wspomaga ją w zarządzaniu pamięcią*/
void graj_w_sokobana(int*** plansza, int*** komendy, punkt* p, punkt* k){
    punkt**Ludzik=(punkt**)calloc((size_t)k->w, sizeof(punkt*));//tu będą zapisywane kolejne położenia ludzika
    for(int j=0; j<k->w; j++){
        Ludzik[j]=(punkt*)calloc(1, sizeof(punkt));
        assert(Ludzik[j]!=NULL);
    } 
    
    punkt* kordy_skrzynki=malloc(sizeof(punkt));    //tu będą zapisywane współrzędne skrzynki, którą trzeba przesunąć
    assert(kordy_skrzynki!=NULL);

    if(k->w!=0){             //jeśli komendy nie są puste wyszukaj i zapisz początkową pozycję ludzika
        wyszukaj_kordy(*plansza, p, (int)'@', Ludzik);
                            //następnie przekaż komendy i tablice do wykonania
        wykonaj_komendy(plansza, komendy, p, k, Ludzik, kordy_skrzynki);
    }
    
    wyczysc_punkty(&Ludzik, k->w);
    free(kordy_skrzynki);
}

/*stworzenie tablicy "plansza" i "komendy"*/
/*zainicjowanie wczytania danych oraz gry w sokobana*/
int main(){
    punkt *zakres_p=malloc(sizeof(punkt));
    punkt *zakres_k=malloc(sizeof(punkt));
    assert(zakres_p!=NULL && zakres_k!=NULL);

    int**plansza=(int**)malloc(sizeof(int*));   
    int**komendy=(int**)malloc(sizeof(int*));
    assert(komendy!=NULL && plansza!=NULL);

    zakres_p->w=W_P;
    zakres_p->k=W_P;
    zakres_k->w=W_P;
    zakres_k->k=K_K;
    
    //jeśli dane zostaną poprawnie wczytane, graj w sokobana
    if(wczytaj_dane(&plansza, &komendy, &zakres_p, &zakres_k)) {
        graj_w_sokobana(&plansza, &komendy, zakres_p, zakres_k);
    }
    
    //zwolnij pamięć
    wyczysc(&plansza, zakres_p->w);
    wyczysc(&komendy, zakres_k->w);
    assert(plansza==NULL);
    assert(komendy==NULL);
    
    free(zakres_p);
    free(zakres_k);

    return 0;
}