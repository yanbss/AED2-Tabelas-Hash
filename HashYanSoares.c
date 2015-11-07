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
	float fatorCarga;
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
int hash(int numero, int ncelulas); //função hash
int compareInt(const void * a, const void * b); //função auxiliar usada pelo qsort(), função incluida na biblioteca stdlib que opera o quicksort em cima do vetor4
void rehash(int set); //função responsável pelo rehash, quando atinge o fator de carga máximo
void eliminaTodos(int set); //função que elimina todos os elementos de um conjunto, para facilitar a implementação do fim()

int main(){
	inicializa();
	return 0;
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
		conjunto = (elemento*) malloc(50*sizeof(elemento)); //aloca o vetor de 50 elementos (pode mudar após rehash)
		conjuntos[contadorConjuntos] = conjunto; //faz o vetor de conjuntos apontar para a primeira célula desse conjunto
		for(i=0; i<50; i++){
			(conjunto+i)->proximo = NULL; //inicializa todas as células apontando para NULL (sem elementos inseridos) - o elemento que está dentro de cada célula NÃO tem valor (funciona como a "cabeça" da lista encadeada, apenas apontando para o primeiro elemento)
		}
		vetorControle[contadorConjuntos].numeroCelulas = 50; //atualiza os valores de controle do conjunto
		vetorControle[contadorConjuntos].numeroElementos = 0;
		vetorControle[contadorConjuntos].fatorCarga = 0;
		contadorConjuntos++;
		return contadorConjuntos-1; //numero referente a ID do conjunto que acaba de ser criado
	}
	else return -1;
}

int hash(int numero, int ncelulas){ //retorna o mod (função hash) do elemento a ser inserido
	return abs(numero%ncelulas);
}

int inserir(int element, int set){
	if(existe(element, set) == 1) return 1; //não deve conter elementos repetidos e, ao fazer isso, não deve acusar erro (então retorna 1)
	
	if(set < contadorConjuntos){ //certifica que está tentando inserir num conjunto que já existe
		int index, ncelulas;
		elemento *ponteiro; //ponteiro que irá apontar para a célula correta referente ao indice gerado pela função hash (dela sai uma lista encadeada com os valores)
		elemento *novo; //novo elemento que será inserido
		novo = malloc(sizeof(elemento));
		novo->valor = element;
		novo->proximo = NULL;
		ncelulas = vetorControle[set].numeroCelulas;
		index = hash(element, ncelulas);
		ponteiro = conjuntos[set] + index;
		if(ponteiro->proximo == NULL) ponteiro->proximo = novo; //inserção de primeiro elemento da célula [index]
		else{
			novo->proximo = ponteiro->proximo; //insere na primeira posição
			ponteiro->proximo = novo; //rearranja os ponteiros para que o que estava na primeira posição, passe para a próxima
		}
		vetorControle[set].numeroElementos++;
		vetorControle[set].fatorCarga = (float) vetorControle[set].numeroElementos / vetorControle[set].numeroCelulas;
		if(vetorControle[set].fatorCarga >= 0.7){
			rehash(set);
		}
		return 1;
	}
	
	else return -1; //caso o conjunto que se deseja inserir não exista
}

int existe(int element, int set){
	if(set < contadorConjuntos){ //certifica que o conjunto a ser procurado já existe
		int index, ncelulas;
		elemento *ponteiro;
		ncelulas = vetorControle[set].numeroCelulas;
		index = hash(element, ncelulas);
		ponteiro = conjuntos[set] + index;
		if (ponteiro->proximo == NULL) return -1; //não existe nenhum valor nessa lista
		else{
			while(ponteiro->proximo != NULL){ //percorre a lista
				ponteiro = ponteiro->proximo;
				if(ponteiro->valor == element) return 1; //se achar o valor, retorna 1
			}
			return -1; //se percorrer toda lista e não achar o valor, retorna -1
		}
	}
	else return -1; //conjunto a ser procurado não existe
}

int excluir(int element, int set){
	if(set < contadorConjuntos){ //certifica que o conjunto do qual será eliminado o elemento existe
		if(existe(element, set) == -1) return -1; //caso esteja tentando eliminar um elemento que não existe no conjunto
		else{ //caso realmente exista no conjunto:
			int index, ncelulas;
			elemento *ponteiro, *anterior;
			ncelulas = vetorControle[set].numeroCelulas;
			index = hash(element, ncelulas);
			anterior = conjuntos[set] + index;
			ponteiro = anterior->proximo;
			while(ponteiro->valor != element){
				ponteiro = ponteiro->proximo;
				anterior = anterior->proximo;
			}
			anterior->proximo = ponteiro->proximo;
			free(ponteiro);
			vetorControle[set].numeroElementos--;
			return 1;
		}
	}
	else return 1; //se o conjunto não existir, não deve retornar erro
}

int unir(int conjunto1, int conjunto2){ //no unir há a necessidade de criar um novo conjunto, armazenar os elementos dos 2 conjuntos em um vetor e inserir eles no novo conjunto
	elemento *ponteiro;
	int *vetor;
	int nelementos, cont=0, i, idconjunto;
	
	//calculo do numero de elementos
	
	nelementos = vetorControle[conjunto1].numeroElementos + vetorControle[conjunto2].numeroElementos;
	
	//criação do novo conjunto
	
	idconjunto = criar();
	
	//criação do vetor dinamicamente alocado e inserção de todos os elementos dos dois conjuntos nele
	vetor = (int*) malloc (nelementos * sizeof(int)); //aloca o espaço necessário para inserir todos os elementos dos 2 conjuntos no vetor
	for(i = 0; i < vetorControle[conjunto1].numeroCelulas; i++){ //insere no vetor os elementos do primeiro conjunto
		ponteiro = conjuntos[conjunto1] + i;
		while(ponteiro->proximo != NULL){
			ponteiro = ponteiro->proximo;
			vetor[cont] = (ponteiro->valor);
			cont++;
		}
	}
	for(i = 0; i < vetorControle[conjunto2].numeroCelulas; i++){ //insere no vetor os elementos do segundo conjunto
		ponteiro = conjuntos[conjunto2] + i;
		while(ponteiro->proximo != NULL){
			ponteiro = ponteiro->proximo;
			vetor[cont] = (ponteiro->valor);
			cont++;
		}
	}
	
	//inserção dos elementos no novo conjunto
	for(i=0; i<cont; i++){ //insere cada um dos elementos no novo conjunto
		inserir(vetor[i], idconjunto);
	}
	return idconjunto; //retorna o id do novo conjunto criado
}

void listar(int set){
	if(vetorControle[set].numeroElementos == 0) return;
	int i, cont=0, nelementos;
	elemento* ponteiro;
	int *vetor; //cria um vetor de inteiros alocado dinâmicamente com o número de elementos do conjunto (será passado para uma função de ordenação)
	nelementos = vetorControle[set].numeroElementos;
	vetor = (int*) malloc(nelementos * sizeof(int));
	for(i = 0; i < vetorControle[set].numeroCelulas; i++){
		ponteiro = conjuntos[set] + i;
		while(ponteiro->proximo != NULL){
			ponteiro = ponteiro->proximo;
			vetor[cont] = (ponteiro->valor);
			cont++;
		}
	}
	qsort(vetor, nelementos, sizeof(int), compareInt);
	for(i = 0; i < nelementos-1; i++){ //vai até o penúltimo elemento para que o último possa ser impresso sem a vírgula no final
		printf("%d,", vetor[i]);
	}
	printf("%d\n", vetor[i]);
}

int compareInt(const void * a, const void * b){
	if( *(int*)a < *(int*)b ) return -1;
	if( *(int*)a == *(int*)b ) return 0;
	if( *(int*)a > *(int*)b ) return 1;
}

void fim(){
	int i, j;
	elemento *ponteiro;
	for(i=0; i < contadorConjuntos; i++){
		eliminaTodos(i); //desaloca todos os elementos que foram inseridos no conjunto
		for(j = (vetorControle[i].numeroCelulas-1); j >= 0; j--){ //desaloca todas as células do conjunto
			ponteiro = conjuntos[i] + j;
			free(ponteiro);
		}
	}
	exit(0);
}

void rehash(int set){ //vai dobrar o numero de celulas da tabela e irá inserir novamente todos os seus elementos
	elemento *ponteiro, *conjunto;
	int *vetor;
	int i, cont=0;
	
	//primeiramente, pega todos os elementos do conjunto e os armazena em um vetor

	vetor = (int*) malloc(vetorControle[set].numeroElementos * sizeof(int));
	for(i = 0; i < vetorControle[set].numeroCelulas; i++){ //insere no vetor os elementos do primeiro conjunto
		ponteiro = conjuntos[set] + i;
		while(ponteiro->proximo != NULL){
			ponteiro = ponteiro->proximo;
			vetor[cont] = (ponteiro->valor);
			cont++;
		}
	}
	
	//desaloca toda a tabela hash para criar a nova
	
	for(i = 0; i < cont; i++){
		excluir(vetor[i], set);
	}
	for(i = (vetorControle[set].numeroCelulas-1); i >= 0; i--){ //desaloca toda a tabela hash do último elemento até o primeiro (para não perder a referência do conjuntos[set] que é o primeiro elemento)
		ponteiro = conjuntos[set] + i;
		free(ponteiro);
	}
	
	//cria a nova tabela com o dobro de tamanho
	
	vetorControle[set].numeroCelulas = vetorControle[set].numeroCelulas * 2; //duplica o tamanho da tabela
	vetorControle[set].numeroElementos = 0;
	vetorControle[set].fatorCarga = 0;
	conjunto = (elemento*) malloc(vetorControle[set].numeroCelulas * sizeof(elemento));
	conjuntos[set] = conjunto; //faz o vetor de conjuntos apontar para a nova tabela, com o dobro de tamanho
	for(i=0; i<vetorControle[set].numeroCelulas; i++){
		(conjunto+i)->proximo = NULL; //inicializa a nova tabela hash fazendo todas as células apontarem para NULL
	}
	for(i=0; i<cont; i++){
		inserir(vetor[i], set);
	}
}

void eliminaTodos(int set){
	elemento *ponteiro;
	int *vetor;
	int i, cont=0;
	vetor = (int*) malloc(vetorControle[set].numeroElementos * sizeof(int)); //armazena todos os elementos em um vetor
	for(i = 0; i < vetorControle[set].numeroCelulas; i++){
		ponteiro = conjuntos[set] + i;
		while(ponteiro->proximo != NULL){
			ponteiro = ponteiro->proximo;
			vetor[cont] = (ponteiro->valor);
			cont++;
		}
	}
	for(i = 0; i < cont; i++){ //exclui todos os elementos
		excluir(vetor[i], set);
	}
}
