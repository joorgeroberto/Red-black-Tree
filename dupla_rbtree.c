/***************************************************/
/**  Projeto  Final                               **/
/**  Implementacao da arvore rubro negra          **/
/**  Arquivo:  dupla_rbtree.c                     **/
/**                                               **/
/**  Alunos:
	Gabriel Santos Menezes
	Matricula: 
	gabriel94.menezes@gmail.com 
	
	Jorge Roberto de Carvalho Junior
	Matricula: 
	jorgercj@dcomp.ufs.br 
**/
/**                   							  **/
/**                         					  **/
/***************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//variavel global
//Contador usado nas funcoes recuo e mostrar e indica a quantidade de recuos usados.
int contador = 0;

//Definindo o tipo booleano em c.
typedef enum { false, true } bool;

//Definicao do tipo noh.
typedef struct reg {
    char  key; //Chave.
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
// Devolve o noh com a maior chave da subárvore cuja raiz é x (ou NULL se a arvore estiver vazia).
noh* max(noh* x) {
	if(x == NULL)
		return NULL;
	if (x->dir == NULL) 
		return x;
    return max(x->dir);
}

// Devolve o nó que contém o piso (floor) de key na subárvore com raiz x.
// Devolve null se esse piso não existe.
noh* floor(noh* x, char key) {
	if (x == NULL) 
		return NULL;
	if (key == x->key) 
		return x;
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

//Verifica se o noh esta vazio.
int isEmpty(noh* r){
	if(r == NULL) 
		return 1;
	else 
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

//Insere um noh na arvore. 
//Se a arvore estiver vazia ele e retorna o noh como raiz.
//Senao vai comparando com os pais ate achar o local adequado, insere o noh e atualiza o valor do pai.
noh* insere(noh* h, char key, int val, noh* father) {
	if (h == NULL) { //insercao sempre como vermelha
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

	//propriedade das arvores, mantendo sempre a regra 
	//de deixar o menor valor para a esquerda e
	if(key < h->key) {
		h->esq = insere(h->esq, key, val, h);
	}
    else { 
    	//o maior para a direita
		if(key > h->key)
			h->dir = insere(h->dir, key, val, h);
 	    else 
			h->conteudo = val; //caso seja igual
							   //apenas atualiza o valor
	}
	
	//logica para a manutencao da arvore
	//similar a funcao balance, todavia o procedimento a seguir
	//se preocupa em mantar com a cor dos filhos da esq e da direita
	//para fazer a manutencao da arvore rubro-negra
	//no caso da balance isso nao eh necessario, visto que
	//ela considera o tratamento dessa funcao na parte da cor dos filhos da esquerda
	//se forem pretos
	if (isRed(h->dir) && !isRed(h->esq))      
		h = rotateLeft(h);
    if (isRed(h->esq)  &&  isRed(h->esq->esq)) 
		h = rotateRight(h);
    if (isRed(h->esq)  &&  isRed(h->dir))    
		flipColors(h);
    
    //atualiza o valor do tamanho da arvore
	h->N = size(h->esq) + size(h->dir) + 1;
	
	if(h->pai == NULL)//sempre a raiz total da arvore tem a cor preta.
		h->cor = 'B';
    return h;
}

// considera apenas a subárvore que tem raiz x
// mesma forma de pesquisa da arvore binaria de busca
noh* busca(noh * x, char key) {
    if(x == NULL) 
		return NULL;
	if(key < x->key) 
		return busca(x->esq, key);
    else 
		if(key > x->key) 
			return busca(x->dir, key);
		else 
			return x;
}

//funcao que adiciona o recuo à impressao do programa.
void recuo(){
	int i;
	for (i = 0;i < contador ; i++){
		printf("   ");	
	}
}


//imprime a arvore da forma raiz-esquerda-direita (ou red)
// colocando com recuo de margem proporcional à profundidade do noh.
void mostrar (noh * r) {
	if (r == NULL) 
		printf("-\n");
	if (r != NULL) {
		if(r->cor == 'B')
			printf("%c\n", r->key);
		else
			printf("%c<*>\n", r->key); //se a cor for vermelha, ha impressao do caractere
									   //para representar sua cor, se n tiver sua cor eh preta
		contador++;	  
		
		recuo();
		mostrar(r->esq);
	  	
		recuo();		
      	mostrar(r->dir);
		contador--; 
   }
}

//funcao para comparar dois caracteres passados, no caso
//sera uma forma de auxilar as funcoes futuras para utilizacao
//da chave da arvore e a chave passada como argumento da funcao
int compara(char a, char b){
	if(a > b) 
		return 1;
	if(a == b) 
		return 0;
	if(a < b) 
		return -1;
}

//assumindo que h eh vermelho e ambos h->esq e h->esq->esq
//sao pretos, faz h->esq ou um dos seus filhos vermelho.
noh* moveRedLeft(noh* h){ 
	flipColors(h);
	if (isRed(h->dir->esq))
	{
		h->dir = rotateRight(h->dir);
		h = rotateLeft(h);
		flipColors(h);
	}
	return h;
}

//restaura a caracteristica da arvore rubro negra
//considera a funcao inserir ja tratando a parte
//dos filhos da esquerda serem pretos
noh* balance(noh* h){
	if(isRed(h->dir)) //n pode ter filho direito vermelho
		h = rotateLeft(h);
	if(isRed(h->esq) && isRed(h->esq->esq))  //nao pode ter uma sequencia de vermelhos
		h = rotateRight(h);
	if(isRed(h->esq) && isRed(h->dir)) //altera a cor do noh
		flipColors(h);				//para permanecer arvore rubro-negra
	
	
	h->N = size(h->esq) + size(h->dir) + 1; //atualiza o numero de nos
	return h;
}

//funcao que deleta o noh com a menor chave
//servira para futuras funcoes
noh* deletarMin(noh* h){ 
	if(h->esq == NULL)  
		return NULL;
	if(isRed(h->esq) && !isRed(h->esq->esq)) 
		h = moveRedLeft(h);
	h->esq = deletarMin(h->esq);
	
	//restaura a arvore
	return balance(h);
}

//funcao principal para deletar o noh com a menor chave,
//utiliza a funcao acima ("deletarmin") e faz o seu tratamento
//para fazer com que a arvore gerada apos a exclusao seja rubro-negra
noh* deleteMin(noh* h){  
	if(!isRed(h->esq) && !isRed(h->dir)) 
		h->cor = 'R';					
	h = deletarMin(h);
	if(!isEmpty(h))    //caso n esteja vazia atualiza a cor da raiz
		h->cor = 'B';
	return h;
}

//remove a chave especificada e seu valor associado a tabela de simbolos
noh* deleta(noh* h, char chave){
	if(compara(chave,h->key) < 0){
		if(!isRed(h->esq) && !isRed(h->esq->esq))
			h = moveRedLeft(h);
		h->esq = deleta(h->esq,chave);
	}
	else{
		if(isRed(h->esq)) 
			h = rotateRight(h);
		if(compara(chave,h->key) == 0 && (h->dir == NULL)) 
			return NULL;
		if(!isRed(h->dir) && !isRed(h->dir->esq)) 
			h = moveRedLeft(h);
		if(compara(chave,h->key) == 0){
			noh* x = min(h->dir);
			h->key = x->key;
			h->conteudo = x->conteudo;
			h->dir = deleteMin(h->dir);
		}
		else h->dir = deleta(h->dir,chave);
	}
	//restaura a arvore
	return balance(h);
}

//funcao principal, fazendo a modificacao necessaria para a arvore gerada pela
// deleta ser uma arvore rubro-negra
noh* remover(noh* r, char chave){ 
	if(chave == NULL) //caso o parametro da chave n seja o especificado
		printf("Erro, argumento invalido");
	if(!isRed(r->esq) && !isRed(r->dir))  //dois filhos pretos podendo ter pai vermelho
		r->cor = 'R'; //tratamento necessario para a remocao, pois a cor eh atualizada logo a seguir
	r = deleta(r,chave); //usa a funcao anterior para a remocao do valor
	if(!isEmpty(r)) //tratamento necessario para continuar rubro-negra
		r->cor = 'B'; //fazendo a raiz ser preta
	return r;
}

//funcao "teto", diferente da funcao floor "piso"
//a menor chave da subarvore com raiz x com valor maior ou igual a chave passada
noh* ceiling_cham(noh* x, char key){
	if(x == NULL) 
		return NULL;
	int cmp = compara(key,x->key);
	if(cmp == 0) 
		return x;
	if(cmp > 0) 
		return ceiling_cham(x->dir,key);
	noh *t = ceiling_cham(x->esq,key);
	if(t != NULL) 
		return t;
	else 
		return x;
}

//funcao principal do ceiling.
//encontra o menor valor maior ou igual a key indicada.
//devolve null caso a raiz esteja vazia
char ceiling(noh *r, char key){ 
	if(isEmpty(r)) 
		printf("ERRO!");
	noh *x = ceiling_cham(r,key);
	if(x == NULL) 
		return NULL;
	else 
		return x->key;
}

//funcao para encontrar o maior numero inteiro entre os valores passados ("a" e "b")
int maximo(int a, int b){
	if(a >= b) 
		return a;
	if(a < b) 
		return b;
}

//funcao que devolve a altura da arvore, utilizando a funcao acima
//para auxiliar na recursao, devolve -1 caso esteja vazia, sendo esse o criterio de parada da recursao
int altura(noh* r){
	if(r == NULL) return -1;
	return 1 + maximo(altura(r->esq),altura(r->dir));
}

//funcao que devolve a altura negra da arvore, utilizando a funcao acima
//para auxiliar na recursao, devolve -1 caso esteja vazia, sendo esse o criterio de parada da recursao
int altura_negra(noh* r){
	if(r == NULL || r->cor == 'R') return -1; //se nao acrescentar, vai acumular ao passar pela cor vermelha
	if(r->cor == 'B') return 1 + maximo(altura_negra(r->esq),altura_negra(r->dir));
	//else return 0;
}

//retorna a quantidade de subarvores pertecentes ao noh com a chave passada
//que se encontra na arvore
int rank(noh *root, char chave){
	if(root == NULL) // retorna 0 se estiver vazia
		return 0;
	int cmp = compara(chave,root->key); //compara o valor das chaves
	if(cmp < 0) 
		return rank(root->esq,chave); //se a chave passada for menor, segue o percurso da esquerda
	else 
		if(cmp > 0) 
			return rank(root->dir,chave); //se a chave passada for maior, segue o percurso da direita
	else 
		return size(root->esq); //caso seja igual, devolve a altura da arvore
}

//retorna a profundidade do noh atraves da chave
//seguindo uma funcao recursiva para contar o numero de vezes que houve deslocamento
//ate chegar a chave passada
int profundidade(noh* r, char chave){
	if(r != NULL && r->key != chave){ //caso seja n esteja vazia, e a chave n for encontrada
		return profundidade(r->esq,chave) + 1; //continua a recursao pelo percurso da esquerda
		return profundidade(r->dir,chave) + 1; //e pelo percurso da direita
	}
	else{ 
		return 0; //caso a raiz seja vazia retorna 0, sendo esse o valor do caso base da funcao
	}
}

//retorna a profundidade do noh negro atraves da chave
//seguindo uma funcao recursiva para contar o numero de vezes que houve deslocamento
//ate chegar a chave passada
int profundidade_negra(noh* r, char chave){
	if(r != NULL && r->key != chave && r->cor == 'B'){ //caso seja n esteja vazia, e a chave n for encontrada
		return profundidade_negra(r->esq,chave) + 1; //continua a recursao pelo percurso da esquerda
		return profundidade_negra(r->dir,chave) + 1; //e pelo percurso da direita
	}
	else{ 
		return 0; //caso a raiz seja vazia retorna 0, sendo esse o valor do caso base da funcao
	}
}

int main(int argc, char** argv){

	printf("Implementacao da arvore rubro-negra\n");
	printf("Programa elaborado por: Gabriel e Jorge\n");
	printf("Para a disciplina de Estrutura de Dados 1\n");
	printf("Como projeto final da disciplina\n");
	//criando a arvore de teste para o exemplo, atraves do texto passado
	//"S E A R C H E X A M P L E"
	
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
	
	//atualiza o valor do pai de todas os nohs
	atualizaPai(NULL, root);
	printf("\n\n");
	printf("Arvore Rubro-Negra antes da remocao:\n");
	//mostra a raiz
	mostrar(root);

	printf("\n");
	//mostra o tamanho
	printf("Size: %d\n",size(root));
	
	//mostra o floor da arvore rubro-negra
	//fazendo o tratamento da chamada
	//caso nao possua a chave passada
	noh* novo = floor(root,'F');
	if(novo != NULL)
		printf("Piso: %c\n",novo->key);
	else
		printf("Não tem piso pois todas as chaves da BST são maiores que a chave inserida.\n");
	
	//noh auxiliar para uso da funcao min e max
	noh* aux; 
	
	aux = min(root); //recebe o menor valor da arovre 
	printf("Min: %c\t", aux->key); //com um espaçamento
	
	aux = max(root); //recebe o manior valor da arvore
	printf("Max: %c\n",aux->key); //e da a quebra de linha
	
	printf("\nArvore apos remocao:");
	//atualizando a arvore apos remocao de uma chave passada
	root = remover(root,'M'); 
	
	printf("\n\n");
	//mostra a arvore atualizada apos a remocao
	mostrar(root); 
	printf("Quantidade de nos: %d\n", root->N); //valor atualizado da quantidade de noh na arvore
	
	//mostra a quantidade de subarvores para o noh para a chave especificada
	printf("Quantidade de subarvores para %c: %d\n",'P',rank(root,'P'));
	
	//mostra a altura apos a remocao
	printf("Altura da arvore apos remocao: %d\n",altura(root));
	
	//mostra a altura apos a remocao
	printf("Altura negra da arvore apos remocao: %d\n",altura_negra(root));
	
	//mostra a profundidade do noh para a chave especificada
	printf("Profundidade do no com a chave %c: %d\n",'C',profundidade(root,'C'));
	
	//mostra a profundidade do noh para a chave especificada
	printf("Profundidade negra do no com a chave %c: %d\n",'C',profundidade_negra(root,'C'));
	
	return 0; //tudo funcionou como o esperado
}
