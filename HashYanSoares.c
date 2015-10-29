#include<stdio.h>
#include<stdlib.h>

//struct dos elementos
typedef struct{
	int valor;
	struct estruturaElemento *proximo;
}elemento;

//struct do vetor de controle (estrutura auxiliar que irá conter o fator de carga e o número de elementos de cada conjunto)
typedef struct{
	int numeroCelulas;
	int numeroElementos;
	int fatorCarga;
}controle;

controle vetorControle[50];

//funções específicas do trabalho
int criar();
int inserir(int element, int conjunto);
int existe(int element, int conjunto);
int excluir(int element, int conjunto);
int unir(int conjunto1, int conjunto2); //retorna identificador do novo conjunto
void listar(int conjunto);
void fim();

//variáveis globais
int contadorConjuntos = 0; //contador que será responsável por retornar o índice do conjunto que está sendo criado
elemento* conjuntos[50]; //vetor estático de ponteiros para os conjuntos (cada conjunto é alocado dinâmicamente na sua criação)

//funções extras
void inicializa(); //inicializa o vetor de conjuntos, fazendo suas células apontarem para NULL (a cada vez que é criado um conjunto, a célula referente a ID do conjunto irá apontar para seu primeiro elemento)
void menu();
//int hash(int elemento);

int main(){
	inicializa();
	menu();
	return 0;
}

void menu(){
	int escolha, element, conjunto, id, retorno, conjunto1, conjunto2;
	while(escolha != 7){
		printf("############################################\n");
		printf("\t1 - Criar novo conjunto\n");
		printf("\t2 - Inserir novo elemento em conjunto\n");
		printf("\t3 - Conferir se existe elemento em conjunto\n");
		printf("\t4 - Excluir elemento de conjunto\n");
		printf("\t5 - Unir conjuntos\n");
		printf("\t6 - Listar conjunto\n");
		printf("\t7 - Encerrar programa\n");
		printf("############################################\n");
		printf("\nInsira sua opcao: ");
		scanf("%d", &escolha);
		switch(escolha){
			case 1:
				id = criar();
				printf("Conjunto %d criado!\n\n", id);
				break;
			case 2:
				printf("Qual elemento a ser inserido? ");
				scanf("%d", &element);
				printf("\nEm qual conjunto? ");
				scanf("%d", &conjunto);
				retorno = inserir(element, conjunto);
				if(retorno == 1) printf("Sucesso!\n");
				if(retorno == -1) printf("Erro!\n");
				break;
			case 3:
				printf("Qual elemento a ser conferido? ");
				scanf("%d", &element);
				printf("\nEm qual conjunto? ");
				scanf("%d", &conjunto);
				retorno = existe(element, conjunto);
				break;
			case 4:
				printf("Qual elemento a ser removido? ");
				scanf("%d", &element);
				printf("\nEm qual conjunto? ");
				scanf("%d", &conjunto);
				retorno = excluir(element, conjunto);
				break;
			case 5:
				printf("Quais conjuntos a serem unidos? ");
				scanf("%d", &conjunto1);
				scanf("%d", &conjunto2);
				retorno = unir(conjunto1, conjunto2);
				break;
			case 6:
				printf("Qual conjunto a ser listado? ");
				scanf("%d", &conjunto);
				listar(conjunto);
				break;
			case 7:
				break;
		}
	}
}

void inicializa(){
	int i;
	for(i=0; i<50; i++){
		conjuntos[i] = NULL;
		vetorControle[i].numeroCelulas = 0;
		vetorControle[i].numeroElementos = 0;
		vetorControle[i].fatorCarga = 0;
	}
}

int criar(){ //aloca dinâmicamente cada conjunto (que tem tamanho inicial 50) - cada conjunto é um vetor de ponteiros para listas encadeadas que serão os elementos
	int i;
	elemento* conjunto; //conjunto de elementos que será alocado dinâmicamente - em primeira instância, o tamanho será 50 elementos
	for(i=0; i<50; i++){
		conjunto = malloc(sizeof(elemento));
		conjunto[i].proximo = NULL; //inicializa todas as células apontando para NULL (sem elementos inseridos) - o elemento que está dentro de cada célula NÃO tem valor (funciona como a "cabeça" da lista encadeada, apenas apontando para o primeiro elemento)
	}
	conjuntos[contadorConjuntos] = &conjunto[0]; //faz o vetor de conjuntos apontar para a primeira célula desse conjunto
	vetorControle[contadorConjuntos].numeroCelulas = 50;
	vetorControle[contadorConjuntos].numeroElementos = 0;
	vetorControle[contadorConjuntos].fatorCarga = 0;
	contadorConjuntos++;
	return contadorConjuntos-1;
}

int inserir(int element, int set){
	return 1;
}

int existe(int element, int set){
	return 1;
}

int excluir(int element, int set){
	return 1;
}

int unir(int conjunto1, int conjunto2){
	return 1;
}

void listar(int set){
}

void fim(){
}
