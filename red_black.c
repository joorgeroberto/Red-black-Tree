#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <math.h>

//contador usado nas funcoes recuo e mostrar e indica a quantidade de recuos usados.
int contador = 0;

//Definindo o tipo booleano em c.
typedef enum { false, true } bool;

//Definicao do tipo noh.
typedef struct reg {
    char  key; // chave.
    struct reg *esq;
	struct reg *dir;
	struct reg *pai;
	char cor;//Pode ser B (black) ou R (red).
	int conteudo;
	int N; //Campo usado na funcao size() e onde ficará registrado o número de nós de uma subárvore.
} noh; // nó

// Devolve o noh com a menor chave da subárvore cuja raiz é x (ou NULL se a arvore estiver vazia).
noh* min(noh* x) {
	if(x == NULL)
		return NULL;
	if (x->esq == NULL) 
		return x;
    return min(x->esq);
}

noh* max(noh* x) {
	if(x == NULL)
		return NULL;
	if (x->dir == NULL) 
		return x;
    return min(x->dir);
}

// Devolve o nó que contém o piso (floor) de key na subárvore com raiz x.
// Devolve null se esse piso não existe.
noh* floor(noh* x, char key) {
	if (x == NULL) 
		return NULL;
	//int cmp = key.compareTo(x.key);
	if (key == x->key) 
		return x;
	//if(cmp < 0)
	if(key < x->key)
		return floor(x->esq, key);

    noh* novo = floor(x->dir, key);
    if (novo != NULL) 
		return novo;
    else 
		return x;
}


//Atualiza o pai de todas as arvores.
void atualizaPai(noh* pai, noh* atual) {
		if(atual != NULL && pai != NULL) {
			if(pai->key != atual->pai->key)
				atual->pai = pai;
		}
		if(atual->esq != NULL)
			atualizaPai(atual, atual->esq);
		if(atual->dir != NULL)
			atualizaPai(atual, atual->dir);
}

//Verifica o campo N onde fica registrado o número de nós da subárvore cuja raiz é x. 
//Essa implementação é conhecida como ansiosa (eager). 
//Mas, toda operação de inserção precisa atualizar o campo N de todos os nós.
int size(noh* x){
	if(x != NULL)
		return (x->N);
	return 0;
}

//Verifica se um noh é vermelho ou não.
bool isRed(noh* x) {
	if (x == NULL) { 
		return false;
	}
	else {
		if(x->cor == 'R'){
			return true;
		}
		else {
			return false;
		}
	}
}

//Muda a cor dos filhos de um no h.
void flipColors(noh* h) {
	h->cor = 'R';
	h->esq->cor = 'B';
	h->dir->cor = 'B';
}

//Rotaciona uma sub-arvore para a direita e atualiza o valor do pai.
noh* rotateRight(noh* h) {
	noh* x = h->esq;
	h->esq = x->dir;
	x->dir = h;
	x->cor = h->cor;
	h->cor = 'R';
	x->N = h->N;
	h->N = size(h->esq) + size(h->dir) + 1;
	x->pai = h->pai;
	h->pai = x;
	return x;
}

//Rotaciona uma sub-arvore para a esquerda e atualiza o valor do pai.
noh* rotateLeft(noh* h) {
	noh* x = h->dir;
    h->dir = x->esq;
	x->esq = h;
	x->cor = h->cor;
	h->cor = 'R';
	x->N = h->N;
	h->N = size(h->esq) + size(h->dir) + 1;
	x->pai = h->pai;
	h->pai = x;
	return x;
}

noh* insere(noh* h, char key, int val, noh* father) {
	if (h == NULL) {
		noh *novo = malloc(sizeof(noh));
		novo->key = key;
		novo->esq = NULL;	
		novo->dir = NULL;
		novo->pai = father;
		novo->cor = 'R';
		novo->conteudo = val;
		novo->N = 1;
		return (novo);
	}

	//int cmp = key.compareTo(h.key);
	//if(cmp < 0)
	if(key < h->key) {
		h->esq = insere(h->esq, key, val, h);
	}
    else { 
		//if (cmp > 0) 
		if(key > h->key)
			h->dir = insere(h->dir, key, val, h);
 	    else 
			h->conteudo = val;
	}
	
    if (isRed(h->dir) && !isRed(h->esq))      
		h = rotateLeft(h);
    if (isRed(h->esq)  &&  isRed(h->esq->esq)) 
		h = rotateRight(h);
    if (isRed(h->esq)  &&  isRed(h->dir))    
		flipColors(h);
    h->N = size(h->esq) + size(h->dir) + 1;
	if(h->pai == NULL)//Se for a raiz total da arvore sua cor eh preta.
		h->cor = 'B';
    return h;
}

// Considera apenas a subárvore que tem raiz x
noh* busca(noh * x, char key) {
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
/*
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
*/
	noh* root = insere(NULL,'S',10, NULL);
	root = insere(root,'E',20, NULL);
	root = insere(root,'A',30, NULL);
	root = insere(root,'R',40, NULL);
	root = insere(root,'C',50, NULL);
	root = insere(root,'H',60, NULL);
	root = insere(root,'E',70, NULL);
	root = insere(root,'X',80, NULL);	
	root = insere(root,'A',90, NULL);
	root = insere(root,'M',100, NULL);	
	root = insere(root,'P',110, NULL);
	root = insere(root,'L',120, NULL);	
	root = insere(root,'E',130, NULL);
	
	atualizaPai(NULL, root);
	mostrar(root);

	printf("Size: %d\n",size(root));
	noh* novo = floor(root,'F');
	if(novo != NULL)
		printf("Piso: %c\n",novo->key);
	else
		printf("Não tem piso pois todas as chaves da BST são maiores que a chave inserida.\n");
	//noh* noBusca = busca(noM,'M'); 
	//printf("%c",noBusca->key);
}
