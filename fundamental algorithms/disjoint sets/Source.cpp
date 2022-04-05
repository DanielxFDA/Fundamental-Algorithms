/*
* Flore Daniel-Andrei
* grupa 30229
* *****CERINTA*****
* implementarea corecta si eficienta a operatiilor de baza pe multimi disjuncte si a algoritmului lui Kruskal folosind multimi disjuncte
* *****EXPLICATII*****
* MAKE_SET(X) 
*	-creeaza o multime noua ce contine elementul x
* UNION(X,Y)
*	-realizeaza reuniunea intre multimea care il contine pe x si multimea care il contine pe y
*	-multimile dinainte vor fi sterse in prealabil
*	-union by rank tine cont de inaltimea celor doi arbori pt a realiza reuniunea
* FIND_SET(X)
*	-cauta multimea in padurea de arbori din care face parte elementul x
*	-euristica path compression leaga toate elementele de pe ramura cu x la radacina arborelui
* Pe langa functiile de baza pentru multimi disjuncte, am implementat algoritmul lui Kruskal, pe langa care am mai implementat
* o functie de generare a unui arbore pentru a putea rezolva cerinta; totodata, am mai implementat si o functie de numarare a componentelor conexe
* *****EVALUARE*****
* Complexitate Kruskal: O(ElgV) (E-edges, V - vertex)
* Complexitate Union: O(mlgn)
* Complexitate Findset: O(n)
* Complexitate Makeset: O(lgn)
*/


#include <stdio.h>
#include <malloc.h>
#include <vcruntime_string.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("Kruskal");
#define NR_TESTS 5
#define MAX_VALUE 10000
#define STEP_SIZE 500
int n;
int comp_u, comp_fs, comp_k, comp_ms;
int atr_u, atr_fs, atr_k, atr_ms;

using namespace std;


typedef struct Node {
	int key;
	int rank;
	Node* parent;
	Node* next;
}Node;

typedef struct Forest {
	Node* first;
	Node* last;
}Forest;

int make_set(Forest* f, int key)
{
	if (f == NULL)
		return 0;
	Node* n = (Node*)calloc(1,sizeof(Node));
	if (n == NULL)
		return 0;
	if (f->first == NULL)
	{
		f->first = n;
		f->last = n;
	}
	else
	{
		f->last->next = n;
		f->last = n;
	}
	atr_ms += 3;
	n->key = key;
	n->rank = 0;
	n->parent = n;
}

Node* get_nth(Forest* f, int n)
{
	if (f == NULL || n < 0)
		return 0;
	Node* result = f->first;
	while (n)
	{
		result = result->next;
		if (result == NULL)
			return 0;
		n--;
	}
	return result;
}

void free_forest(Forest* f)
{
	if (f == NULL)
		return;
	if (f->first != NULL)
	{
		Node* n = f->first;
		Node* p;
		while (n)
		{
			p = n->next;
			free(n);
			n = p;
		}
	}
	free(f);
}

Node* find_set(Node* n)
{
	if (n == NULL)
		return 0;
	comp_fs++;
	if (n != n->parent)
	{
		n->parent = find_set(n->parent);
		atr_fs++;
	}
	return n->parent;
}

void link(Node* x, Node* y)
{
	if (x == NULL || y == NULL)
		return;
	comp_u++;
	if (x->rank > y->rank)
	{
		atr_u++;
		y->parent = x;
	}
	else
	{
		atr_u++;
		x->parent = y;
		comp_u++;
		if (x->rank == y->rank)
		{
			atr_u++;
			y->rank += 1;
		}
	}
}

void set_union(Node* x, Node* y)
{
	if (x == NULL || y == NULL)
		return;
	link(find_set(x), find_set(y));
}

void print_sets(Forest* f)
{
	if (f == NULL)
		return;
	Node* n = f->first;
	printf("Setul actual este:  ");
	while (n)
	{
		printf("%d(%d) ", n->key, n->parent->key);
		n = n->next;
	}
	printf("\n");
}

//kruskal
typedef struct Edge {
	int a;
	int b;
	int weight;
}Edge;

int** kruskal(int** G, int length)
{
	int alloc_failure = 0;
	int result;
	int edge_count;
	Forest* f = 0;
	Edge* e = 0;
	int** A = (int**)calloc(length, sizeof(int*));
	if (A == NULL)
		return 0;
	for (int i = 0; i < length; i++)
	{
		A[i] = (int*)calloc(length, sizeof(int));
		if (A[i] == NULL)
		{
			alloc_failure = 1;
			goto end;
		}
	}
	f = (Forest*)calloc(1, sizeof(Forest));
	if (f == NULL)
	{
		alloc_failure = 1;
		goto end;
	}
	e = (Edge*)calloc(length * length, sizeof(Edge));
	if (e == NULL)
	{
		alloc_failure = 1;
		goto end;
	}
	for (int i = 0; i < length; i++)
	{
		result = make_set(f, i);
		if (result == 0)
		{
			alloc_failure = 1;
			goto end;
		}
	}
	edge_count = 0;
	for (int i = 0; i < length - 1; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (G[i][j] != 0)
			{
				e[edge_count].a = i;
				e[edge_count].b = j;
				e[edge_count].weight = G[i][j];
				edge_count++;
			}
		}
			
	}
	//sortare
	for (int i = 0; i < edge_count - 1; i++)
	{
		for (int j = i + 1; j < edge_count; j++)
		{
			if (e[i].weight > e[j].weight)
			{
				Edge aux = e[i];
				e[i] = e[j];
				e[j] = aux;
			}
		}
	}
	for (int i = 0; i < edge_count; i++)
	{
		if (find_set(get_nth(f, e[i].a)) != find_set(get_nth(f, e[i].b)))
		{
			A[e[i].a][e[i].b] = G[e[i].a][e[i].b];
			A[e[i].b][e[i].a] = G[e[i].a][e[i].b];
			set_union(get_nth(f, e[i].a), get_nth(f, e[i].b));
		}
	}

	end:
	if (f)
	{
		free_forest(f);
	}
	if (e)
	{
		free(e);
	}
	if (alloc_failure)
	{
		if (A)
		{
			for (int i = 0; i < length; i++)
			{
				if (A[i])
					free(A[i]);
			}
			free(A);
		}
		return 0;
	}
	return A;
}

void print_matr(int** v, int length)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			printf("%d ", v[i][j]);
		}
		printf("\n");
	}
}

int get_the_number_of_conex(int** m, int length)
{
	int alloc_failure = 0;
	int result, empty, conex_components;
	Forest* f;
	f = (Forest*)calloc(1, sizeof(Forest));
	if (f == NULL) 
	{
		alloc_failure = 1;
		goto end;
	}
	for (int i = 0; i < length; i++)
	{
		result = make_set(f, i);
		if (result == 0)
		{
			alloc_failure = 1;
			goto end;
		}
	}
	for (int i = 0; i < length - 1; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (m[i][j] && find_set(get_nth(f, i)) != find_set(get_nth(f, j)))
			{
				set_union(get_nth(f, i), get_nth(f, j));
			}
		}
	}
	conex_components = 1;
	for (int i = 1; i < length; i++)
	{
		if (find_set(get_nth(f, 0)) != find_set(get_nth(f, i)))
		{
			set_union(get_nth(f, 0), get_nth(f, i));
			conex_components++;
		}
	}
end:
	if (f != NULL)
	{
		free_forest(f);
	}
	if (alloc_failure)
	{
		return -1;
	}
	return conex_components;
}

int** random_conex_graph_generator(int length)
{
	int alloc_failure = 0;
	int edge_count;
	
	int * conected_nodes = new int[length];
	memset(conected_nodes, 0, length * sizeof(int));
	int** v = (int**)calloc(length, sizeof(int*));
	if (v == NULL)
	{
		alloc_failure = 1;
		goto end;
	}
	for (int i = 0; i < length; i++)
	{
		v[i] = (int*)calloc(length, sizeof(int));
		if (v[i] == 0)
		{
			alloc_failure = 1;
			goto end;
		}
	}
	while (1)
	{
		int i = rand() % length;
		int j = rand() % length;
		if (i != j)
		{
			v[i][j] = rand();
			v[j][i] = v[i][j];
			conected_nodes[i] = 1;
			conected_nodes[j] = 1;
			break;
		}
	}
	edge_count = 1;
	while (edge_count < length - 1)
	{
		int i = rand() % length;
		if (conected_nodes[i] != 0)
		{
			continue;
		}
		int j;
		do
		{
			j = rand() % length;
		} while (!conected_nodes[j]);
		v[i][j] = rand();
		v[j][i] = v[i][j];
		conected_nodes[i] = 1;
		edge_count++;
	}
	while (edge_count < 4 * length)
	{
		int i = rand() % length;
		int j = rand() % length;
		if (i == j || v[i][j] != 0)
		{
			continue;
		}
		v[i][j] = rand();
		v[j][i] = v[i][j];
		edge_count++;
	}
end:
	if (alloc_failure)
	{
		if (v)
		{
			for (int i = 0; i < length; i++)
			{
				if (v[i])
				{
					free(v[i]);
				}
			}
			free(v);
		}
		return 0;
	}
	return v;
}

void demo()
{
	Forest* f = (Forest*)calloc(1,sizeof(Forest));
	if (f == NULL)
	{
		return;
	}
		
	for (int i = 0; i < 10; i++)
	{
		make_set(f, i + 1);
	}
	print_sets(f);
	set_union(get_nth(f, 0), get_nth(f, 1));
	print_sets(f);
	set_union(get_nth(f, 2), get_nth(f, 3));
	print_sets(f);
	set_union(get_nth(f, 4), get_nth(f, 5));
	print_sets(f);
	set_union(get_nth(f, 5), get_nth(f, 6));
	print_sets(f);
	set_union(get_nth(f, 8), get_nth(f, 9));
	print_sets(f);

	printf("\nElementul %d face parte din setul %d\n", 1, find_set(get_nth(f, 0))->key);
	printf("Elementul %d face parte din setul %d\n", 3, find_set(get_nth(f, 2))->key);
	printf("Elementul %d face parte din setul %d\n", 5 ,find_set(get_nth(f, 4))->key);
	printf("Elementul %d face parte din setul %d\n", 7, find_set(get_nth(f, 6))->key);
	printf("Elementul %d face parte din setul %d\n", 9, find_set(get_nth(f, 8))->key);

	int** v = (int**)calloc(5, sizeof(int*));
	if (v == NULL)
		return;
	for (int i = 0; i < 5; i++)
	{
		v[i] = (int*)calloc(5, sizeof(int));
		if (v[i] == NULL)
			return;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			v[i][j] = 0;
		}
	}
	v[0][1] = v[1][0] = 1;
	v[1][2] = v[2][1] = 2;
	v[2][3] = v[3][2] = 3;
	v[3][4] = v[4][3] = 4;
	v[0][2] = v[2][0] = 5;
	v[4][0] = v[0][4] = 6;

	printf("\nMatricea inainte de Kruskal:\n");
	print_matr((int**)v, 5);
	printf("\n");

	int** a = kruskal(v, 5);
	printf("\nMatricea dupa Kruskal:\n");
	print_matr(a, 5);
	printf("\nComponente conexe:%d\n", get_the_number_of_conex(a, 5));
}




void perf_all()
{
	for (int test = 0; test < NR_TESTS; test++)
	{
		for (n = STEP_SIZE; n <=9001; n += STEP_SIZE)
		{
			int** v = random_conex_graph_generator(n);
			if (v == NULL)
			{
				return;
			}
			comp_u = comp_fs = comp_k = comp_ms = 0;
			atr_u = atr_fs = atr_k = atr_ms = 0;
			int** g = kruskal(v, n);
			for (int i = 0; i < n; i++)
			{
				free(g[i]);
			}
			free(g);
			printf("Test:%d N: %d reusit.\n", test, n);
			fflush(stdout);
			Operation op_atr_u = p.createOperation("Union atribuiri", n);
			Operation op_atr_fs = p.createOperation("Findset atribuiri", n);
			Operation op_atr_k = p.createOperation("Kruskal atribuiri", n);
			Operation op_atr_ms = p.createOperation("Makeset atribuiri", n);
			Operation op_comp_u = p.createOperation("Union comparatii", n);
			Operation op_comp_fs = p.createOperation("Findset comparatii", n);
			Operation op_comp_k = p.createOperation("Kruskal comparatii", n);
			op_atr_u.count(atr_u);
			op_atr_fs.count(atr_fs);
			op_atr_ms.count(atr_ms);
			op_atr_k.count(atr_ms + atr_fs + atr_u);
			op_comp_fs.count(comp_fs);
			op_comp_u.count(comp_u);
			op_comp_k.count(comp_fs + comp_u);
			for (int i = 0; i < n; i++)
			{
				free(v[i]);
			}
			free(v);
		}
	}
	p.divideValues("Union atribuiri", NR_TESTS*2);
	p.divideValues("Findset atribuiri", NR_TESTS*2);
	p.divideValues("Kruskal atribuiri", NR_TESTS*2);
	p.divideValues("Makeset atribuiri", NR_TESTS*2);
	p.divideValues("Union comparatii", NR_TESTS*2);
	p.divideValues("Findset comparatii", NR_TESTS*2);
	p.divideValues("Kruskal comparatii", NR_TESTS*2);
	p.showReport();
}
int main()
{
	//demo();
	perf_all();
	return 0;
}