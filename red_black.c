#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//contador que indicara a quantidade de recuos
int contador = 0;

typedef enum { false, true } bool;

typedef struct reg {
    char  key; // chave
    struct reg *esq;
	struct reg *dir;
	struct reg *pai;
	char cor;//Pode ser B (black) ou R (red)
	int conteudo;
} noh; // nó

/*
private Value get(Node x, Key key) {
    // Considera apenas a subárvore que tem raiz x
    if(x == null) 
		return null;
    int cmp = key.compareTo(x.key);
    if(cmp < 0) 
		return get(x.left, key);
    else 
		if (cmp > 0) 
			return get(x.right, key);
		else 
			return x.val;
}
*/
/*
private boolean isRed(Node x) {
	if (x == null) return false;
	return x.color == RED;
}
*/
bool isRed(noh* x) {
	if (x == NULL) { 
		return false;
	}
	else {
		x->cor = 'R';
		return (x);
	}
}

noh* busca(noh * x, char key) {// Considera apenas a subárvore que tem raiz x
    if(x == NULL) 
		return NULL;
    //int cmp = key.compareTo(x.key);
	//int cmp = strcmp(key, x->key); 
    //if(cmp < 0)
	if(key < x->key) 
		return busca(x->esq, key);
    else 
		//if(cmp > 0)
		if(key > x->key) 
			return busca(x->dir, key);
		else 
			return x;
}
//Funcao que adiciona o recuo à impressao do programa.
void recuo(){
	for (int i = 0;i < contador ; i++){
		printf("   ");	
	}
}

//Imprime a arvore da forma raiz-esquerda-direita (ou red) e colocando com recuo de margem proporcional à profundidade do noh.
void mostrar (noh * r) {
	if (r == NULL) 
		printf("-\n");
	if (r != NULL) {
		if(r->cor == 'B')
			printf("%c\n", r->key);
		else
			printf("%c<*>\n", r->key);
		contador++;	  
		
		recuo();
		mostrar(r->esq);
	  	
		recuo();		
      	mostrar(r->dir);
		contador--; 
   }
}

void main(){
	//Criando os nohs
	noh *noM = malloc(sizeof(noh));
	noM->key = 'M';
	noM->esq = NULL;	
	noM->dir = NULL;
	noM->pai = NULL;
	noM->cor = 'B';

	noh *noJ = malloc(sizeof(noh));
	noJ->key = 'J';
	noJ->esq = NULL;	
	noJ->dir = NULL;
	noJ->pai = NULL;
	noJ->cor = 'B';

	noh *noR = malloc(sizeof(noh));
	noR->key = 'R';
	noR->esq = NULL;	
	noR->dir = NULL;
	noR->pai = NULL;
	noR->cor = 'B';

	noh *noE = malloc(sizeof(noh));
	noE->key = 'E';
	noE->esq = NULL;	
	noE->dir = NULL;
	noE->pai = NULL;
	noE->conteudo = 50;
	noE->cor = 'B';

	//Inserindo os nohs na arvore
	noM->esq = noJ;
	noM->dir = noR;

	noJ->esq = noE;
	noJ->dir = NULL;
	noJ->esq->cor = 'R';

	mostrar(noM);
	//noh* noBusca = busca(noM,'M'); 
	//printf("%c",noBusca->key);
}
