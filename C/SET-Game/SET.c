/**
 * "SET"
 *
 * Pierwszy projekt z Wstępu Do Programowania.
 *
 * Program dokańcza rozgrywkę w SETa.
 *
 * Wczytuje od uzytkownika karty.
 * Dopóki nie skończą się ruchy: 
 *      wypisuje stół oraz pierwszą trójkę kart spełniającą SET.
 *      odrzuca karty, które utworzyły już SET.
 *      dobiera na ich miejsce nowe karty, jeśli jeszcze jakieś są w talii.
 * Jeśli nie ma ruchy dobiera dodatkowe trzy karty na stół. 
 * Jeśli nie ma już kart do dobrania w tali kończy rozgrywkę.
 *
 * autorka: Kamila Godlewska <kp.godlewska@student.uw.edu.pl>
 * numer indeksu: 453 723
 * wersja: 1.0
 * data: 21.12.2024
 */

#include <stdio.h>
#include <stdlib.h>

#define Talia 81 //maksymalna ilość kart w talii
#define Stol 12 //Standardowa ilość kart na stole

int potega(int x, int a){ //potęga stopnia a z x
    int y;

    if (a==0) y=1;
    else y=x;

    for(int i=1; i<a; i++){
        y*=x;
    }
    return y;
}

/* wczytuje karty podane przez użytkownika */
int wczytaj_set(int A[], int n){
    int g=0;

    for(int i=0; i<n; i++){
        if(scanf("%d", &A[i]) !=1) break;
        else g++;
    }

    return g;
}

/* wybiera kartę, która stworzyłaby SET z kartami a i b*/
int okresl_szukana_karte(int a, int b){
    int ra, rb, c=0;

    for (int i=1; i<5; i++){
        /*Odczytujemy, które wersje cechy zostały zakodowane na kartach a i b*/
        ra= a%(potega(10, i));
        rb= b%(potega(10, i));

        a -=ra;
        b -=rb;

        /**
         * Ja podstawie odczytanej z znanych kart cechy ustalamy cechę 3ciej karty:
         * - jeśli cechy obu kart są takie same, trzecia karta powinna mieć identyczną wersję cechy, 
         * - w przeciwnym wypadku: trzecia karta powinna mieć inną odmianę cechy
         */

        if(ra==rb) c+=rb;
        else c= c + (6*(potega(10, (i-1))) -ra -rb);
    }

    return c;
}

/**
 * Usuwa wartość komurki numer 'i' oraz przesuwa karty z reszty komórek w lewo.
 * Zwraca nowy numer ilości kart w grze.
 * */
int usun (int A[], int i, int n){
    
    for(int j=i; j<n-1; j++){
        A[j]=A[j+1];                
    }
    A[n-1]=0;  //bo po skończeniu pętli A[n-1] ma jeszcze przypisaną wartość

    return n-1;
}

/**
 * Określa, czy w grze jest jeszcze ruch. 
 * Jeśli tak: wykonuje go, czyli wypisuje znalezioną trójkę tworzącą SET i usuwa ją z stołu.
 * Zwraca ilość kart po usunięciu trójki. 
 * Jeśli nie dało się znaleźć SETa zwraca ilość kart równą początkowej.
 */
int szukaj_trojki(int A[], int n, int g){
    int znajdz=0;
    int i=0, j, k;
    int sz;
    
    while(znajdz==0 && i<(g-2)){
        j=i+1;
        while(znajdz==0 && j<(g-1)){
            k=j+1;
            sz= okresl_szukana_karte(A[i], A[j]);

            while(znajdz==0 && k<g){
                if(A[k]==sz) znajdz=1;
                k++;
            }  
            j++;
        }   i++;
    }

    if(znajdz==1){
        printf("- %d %d %d\n", A[i-1], A[j-1], A[k-1]);
        n= usun(A, i-1, n);
        n= usun(A, (j-2), n);
        n= usun(A, (k-3), n); //liczba kart zmniejszona o 3
    }

    return n; //zwraca aktualną długość tablicy (czyli ilość kart)
}

/*wypisuje karty aktualnie leżące na stole*/
void wypisz_stol(int A[], int g){
    printf("=");

    for(int i=0; i<g; i++){
        printf(" %d", A[i]);
    }
    printf("\n");
}

/**
 * Dobiera trzy karty na stół 
 * (w funkcji graj_w_set sprawdziliśm,że możemy to zrobić bez wyjścia poza zakres) */
int dobierz_trzy(int g){
    /*podana przez użytkownika lb. kart jest podzielna przez 3, więc jeśli możemy dobrać to na pewno możemy dobrać trzy*/
    g+=3; 

    printf("+\n");

    return g;
}

/**
 * Dopóki nie skończą się ruchy:
 * - wypisuje karty aktualnie leżące na stole
 * - szuka trójki kart, która stworzyłaby SET
 * - jeśłi nie udało się znaleźć takiej trójki dobiera karty
 * - jeśli nie ma z czego dobrać kart kończy grę
 */
void graj_w_set(int A[], int n){
    int w=0; //czy wykorzystano wszystkie możliwe ruchy
    int temp=0; //do sprawdzania, czy można dobrać karty
    int g=Stol; //granica stołu: pamięta ilość kart wystawionych na stół
    if(g>n) g=n; //jeśli jest mniej niż 12 kart, granicą stołu jest n

    while(w==0){
        wypisz_stol(A, g); 
        
        temp= szukaj_trojki(A, n, g);

        /*sprawdzenie, czy udało się zanleźć trójkę towrzącą SET*/
        if(n!=temp) {//trójka została znaleziona, z tablicy wyeliminowaliśmy trzy karty
            n=temp;//talia ma teraz mniej kart

            /*sprawdzamy, czy trzeba zmienić ilość kart na stole*/
            if(n>Stol){             //jeśli nadal możemy mieć 12 kart na stole to je miejmy
                if(g!=Stol) g -=3;  //jeśli wcześniej dobraliśmy: mieliśmy ponad 12 kart, to trzeba granicę stołu zmniejszyć o 3
            } else g=n; //jeśli karty w talii się skończyły to mniej kart wypiszemy na stole
        }
        /*jeśli trójka nie została znaleziona:*/
        else{
            if(n>g) g= dobierz_trzy(g);//nie ma ruchu, ale jest z czego dobrać
            else w=1; //nie ma rchu i nie ma  z czego dobrać, czyli wykorzystaliśmy wszystkie ruchy
        }
    }
}


int main(){
    int A[Talia];
    int n;

    n= wczytaj_set(A, Talia);
    graj_w_set(A, n);  

    return 0;
}