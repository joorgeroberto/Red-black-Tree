#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//contador que indicara a quantidade de recuos
int contador = 0;

typedef struct reg {
    char  conteudo; // conteúdo
    struct reg *esq;
	struct reg *dir;
	struct reg *pai;
	char cor;//Pode ser B (black) ou R (red)
} noh; // nó



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
			printf("%c\n", r->conteudo);
		else
			printf("%c<*>\n", r->conteudo);
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
	noM->conteudo = 'M';
	noM->esq = NULL;	
	noM->dir = NULL;
	noM->pai = NULL;
	noM->cor = 'B';

	noh *noJ = malloc(sizeof(noh));
	noJ->conteudo = 'J';
	noJ->esq = NULL;	
	noJ->dir = NULL;
	noJ->pai = NULL;
	noJ->cor = 'B';

	noh *noR = malloc(sizeof(noh));
	noR->conteudo = 'R';
	noR->esq = NULL;	
	noR->dir = NULL;
	noR->pai = NULL;
	noR->cor = 'B';

	noh *noE = malloc(sizeof(noh));
	noE->conteudo = 'E';
	noE->esq = NULL;	
	noE->dir = NULL;
	noE->pai = NULL;
	noE->cor = 'B';

	//Inserindo os nohs na arvore
	noM->esq = noJ;
	noM->dir = noR;

	noJ->esq = noE;
	noJ->dir = NULL;
	noJ->esq->cor = 'R';

	mostrar(noM);
	
}
