/*
* Flore Daniel-Andrei, gr 30229
* ------CERINTA------
* Transformarea din reprezentarea parinte -> reprezentarea arbore multicai -> reprezentare binara + 3x prettyPrints
* 
* ------EXPLICATII------
* ###prettyPrint
*	-se va efectua o spatiere fata de inceputul randului in functie de nivelul actual atins
*	-se va printa nodul actual considerat radacina a unui subarbore
*	-va lua radacina, dupa va lua pe rand fiecare copil al radacinii considerate si ii va cauta la randul sau copiii, odata ce toti copiii au fost descoperiti
* se va intoarce la urmatorul nod copil anterior ( fratele radacinii subarborelui actual )
* 
* ###transformarea1 (reprezentarea parinte -> reprezentarea arbore multicai)
*	-inverseaza modul de pointare (din copil-> parinte in parinte->copil)
*	-foloseste un vector temporar pt copii, initializat cu null
*	-pentru fiecare indice al vectorului (nod), foloseste un help function care creeaza mai intai nodul cu cheia i, verifica daca valoarea elementului de pe pozitia i este
* egala cu -1, caz in care am descoperit radacina, altfel va relua recursiv procedura
*	-pentru fiecare nod, i se vor adauga copii intr-un vector de copii, realizand astfel structura specifieca a unui arbore multicai: fiecare nod va contine o cheie si un vector cu pointeri spre nodurile copil
* 
* ###transformarea2 (reprezentarea arbore multicai -> reprezentare binara)
*	-vom folosi un nod secondRoot de tip R3 in care vom pastra arborele modificat R3
*	-in reprezentarea binara, fiecare nod contine cheia si un pointer spre primul copil si un al doilea catre fratele din dreapta
*	-cat timp radacina de tip R2 mai are copii, secondRoot-> child va primi rezultatul recursiv al transofrmarii efectuate pe copil de root R2 (fiecare nod va primi un pointer spre primul sau copil)
*	-dupa gasirea copiilor, se va cauta fratele din dreapta a fiecarui nod, in while-ul respectiv
*/


#include <stdio.h>
#include <malloc.h>

#define COUNT 5

typedef struct NodeR2 {
	int key;
	int nrChildren;
	struct NodeR2** children;
}NodeT;

typedef struct NodeR3 {
	int key;
	struct NodeR3* child;
	struct NodeR3* brother;
}NodeS;

void prettyPrintR1(int pi[], int length, int space, int root)
{
	space += COUNT;
	for (int i = 0; i < space; i++)
	{
		printf(" ");
	}
	printf("%d\n", root);
	for (int i = 1; i < length; i++)
	{
		if (pi[i] == root)
		{
			prettyPrintR1(pi, length, space + 1, i);
		}
	}
}


//transformarea 1
NodeR2* createNodeT1(int key)
{
	NodeR2* p = (NodeR2*)malloc(sizeof(NodeR2));
	p->children = (NodeR2**)malloc(5 * sizeof(NodeR2*)); 
	for (int i = 0; i < 5; i++)
	{
		p->children[i] = NULL;
	}
	p->key = key;
	p->nrChildren = 0;
	return p;
	free(p);
}

void help_function_tr1(int i, NodeR2** root, NodeR2** ok, int pi[])
{
	if (ok[i] != NULL)
		return;
	ok[i] = createNodeT1(i);
	if (pi[i] == -1)
	{
		*root = ok[i];
		return;
	}
	if (ok[pi[i]] == NULL)
		help_function_tr1(pi[i], root, ok, pi);
	NodeR2* p = ok[pi[i]];
	int index = 0;
	while (p->children[index] != NULL)
		index++;
	p->nrChildren++;
	p->children[index] = ok[i];
}

NodeR2* transformR1toR2(int length, int pi[])
{
	NodeR2** ok = (NodeR2**)malloc(length * sizeof(NodeR2*));
	for (int i = 1; i < length; i++)
		ok[i] = NULL;
	NodeR2* root = NULL;
	for (int i = 1; i < length; i++)
		help_function_tr1(i, &root, ok, pi);
	return root;
	free(ok);
}

void prettyPrintR2(NodeR2* root, int length, int space)
{
	space += COUNT;
	for (int i = 0; i < space; i++)
	{
		printf(" ");
	}

	printf("%d\n", root->key);
	for (int i = 0; i < root->nrChildren; i++)
		prettyPrintR2(root->children[i], length + 1, space+1);
}

//transformarea 2
NodeR3* createNodeT2(int key)
{
	NodeR3* p = (NodeR3*)malloc(sizeof(NodeR3));
	p->key = key;
	p->brother = NULL;
	p->child = NULL;
	return p;
	free(p);
}

NodeR3* transformR2toR3(NodeR2* root)
{
	NodeR3* secondRoot = createNodeT2(root->key);
	if (root->children[0] != NULL)
	{
		secondRoot->child = transformR2toR3(root->children[0]);
		int index = 1;
		NodeR3* temp = secondRoot->child;
		while (root->children[index] != NULL && index < 5)
		{
			temp->brother = transformR2toR3(root->children[index]);
			temp = temp->brother;
			index++;
		}
	}
	return secondRoot;
}

void prettyPrintR3(NodeR3* root, int space)
{
	if (root == NULL)
		return;
	
	space += COUNT;
	for (int i = 0; i < space; i++)
		printf(" ");
	printf("%d\n", root->key);
	prettyPrintR3(root->child, space + 1);
	prettyPrintR3(root->brother, space-5);

}
int main()
{
	int pi[] = { 0,2,7,5,2,7,7,-1,5,2 };
	int length = sizeof(pi) / sizeof(pi[1]);
	printf("Sirul initial: ");
	for (int i = 1; i < length; i++)
	{
		printf("%d ", pi[i]);
	}
	printf("\nReprezentarea pentru R1:\n");
	prettyPrintR1(pi, length, 0, 7);

	NodeR2* probaT1 = transformR1toR2(length, pi);
	printf("\nReprezentarea pentru R2:\n");
	prettyPrintR2(probaT1, 0,0);

	NodeR3* probaT2 = transformR2toR3(probaT1);
	printf("\nReprezentarea pentru R3:\n");
	prettyPrintR3(probaT2, 0);
}