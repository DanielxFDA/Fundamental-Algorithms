/*
*	Flore Daniel-Andrei, gr. 30229
* --------------OBSERVATII--------------
*	Hash Table - are rolul de a determina cat mai rapid daca o cheie se afla sau nu intr-o tabela
*	-celelalte operatii specifice structurilor de date se realizeaza in timp cat mai putin
*	-conform cerintelor de laborator, am implementat o astfel de structura care se bazeaza 
* pe adresare deschisa, functia de dispersie fiind una quadratica
*	-quadratic probing are un plus fata de celelalte functii de hash deoarece imprastie elementele
* apropiate, diminuand coliziunile
*	-fiind adresare deschisa, factorul alpha ( de umplere ) va fi <=1; elementele inserate se vor afla
* exclusiv in sloturile tabelei (nu apare o alta structura de date pt chaining)
*	-pentru a realiza cerinta (observarea in caz mediu statistic cum functioneaza o cautare cu succes si o cautare fara succes)
*	-m-am folosit de vectorul v (de aici am introdus in hashtable valorile); astfel: am parcurs vectorul pe 3000 elemente cu indici random
*	-pentru i impar, am luat valorile reale din vector(cele care se gasesc in hashtable) pentru a verifica numarul de operatii pentru chei gasite cu succes
*	-pentru i par, am luat valorile reale din vector si le-am adunat o valoare a.i. sa depaseasca orice valoare prezenta in hashtable
*	**search: in adresarea directa, continua sa verifice pana cand valoarea slotului este egala cu cheia cautata sau gaseste un spatiu gol
*	**insert: in adresarea directa, continua sa verifice pana gaseste un slot gol, cand il gaseste, insereaza valoarea cheii
* 
* --------------OBSERVATII TABEL--------------
*	-observam ca efortul real se apropie de valorile teoretice
*	-numarul de celule accesate creste odata cu cresterea factorului de umplere
*	-in cazul in care cautarea returneaza un succes, cresterea este lenta, nesemnificativa
*	-in cazul insucceselor, cresterea este semnificativa; este determinata totodata de lipsa spatiilor libere
* care ar fi oprit cautarea din timp
* 
* --------------COMPLEXITATI--------------
* asteptam, in medie:   o cautare cu succes: (1/alpha)ln(1/(1-alpha) accese
*						o cautare fara succes: 1/(1-alpha) accese
*						o inserare: 1/(1-alpha)
* 
* 
* 
* 
*/


#include <stdio.h>
#include "Profiler.h"
#include <time.h>

Profiler prof("hashTable");

typedef struct cell {
	int key;
	int status;
}Cell;

enum {FREE, OCCUPIED};

void afisare_hash_table(Cell* T, int m)
{
	printf("\nTabela de dispersie este: \n");
	for (int i = 0; i < m; i++) {
		if (T[i].status == OCCUPIED)
			printf("T[%d]=%d\n", i, T[i].key);
		else
			printf("T[%d]=--\n", i);
	}
}

int h_prime(int k, int m)
{
	int poz = k % m;
	if (poz < 0)
		poz += m;
	return poz;
}

int c1 = 2;
int c2 = 3;
int quadratic_probing(int k, int m, int i)
{
	return (h_prime(k, m) + c1*i + c2*(i * i)) % m;
}

void insert_key(int k, Cell* T, int m)
{
	for (int i = 0; i < m; i++)
	{
		int poz = quadratic_probing(k, m, i);
		if (T[poz].status == FREE)
		{
			T[poz].key = k;
			T[poz].status = OCCUPIED;
			return;
		}
	}
	printf("hashTable plin. Nu se poate insera %d.\n", k);
}

int nrCeluleAccesate=0;

int search_key(int k, Cell* T, int m)
{
	for (int i = 0; i < m; i++)
	{
		nrCeluleAccesate++;
		int poz = quadratic_probing(k, m, i);
		if (T[poz].status == FREE)
		{
			return -1;
		}
			
		else
			if (T[poz].key == k)
			{
				return poz;
			}
				
	}
	return -1;
}

void set_table_free(Cell* T, int m)
{
	int i;
	for (i = 0; i < m; i++)
	{
		T[i].status = FREE;
	}
}

void demo()
{
	int m = 10;
	Cell* T = (Cell*)malloc(m * sizeof(Cell));
	set_table_free(T, m);
	int v[] = { 19, 36, 5, 21, 4, 26, 14, 17, 13};
	for (int i = 0; i < sizeof(v) / sizeof(int); i++)
		insert_key(v[i], T, m);
	afisare_hash_table(T, m);

	printf("Cauta 36: Gasit %d ", search_key(36, T, m));
}

int N = 9973; //nr total de elemente din tabel
#define NR_TESTS 5
void perf()
{
	Cell* T = (Cell*)malloc(N * sizeof(Cell));
	set_table_free(T, N);
	int v[9974];
	float alpha[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };
	float efort_mediu_gasite[5];
	float efort_maxim_gasite[5];
	float efort_mediu_negasite[5];
	float efort_maxim_negasite[5];
	for (int i = 0; i < 5; i++)
	{
		efort_mediu_gasite[i] = 0;
		efort_maxim_gasite[i] = 0;
		efort_mediu_negasite[i] = 0;
		efort_maxim_negasite[i] = 0;
	}
	int n; // nr de elemente ce trebuie inserate
	for (int test = 0; test < NR_TESTS; test++)
	{
		FillRandomArray(v, N, 1, 500000);
		for (int caz = 0; caz < 5; caz++)
		{
			n =(int) (N * alpha[caz]); //atatea elemente vor fi inserate in tabel
			for (int q = 0; q < n; q++)
			{
				insert_key(v[q], T, N);
			}
			for (int i = 0; i < 3000; i++)
			{
				nrCeluleAccesate = 0;
				if (i % 2)
				{
					search_key(v[rand() % n], T, N);
					efort_mediu_gasite[caz] += nrCeluleAccesate;
					if (nrCeluleAccesate > efort_maxim_gasite[caz])
						efort_maxim_gasite[caz] = nrCeluleAccesate;
				}
				else
				{
					search_key(v[rand() % n] + 500000, T, N);
					efort_mediu_negasite[caz] += nrCeluleAccesate;
					if (nrCeluleAccesate > efort_maxim_negasite[caz])
						efort_maxim_negasite[caz] = nrCeluleAccesate;
				}
			}

			set_table_free(T, N);
		}
		
	}

	for (int i = 0; i < 5; i++)
	{
		efort_mediu_negasite[i] /= (3000 / 2 * 5);
		efort_mediu_gasite[i] /= (3000 / 2 * 5);
	}
	printf("Caz  | Efort mediu gasite | Efort maxim gasite | Efort mediu negasite | Efort maxim negasite\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%-5.2f| %-19.2f| %-19.0f| %-21.2f| %.0f\n", alpha[i], efort_mediu_gasite[i], efort_maxim_gasite[i], efort_mediu_negasite[i], efort_maxim_negasite[i]);
	}
	
	
}
int main()
{
	//demo();
	srand(time(0));
	perf();
}