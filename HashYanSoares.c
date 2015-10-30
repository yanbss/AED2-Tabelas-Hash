#include<stdio.h>
#include<stdlib.h>

//struct dos elementos
struct estruturaElemento{
	int valor;
	struct estruturaElemento *proximo;
};

typedef struct estruturaElemento elemento;

//struct do vetor de controle (estrutura auxiliar que irá conter o fator de carga e o número de elementos de cada conjunto)
typedef struct{ //foi usado typedef pra facilitar criar diretamente um vetor desse tipo de estrutura
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
int hash(int numero, int ncelulas); //função hash

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
				if(id != -1) printf("Conjunto %d criado!\n\n", id);
				else printf("Impossivel criar novo conjunto!\n");
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

int criar(){ //aloca dinâmicamente cada conjunto (que tem tamanho inicial 50) - cada conjunto é um vetor de ponteiros para listas encadeadas que serão os elementos (com o elemento da própria célula sem valor, apenas apontando para o primeiro elemento)
	if(contadorConjuntos < 50){ //certifica que só podem ser criados 50 conjuntos
		int i;
		elemento* conjunto; //conjunto de elementos que será alocado dinâmicamente - em primeira instância, o tamanho será 50 elementos
		conjunto = malloc(50*sizeof(elemento));
		for(i=0; i<50; i++){
			conjunto[i].proximo = NULL; //inicializa todas as células apontando para NULL (sem elementos inseridos) - o elemento que está dentro de cada célula NÃO tem valor (funciona como a "cabeça" da lista encadeada, apenas apontando para o primeiro elemento)
		}
		conjuntos[contadorConjuntos] = &conjunto[0]; //faz o vetor de conjuntos apontar para a primeira célula desse conjunto
		vetorControle[contadorConjuntos].numeroCelulas = 50; //atualiza os valores de controle do conjunto
		vetorControle[contadorConjuntos].numeroElementos = 0;
		vetorControle[contadorConjuntos].fatorCarga = 0;
		contadorConjuntos++;
		return contadorConjuntos-1; //numero referente a ID do conjunto que acaba de ser criado
	}
	else return -1;
}

int hash(int numero, int ncelulas){ //retorna o mod (função hash) do elemento a ser inserido
	return numero%ncelulas;
}

int inserir(int element, int set){
	if(set < contadorConjuntos){ //certifica que está tentando inserir num conjunto que já existe
		int index, ncelulas;
		elemento *ponteiro; //ponteiro que irá apontar para a célula correta referente ao indice gerado pela função hash (dela sai uma lista encadeada com os valores)
		elemento *novo; //novo elemento que será inserido
		novo = malloc(sizeof(novo));
		novo->valor = element;
		novo->proximo = NULL;
		ncelulas = vetorControle[set].numeroCelulas;
		index = hash(element, ncelulas);
		ponteiro = conjuntos[set] + (index*sizeof(elemento));
		if(ponteiro->proximo == NULL) ponteiro->proximo = novo; //inserção de primeiro elemento da célula [index]
		else{
			while(ponteiro->proximo != NULL) ponteiro = ponteiro->proximo; //caso não seja o primeiro, percorre a lista e insere no final
			ponteiro->proximo = novo;
		}
		free(ponteiro);
		return 1;
	}
	else return -1; //caso o conjunto que se deseja inserir não exista
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
