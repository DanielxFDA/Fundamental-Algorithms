/*
* Flore Daniel-Andrei, gr. 30229
* Am implementat cele doua metode de construire a structurii de date Heap: 
*		Bottom-up ("de jos in sus", respectiv Top-down ("de sus in jos");
*	***Metoda de construire Bottom-up a Heap-ului
*		-este necesara pentru construirea unui Heap dintr-un sir cu n elemente apelarea succesiva a functiei heapify
		-functia heapify are scopul de a reface structura de heap atunci cand se adauga un element in heap
		-prin aceste apeluri recursive se va cauta pozitia radacinii in viitorul heap rezultat
		-d.p.d.v. al complexitatii: worst case: sir ordonat crescator - fiecare element inserat va fi repozitionat la nivelul frunzelor
			=> complexitate liniara O(n)
	***Metoda de construire Top-Down a Heap-ului
*		-algoritm specific utilizarii heap-ului ca structura de date: coada de prioritati cu operatiile aferente
*		-pentru construirea heap-ului dintr-un sir, la inserarea unui nou element din sirul dorit in heap acesta va fi pozitionat drept frunza
*			urmand a i se cauta pozitia lui corecta din arbore	
*		-d.p.d.v. al complexitatii: worst case: sir ordonat crescator - fiecare element inserat trebuie dus pana la root
*		-heap-ul va creste liniar odata cu inserarea elementelor, aducerea acestora pe pozitia radacinii va fi logaritmica
*			=> complexitate O(nlogn)
*	***Observatii
*		-conform lucrarii de laborator, au fost realizate graficele specifice cazului mediu AVG, masuratorile fiind repetate de 5 ori
		-am observat faptul ca pe AVG, cel mai rapid dintre algoritmi a fost cel Buttom-up d.p.d.v. al numarului total de operatii
			astfel, daca algoritmul Top-Down a efectuat pt un sir de 500 elemente aprox 11k operatii, cel Bottom-up a efectual in jur de 6k operatii
		-din grafice, rezulta faptul ca Buttom-up prezinta o crestere liniara, pe cand cel Top-down are o crestere rapida, cu coef. logaritmic
	***Concluzie
		-algoritmul Bottom-up poate fi considerat mai rapid decat cel Top-down in cazul average cu 5 repetari ale operatiilor.
*/

#include <stdio.h>
#include <math.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5
int n;

Profiler p("buildHeap");

int parent(int i)
{
	return (i-1) / 2;
}

int left(int i)
{
	return 2 * i+1;
}

int right(int i)
{
	return 2 * i + 2;
}

void copyArray(int v1[], int v2[], int n)
{
	for (int i = 0; i < n; i++)
		v2[i] = v1[i];
}

void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;

}

void maxHeapify(int v[], int heapSize, int i)
{
	Operation opComp = p.createOperation("buildHeapBU-comp", heapSize);
	Operation opAttr = p.createOperation("buildHeapBU-attr", heapSize);

	int largest = i;
	int l = left(i);
	int r = right(i);
	if (l < heapSize)
	{
		opComp.count();
		if(v[l] > v[largest])
			largest = l;
	}
		
	if (r < heapSize)
	{
		opComp.count();
		if(v[r] > v[largest])
				largest = r;
	}
		
	if (largest != i)
	{
		opAttr.count(3);
		swap(&v[i], &v[largest]);
		maxHeapify(v,heapSize, largest);
	}
}

void buildMaxHeapBU(int v[], int length)
{
	int index = (length / 2)-1 ;
	for (int i = index; i >= 0; i--)
	{
		maxHeapify(v, length, i);
	}
		
}

void heapSortBU(int v[], int length)
{
	buildMaxHeapBU(v, length);
	int heapSize = length;
	for (int i = length-1; i >= 1; i--)
	{
		swap(&v[0], &v[i]);
		heapSize -= 1;
		maxHeapify(v, heapSize, 0);

	}
}

void heapIncreaseKey(int v[], int i, int key)
{
	Operation opComp = p.createOperation("buildHeapTD-comp", n);
	Operation opAttr = p.createOperation("buildHeapTD-attr", n);
	opComp.count();
	if (key < v[i])
	{
		printf("new key is smaller than current key");
		return;
	}
	opAttr.count();
	v[i] = key;
	while (i > 0)
	{
		opComp.count();
		if (v[parent(i)] < v[i])
		{
			opAttr.count(3);
			swap(&v[i], &v[parent(i)]);
			i = parent(i);
		}
		else
			break;
		
	}
		
}

void maxHeapInsert(int v[], int* heapSize, int key)
{
	*heapSize += 1;
	v[*heapSize] = -INFINITY;
	heapIncreaseKey(v, *heapSize, key);
}

void buildMaxHeapTD(int v[], int length)
{
	int heapSize = 0;
	for (int i = 1; i < length; i++)
		maxHeapInsert(v, &heapSize, v[i]);
}

void heapSortTD(int v[], int length)
{
	buildMaxHeapTD(v, length);
	int heapSize = length;
	for (int i = length - 1; i >= 1; i--)
	{
		
		swap(&v[0], &v[i]);
		heapSize -= 1;
		maxHeapify(v, heapSize, 0);

	}
}

void printHeap(int v[], int n)
{
	printf("Heap-ul obtinut este: \n");
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

void demo()
{
	int v[] = { 1,3,6,5,9,8 };
	int length = 6;
	//buildMaxHeapBU(v, length);
	//heapSortBU(v, length);
	//buildMaxHeapTD(v, length);
	//heapSortBU(v, length);
	//heapSortTD(v, length);
	printHeap(v, length);
}

void perf()
{
	int v[MAX_SIZE];
	
	int vCopy1[MAX_SIZE];
	for (int test = 0; test < NR_TESTS; test++)
	{
		for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
		{
			FillRandomArray(v, n);
			copyArray(v, vCopy1, n);
			buildMaxHeapBU(v, n);
			buildMaxHeapTD(vCopy1, n);
		}
	}
	p.divideValues("buldHeapTD-comp", NR_TESTS);
	p.divideValues("buildHeapTD-attr", NR_TESTS);
	p.addSeries("buildHeapTD-total", "buildHeapTD-comp", "buildHeapTD-attr");
	p.divideValues("buldHeapBU-comp", NR_TESTS);
	p.divideValues("buildHeapBU-attr", NR_TESTS);
	p.addSeries("buildHeapBU-total", "buildHeapBU-comp", "buildHeapBU-attr");
	p.createGroup("total", "buildHeapBU-total", "buildHeapTD-total");
	p.createGroup("total-comp", "buildHeapBU-comp", "buildHeapTD-comp");
	p.createGroup("total-attr", "buildHeapBU-attr", "buildHeapTD-attr");
	p.showReport();

}

int main()
{
	//demo();
	perf();
}