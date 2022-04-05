/*
*  Flore Daniel-Andrei, gr.30229
* Am implementat cele trei metode de sortare cerute in tema:
*	HeapSort:
*		-folosim metoda de constructie bottomUp a Heap-ului (descrisa in TEMA2) => complexitate liniara pe constructie O(n)
*		-prin iteratii succesive, vom extrage radacina Heap-ului, folosind mai apoi functia
*		-maxHeapify pentru a mentine proprietatea de Heap; se vor efectua n-1 repetitii pana se va ajunge la un heap de size 2
*		-maxHeapify are o complexitate O(logn), astfel reiesind complexitatea de O(nlogn) a heapSort-ului
* 
*	QuickSort:
*		-precum mergeSort, aplica divide-and-conquer pentru a rezolva problema sortarii
		-functia partition selecteaza un element drept pivot, in jurul caruia se vor partitiona sub-siruri
		-sub-sirurile vor fi sortate recursiv; d.p.d.v. al stabilitatii, quickSort nu este considerat ca fiind un algoritm stabil
		-complexitatea sa in avg case: O(nlogn), worst case: O(n^2), best case: O(nlogn)
	RandomizedSelect / QuickSelect:
		-similar QuickSort, diferenta e ca, in loc sa fie recursivitate pe ambele parti dupa gasirea pivot-ului
		-va alege in functie de k in care parte va merge recursiv
		-complexitatea: O(n) cu un worst case: O(n^2);
	
* OBSERVATII:
*		-conform lucrarii de laborator, au fost realizate graficele specifice cazului mediu avg, masuratorile fiind repetate de 5 ori
*		-au fost masurate totalul de operatii pentru QuickSort si HeapSort, graficul continand ambele curbe
*		-am observat faptul ca pentru cazul AVG, cel mai rapid a fost algoritmul QuickSort
*		-astfel, pt un sir de 500 elemente, diferenta dintre cele doua metode de sortare a fost in jurul a 6k operatii
		-din grafice rezulta faptul ca algoritmul QuickSelect are o curba nu tocmai liniara, fiind diferente considerabile intre nr de operatii pt diferite valori ale lui n

  CONCLUZIE:
		-algoritmul QuickSort poate fi considerat mai rapid decat HeapSort pentru cazul Average cu 5 reperati ale operatiilor 
  
	QUICKSORT WORST&BEST CASE
	---Worst Case
		-pentru worst case, am ales sa utilizam un sir deja sortat crescator, astfel, pivotul va fi ales asa incat o partitie va avea n-1 elemente, iar cealalta 1, rezultand un numar considerabil de operatii
		-complexitate: O(n^2)
	---Best Case
		-pentru best case, am ales sa utilizam un sir sortat descrescator, rezultand o complexitate O(nlogn)
*/	

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5
int n;
Profiler prof("sortari");

int parent(int i)
{
	return (i - 1) / 2;
}

int left(int i)
{
	return 2 * i + 1;
}

int right(int i)
{
	return 2 * i + 2;
}

void swap(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;

}

void printArray(int v[], int n)
{
	printf("Array-ul obtinut este: \n");
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

void copyArray(int v1[], int v2[], int n)
{
	for (int i = 0; i < n; i++)
		v2[i] = v1[i];
}


//heapsort
void maxHeapify(int v[], int heapSize, int i)
{

	Operation opTotalHS1 = prof.createOperation("heapSort1-total", n);
	int largest = i;
	int l = left(i);
	int r = right(i);
	if (l < heapSize)
	{
		opTotalHS1.count();
		if (v[l] > v[largest])
			largest = l;
	}

	if (r < heapSize)
	{
		opTotalHS1.count();
		if (v[r] > v[largest])
			largest = r;
	}

	if (largest != i)
	{
		opTotalHS1.count(3);
		swap(&v[i], &v[largest]);
		maxHeapify(v, heapSize, largest);
	}
}

void buildMaxHeapBU(int v[], int length)
{
	int index = (length / 2) - 1;
	for (int i = index; i >= 0; i--)
	{
		maxHeapify(v, length, i);
	}

}

void heapSort(int v[], int length)
{
	Operation opTotalHS2 = prof.createOperation("heapSort2-total", n);
	buildMaxHeapBU(v, length);
	int heapSize = length;
	for (int i = length - 1; i >= 1; i--)
	{
		opTotalHS2.count(3);
		swap(&v[0], &v[i]);
		heapSize -= 1;
		maxHeapify(v, heapSize, 0);

	}
}


//quicksort
int partition(int v[], int p, int r)
{
	Operation opTotalQS = prof.createOperation("quickSort-total", n);
	opTotalQS.count();
	int x = v[r];
	int i = p-1;
	for (int j = p; j <= r - 1; j++)
	{
		opTotalQS.count();
		if (v[j] <= x)
		{
			i = i + 1;
			opTotalQS.count(3);
			swap(&v[i], &v[j]);
		}
	}
	opTotalQS.count(3);
	swap(&v[i + 1], &v[r]);
	return i + 1;
}

void quickSort(int v[], int p, int r)
{
	if (p < r)
	{
		int q = partition(v, p, r);
		quickSort(v, p, q - 1);
		quickSort(v, q + 1, r);
	}
	
}

//quickselect
int randomizedPartition(int v[], int p, int r)
{
	int i = rand() % r + p;
	swap(&v[r], &v[i]);
	return partition(v, p, r);
}

int randomizedSelect(int v[], int p, int r, int i)
{
	if (p == r)
		return v[p];
	int q = randomizedPartition(v, p, r);
	int k = q - p + 1;
	if (i == k)
		return v[q];
	else if (i < k)
		return randomizedSelect(v, p, q - 1, i);
	else
		return randomizedSelect(v, q + 1, r, i - k);

}

void demo()
{
	int v[] = { 1,3,6,5,9,8,11,0 };
	int length = 8;
	int vCopy1[20];
	int vCopy2[20];
	copyArray(v, vCopy1, length);
	copyArray(v, vCopy2, length);
	heapSort(v, length);
	printArray(v, length);
	quickSort(vCopy1, 0, length-1);
	printArray(vCopy1, length);
	quickSort(vCopy2, 0, length - 1);
	printArray(vCopy2, length);
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
			heapSort(v, n);
			quickSort(vCopy1, 0, n - 1);
		}
	}
	
	prof.divideValues("heapSort1-total", NR_TESTS);
	prof.divideValues("heapSort2-total", NR_TESTS);
	prof.addSeries("heapSort-total", "heapSort1-total", "heapSort2-total");
	prof.divideValues("quickSort-total", NR_TESTS);
	prof.createGroup("total", "heapSort-total", "quickSort-total");
	prof.showReport();
}

void perf_quickSort_bestworst(int order)
{
	int v[MAX_SIZE];
	int vCopy1[MAX_SIZE];
	for (int test = 0; test < NR_TESTS; test++)
	{
		for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
		{
			FillRandomArray(v, n,10,50000,false, order);
			copyArray(v, vCopy1, n);
			quickSort(vCopy1, 0, n - 1);
		}
	}
	prof.divideValues("quickSort-total", NR_TESTS);
}

void perf_quickSort_bestworst_all()
{
	prof.reset("worst");
	perf_quickSort_bestworst(ASCENDING);
	prof.reset("best");
	perf_quickSort_bestworst(DESCENDING);
	prof.showReport();
}
int main()
{
	//demo();
	perf();
	perf_quickSort_bestworst_all();
}