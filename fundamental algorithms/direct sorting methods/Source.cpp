/*
* Flore Daniel-Andrei gr. 30229
* Am implementat 3 algoritmi de sortare: bubbleSort, selectionSort, insertionSort
* ***bubbleSort functioneaza prin interschimbari repetare intre elementele adiacente
*            este un algoritm de sortare stabil; Best: O(n), Worst: O(n^2)
* ***selectionSort functioneaza prin cautari repetate ale elementului minim din sir
*               plasandu-l dupa gasire la inceputul sirului va mentine astfel 2 subsiruri: 
                unul sortat la inceput si cel nesortat; O(n^2)
* ***insertionSort functioneaza impartind sirul in doua parti: una sortata si una nesortata
*              de fiecare data cand gaseste un element nou, il va compara cu elementele dinaintea sa
*              la final fiind inserat pe pozitia potrivita
*              este un algoritm de sortare stabil; O(n^2)
* 
* Am observat ca pe average case cel mai rapid dintre algoritmi a fost selectionSort;
*   insa este de remarcat faptul ca d.p.d.v. al: 
*            nr. total de op, diferenta dintre acesta si insertionSort este de ordinul a aprox. 100 de operatii
*            nr. de atribuiri, selectionSort are cele mai putine
*            nr. de comparatii, insertionSort are cele mai putine
* 
* Am observat ca pe best case cel mai rapid dintre algoritmi a fost insertionSort;
*   insa este de remarcat faptul ca d.p.d.v. al: 
*            nr. total de op, diferenta dintre acesta si ceilalti doi algoritmi este de aprox. 120.000 operatii 
*            nr. de atribuiri, bubbleSort are 0 pentru orice numar de date de intrare 
*            nr. de comparatii, insertionSort are cele mai putine, ceilalti doi algoritmi avand un numar egal de 124.750
* 
* Am observat ca pe worst case cel mai rapid dintre algoritmi a fost selectionSort;
*   insa este de remarcat faptul ca d.p.d.v. al: 
*            nr. total de op, diferenta dintre acesta si ceilalti doi algoritmi este de ordinul sutelor de mi
*            nr. de atribuiri, selectionSort are cele mai putine, fiind urmat de ceilalti doi algoritmi cu nr considerabile de op
*            nr. de comparatii, toti cei 3 algoritmi prezinta acelasi numar pentru fiecare set de date, fiind diferentiati de nr de atribuiri
*
* 
*/
#include <stdio.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5

Profiler p("sortari");

//functie swap 
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// functia sortare bubbleSort
void bubbleSort(int v[], int n)
{
    Operation opComp = p.createOperation("bubble-comp", n);
    Operation opAttr = p.createOperation("bubble-attr", n);
    int i, j;
    for (i = 0; i < n - 1; i++) 
        for (j = 0; j < n - i - 1; j++) {
            opComp.count();
            if (v[j] > v[j + 1]) {
                opAttr.count(3);
                swap(&v[j], &v[j + 1]);
            }
                
        }   
}

// functia sortare selectionSort
void selectionSort(int v[], int n)
{
    Operation opComp = p.createOperation("selection-comp", n);
    Operation opAttr = p.createOperation("selection-attr", n);
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            opComp.count();
            if (v[j] < v[min_idx])
                min_idx = j;
        }
        opAttr.count(3);
        swap(&v[min_idx], &v[i]);
    }
}

//functie sortare insertionSort
void insertionSort(int v[], int n)
{
    Operation opComp = p.createOperation("insertion-comp", n);
    Operation opAttr = p.createOperation("insertion-attr", n);
    int i, key, j;
    for (i = 1; i < n; i++) {
        opAttr.count();
        key = v[i];
        j = i - 1;
        while (j >= 0 ) {
            opComp.count();
            if (v[j] < key)
                break;
            opAttr.count();
            v[j + 1] = v[j];
            j = j - 1;
        }
        opAttr.count();
        v[j + 1] = key;
    }
}
//afisare vector
void printArray(int v[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", v[i]);
    printf("\n");
}

//copiere data from v1 to v2
void copyArray(int v1[], int v2[], int n)
{
    for (int i = 0; i < n; i++) {
        v2[i] = v1[i];
    }
        
}

void demo()
{
    int v[10] = { 3,4,1,1,6,7,8 };
    int n = 7;
    //bubbleSort(v, n);
    //selectionSort(v, n);
    //insertionSort(v, n);
    printArray(v, n);
}

void perf(int order)
{
    int v[MAX_SIZE];
    int n;
    int vCopy1[MAX_SIZE];
    int vCopy2[MAX_SIZE];
    for (int test = 0; test < NR_TESTS; test++)
    {
        for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
            {
                FillRandomArray(v, n,10,50000, false, order);
                copyArray(v, vCopy1, n);
                copyArray(v, vCopy2, n);
                bubbleSort(v, n);
                insertionSort(vCopy1, n);
                selectionSort(vCopy2, n);
            }
    }
    p.divideValues("bubble-attr", NR_TESTS);
    p.divideValues("bubble-comp", NR_TESTS);
    p.addSeries("bubble-total", "bubble-attr", "bubble-comp");
    p.divideValues("insertion-attr", NR_TESTS);
    p.divideValues("insertion-comp", NR_TESTS);
    p.addSeries("insertion-total", "insertion-attr", "insertion-comp");
    p.divideValues("selection-attr", NR_TESTS);
    p.divideValues("selection-comp", NR_TESTS);
    p.addSeries("selection-total", "selection-attr", "selection-comp");
    p.createGroup("total", "bubble-total", "selection-total", "insertion-total");
    p.createGroup("total-attr", "bubble-attr", "selection-attr", "insertion-attr");
    p.createGroup("total-comp", "bubble-comp", "selection-comp", "insertion-comp");
    //p.showReport();
}

void perf_all()
{
    perf(UNSORTED);
    p.reset("best");
    perf(ASCENDING);
    p.reset("worst");
    perf(DESCENDING);
    p.showReport();
}
int main()
{
    //demo();
    //perf();
    perf_all();
}