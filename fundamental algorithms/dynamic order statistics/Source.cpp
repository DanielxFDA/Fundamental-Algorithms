/*
* Flore Daniel-Andrei
* gr. 30229
*  *****CERINTA*****
* implementare BUILDPBT, OSSELECT, OSDELETE si calcul atribuiri, comparatii in total
*  *****EXPLICATII*****
*  -functiile au fost implementate recursiv, mergand din aproape in aproape pe ramurile arborelui 
*  -sirul initial este sortat, astfel, luam recursiv mijlocul acestuia si il introducem in pbt, dupa continuam pasii pt fiecare jumatate a sirului impartit in doua
*  -pt os select se merge pe ramuri recursiv
*  ******OBSERVATII*****
*  -din grafice, observam ca in total se efectueaza O(n) operatii pentru build si delete, respectiv O(ln(n)) pt select
* 
*/

#include <stdio.h>
#include <time.h>
#include "Profiler.h"

#define COUNT 10
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TEST 5
Profiler p("proft8");
int atr;
int comp;
int n;

typedef struct Node {
	int key;
	int size;
	struct Node* left;
	struct Node* right;
}Node;

Node* createNewNode(int key)
{
	Node* nod = (Node*)malloc(sizeof(Node));
	nod->key = key;
	nod->left = NULL;
	nod->right = NULL;
	nod->size = 0;
	return nod;
}

Node* buildPBT(int v[], int low, int high)
{
	if (low > high)
		return NULL;
	int mid = abs((low + high) / 2);
	Node* nod = createNewNode(v[mid]);
	if (nod == NULL)
		return NULL;
	nod->size = high+1;
	nod->left = buildPBT(v, low, mid-1);
	nod->right = buildPBT(v, mid + 1, high);
	return nod;
}

void prettyPrint(Node* p, int space)
{
	if (p == NULL)
		return;
	prettyPrint(p->right, space + 1);
	for (int i = 0; i < space; i++)
		printf("\t");
	printf("%d\n", p->key);
	prettyPrint(p->left, space + 1);
}


int sizeValue(Node* root)
{
	if (root == NULL)
	{
		return 0;
	}
	root->size = sizeValue(root->left) + sizeValue(root->right) + 1;
	return root->size;
}

Node* osSelect(Node* root, int ithsmallest)
{
	int r; 
	if (root == NULL)
		return NULL;
	if (root->left == NULL)
		r = 1;
	else 
		r = root->left->size +1;	
	comp += 1;
	if (ithsmallest == r)
		return root;
	comp += 1;
	if (ithsmallest < r)
		return osSelect(root->left, ithsmallest);
	else
		return osSelect(root->right, ithsmallest - r);
}


Node* findMin(Node* root)
{
	if (root == NULL)
		return NULL;
	if (root->left == NULL)
		return root;
	return findMin(root->left);
}
Node* osDelete(Node* root, int ithsmallest)
{
	Node* p;
	if (root == NULL)
		return NULL;
	comp += 1;
	if (ithsmallest < root->key) {
		root->left = osDelete(root->left, ithsmallest);
		atr += 1;
	}
		
	else if ( ithsmallest > root->key) {
		comp += 1;
		root->right = osDelete(root->right, ithsmallest);
		atr += 1;
	}
		
	else
	{
		comp += 1;
		if (root->left == NULL && root->right == NULL)
		{
			return NULL;
		}
		else if ( root->left == NULL)
		{
			comp += 1;
			p = root->right;
			free(root);
			return p;
			atr += 1;
		}
		else if (root->right == NULL)
		{
			p = root->left;
			free(root);
			return p;
			atr += 1;
		}
		p = findMin(root->right);
		root->key = p->key;
		root->right = osDelete(root->right, p->key);
		atr += 2;
	}
	return root;
}

void perf()
{
	
	Node* root;
	for (int test = 0; test < NR_TEST; test++)
	{
		for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
		{
			atr = 0; comp = 0;
			int sir[MAX_SIZE];
			for (int i = 0; i < n; i++)
			{
				sir[i] = i + 1;
			}
			root = buildPBT(sir, 0, n-1);
			for (int i = 0; i < n; i++)
			{
				int index = rand() % (n-i)+1;
				osSelect(root, index);
				osDelete(root, index);
			}
			Operation opAttr = p.createOperation("OS_S_D_Attr", n);
			Operation opComp = p.createOperation("OS_S_D_Comp", n);
			opAttr.count(atr);
			opComp.count(comp);
		}
	}
	
	p.divideValues("OS_S_D_Attr", NR_TEST);
	p.divideValues("OS_S_D_Comp", NR_TEST);
	p.addSeries("OS_S_D_Total", "OS_S_D_Attr", "OS_S_D_Comp");
	p.showReport();
}

void demo()
{
/*
	****ALGORITM GENERAL****
	int a[] = {1,4,5,9,14,17,21,25 };
	Node* root;
	root = buildPBT(a, 0, 7);
	printf("Arborele inainte de operatii:\n");
	prettyPrint(root, 0);
*/
	int n = 11;
	int v[11];
	for (int i = 0; i < n; i++)
		v[i] = i + 1;
	Node* root = buildPBT(v, 0, 10);
	printf("Arborele inainte de operatii:\n");
	prettyPrint(root, 0);
	
	//verificare osSelect
	Node* proba1 = osSelect(root, 3);
	printf("\n Nodul cu a 3-a cea mai mica cheie are valoarea = %d.", proba1->key);
	Node* proba2 = osSelect(root, 5);
	printf("\n Nodul cu a 5-a cea mai mica cheie are valoarea = %d.", proba2->key);
	Node* proba3 = osSelect(root, 8);
	printf("\n Nodul cu a 8-a cea mai mica cheie are valoarea = %d.", proba3->key);
	
	//verificare osDelete
	printf("\nArborele dupa stergerea nodului cu cheia 3:\n");
	osDelete(root, 3);
	prettyPrint(root, 0);
	sizeValue(root);
	printf("\nArborele dupa stergerea nodului cu cheia 6:\n");
	osDelete(root, 6);
	sizeValue(root);
	prettyPrint(root, 0);
	printf("\nArborele dupa stergerea nodului cu cheia 2:\n");
	osDelete(root, 2);
	sizeValue(root);
	prettyPrint(root, 0);
}

int main()
{
	//srand(time(0));
	//demo();
	perf();
	return 0;
}