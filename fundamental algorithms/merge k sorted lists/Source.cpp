/*
* Flore Daniel-Andrei, gr. 30229
* 
* -----CERINTA-----
*	-se dau k liste ordonate cu un nr total de n elemente => n/k elemente per fiecare lista
*   -se cere un algoritm de interclasare a celor k liste in mod eficient
* 
* -----EXPLICATII-----
*	-pentru a rezolva problema si a gasi solutia intr-un mod eficient, am folosit un minHeap
*   -in minHeap am mentinut constant primele valori din fiecare lista, respectand constant proprietatea de minHeap
*   -astfel, avand capetele listelor si valorile acestora in forma de minHeap, vom extrage radacina heap-ului (de obicei primul nod din prima lista dupa buildHeap)
*   -locul acelui nod va fi luat de urmatorul nod din lista, repetandu-se procesul de minHeapify; nodul extras va fi mai apoi adaugat la sfarsitul unei liste finale, lista care va reprezenta interclasarea propriu-zisa
*   -la final, minHeap-ul va fi epuizat toate nodurile listelor, noduri care au fost trecute rand pe rand in lista finala
*
* -----COMPLEXITATE-----
*	-buildMinHeap -> O(k)
*	-insert_last (in lista finala) -> O(1)
*	-minHeapify -> O(logk)
*	^^toate acestea se vor repeta de n ori in bucla while, pana toate nodurile vor fi introduse in ordinea ceruta in lista finala
*		=>> O(nlogk)
* 
* -----OBSERVATII GRAFIC-----
*	-dupa cum era de asteptat, numarul total de operatii pe care algoritmul nostru il realizeaza este direct proportional cu numarul de liste de k elemente care trebuie interclasate, cat si in functie de nr total de elemente n
*	-astfel, din primul grafic rezulta faptul ca, odata cu cresterea lui k si n, numarul de operatii va fi mult mai mare
*	-observam faptul ca numarul total de elemente n nu afecteaza intr-un mod grav numarul de operatii, acesta fiind afectat in special de nr de subsiruri de interclasat
* 
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 400
#define NR_TESTS 5
Profiler prof("mergeKLists");
int n, k; //n-nr total elem, k-nr liste

int nrOp; //variabila pt nr de operatii

typedef struct node {
	int key;
	struct node* next;
}node;

typedef struct list {
	node* first;
	node* last;
}list;

list* list_new()
{
	list* l = (list*)malloc(sizeof(list));
	if (l == NULL)
	{
		printf("Nu s-a putut aloca lista noua.");
		exit(1);
	}
	l->first = NULL;
	l->last = NULL;
	return l;
}

void list_free(list* l)
{
	if(l == NULL)
	{
		return;
	}
	node* it = l->first;
	while (it != NULL)
	{
		node* next = it->next;
		free(it);
		it = next;
	}
	free(l);
}

void list_insert_last(list* l, int key)
{
	if (l == NULL)
	{
		return;
	}
	node* p = (node*)malloc(sizeof(node));
	p->key = key;
	p->next = NULL;
	nrOp++;
	if (l->last == NULL)
	{
		nrOp++;
		l->first = p;
		l->last = p;
	}
	else
	{
		
		nrOp++;
		l->last->next = p;
		l->last = p;
	}
}

void list_delete_first(list* l)
{
	nrOp++;
	if(l->first == NULL)
	{
		return;
	}
	nrOp++;
	node* p = l->first;
	l->first = l->first->next;
	free(p);
}

void list_print(list* l)
{
	if (l == NULL)
	{
		printf("Lista e goala.");
	}
	node* p = l->first;
	while (p)
	{
		printf("%d->", p->key);
		p = p->next;
		if (p == NULL)
		{
			printf("NULL");
		}
	}
	printf("\n");
}

void swap_lists(list* l1, list* l2)
{
	list laux;
	laux = *l1;
	*l1 = *l2;
	*l2 = laux;
}


int parent(int i)
{
	return i / 2;
}

int left(int i)
{
	return (2 * i + 1);
}

int right(int i)
{
	return (2 * i + 2);
}

void minHeapify(list* listArray[], int i, int* heapSize)
{
	int smallest = i;
	int l = left(i);
	int r = right(i);
	nrOp++;
	if (l < *heapSize && listArray[l]->first->key < listArray[smallest]->first->key)
	{
		smallest = l;
	}
	nrOp++;
	if (r < *heapSize && listArray[r]->first->key < listArray[smallest]->first->key)
	{
		smallest = r;
	}
	if (smallest != i)
	{
		nrOp += 3;
		swap_lists(listArray[i], listArray[smallest]);
		minHeapify(listArray, smallest, &(*heapSize));
	}
}

void buildMinHeap(list* listArray[], int* heapSize)
{
	for (int i = *heapSize / 2 - 1; i >= 0; i--)
	{
		minHeapify(listArray, i, &(*heapSize));
	}
}

list* merge_k_lists(list* listArray[], int k)
{
	int heapSize = k;
	buildMinHeap(listArray, &heapSize);

	list* final_list = list_new();

	while (heapSize > 0)
	{
		list_insert_last(final_list, listArray[0]->first->key);
		list_delete_first(listArray[0]);
		if (listArray[0]->first == NULL)
		{
			free(listArray[0]);
			nrOp++;
			listArray[0] = listArray[heapSize - 1];
			heapSize--;
		}
		minHeapify(listArray, 0, &heapSize);
	}
	return final_list;
}

void demo()
{
	n = 20;
	k = 4;
	int v[100];
	list* listArray[100];
	for (int i = 0; i < k; i++)
	{
		FillRandomArray(v, n / k, 0, 100, false, ASCENDING);
		listArray[i] = list_new();
		for (int j = 0; j < n / k; j++)
		{
			list_insert_last(listArray[i], v[j]);
		}
	}
	for (int q = 0; q < k; q++)
	{
		list_print(listArray[q]);
	}
	printf("\n");

	list* final_list = merge_k_lists(listArray, k);
	
	printf("Lista obtinuta dupa interclasare este: \n");
	list_print(final_list);
}

void perf()
{
	int k1 = 5;
	for (int teste = 0; teste < NR_TESTS; teste++)
	{
		for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
		{	nrOp= 0;
			int v[20001];
			list* listArray[7];
			for (int i = 0; i < k1; i++)
			{
				FillRandomArray(v, n / k1, 10, 10000, false, ASCENDING);
				listArray[i] = list_new();
				for (int j = 0; j < n / k1; j++)
				{
					list_insert_last(listArray[i], v[j]);
				}
			}
			list* final_list = merge_k_lists(listArray, k1);
			Operation nrOpK5 = prof.createOperation("nrOp k=5", n);
			nrOpK5.count(nrOp);
		}	
	}
	int k2 = 10;
	for (int teste = 0; teste < NR_TESTS; teste++)
	{
		for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
		{
			nrOp = 0;
			int v[20001];
			list* listArray[15];
			for (int i = 0; i < k2; i++)
			{
				FillRandomArray(v, n / k2, 10, 10000, false, ASCENDING);
				listArray[i] = list_new();
				for (int j = 0; j < n / k2; j++)
				{
					list_insert_last(listArray[i], v[j]);
				}
			}
			list* final_list = merge_k_lists(listArray, k2);
			Operation nrOpK10 = prof.createOperation("nrOp k=10", n);
			nrOpK10.count(nrOp);
		}
	}
	int k3 = 100;
	for (int teste = 0; teste < NR_TESTS; teste++)
	{
		for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
		{
			nrOp = 0;
			int v[20001];
			list* listArray[103];
			for (int i = 0; i < k3; i++)
			{
				FillRandomArray(v, n / k3, 10, 10000, false, ASCENDING);
				listArray[i] = list_new();
				for (int j = 0; j < n / k3; j++)
				{
					list_insert_last(listArray[i], v[j]);
				}
			}
			list* final_list = merge_k_lists(listArray, k3);
			Operation nrOpK100 = prof.createOperation("nrOp k=100", n);
			nrOpK100.count(nrOp);
		}
	}

	for (int teste = 0; teste < NR_TESTS; teste++)
	{
		n = MAX_SIZE;
		for (k = 10; k <= 500; k += 10) 
		{
			nrOp = 0;
			int v[20001];
			list* listArray[600];
			for (int i = 0; i < k; i++)
			{
				FillRandomArray(v, n / k, 10, 10000, false, ASCENDING);
				listArray[i] = list_new();
				for (int j = 0; j < n / k; j++)
				{
					list_insert_last(listArray[i], v[j]);
				}
			}
			list* final_list = merge_k_lists(listArray, k);
			Operation nrOpN10000 = prof.createOperation("nrOp n=10000", k);
			nrOpN10000.count(nrOp);
		}
	}

	prof.divideValues("nrOp k=5", 5);
	prof.divideValues("nrOp k=10", 5);
	prof.divideValues("nrOp k=100", 5);
	prof.createGroup("nrOp", "nrOp k=5", "nrOp k=10", "nrOp k=100");
	prof.divideValues("nrOp n=10000", 5);
	prof.showReport();
}

int main()
{
	//demo();
	perf();
	return 0;
}


