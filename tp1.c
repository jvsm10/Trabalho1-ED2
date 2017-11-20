#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

typedef struct arvo
{
    char simb;
    unsigned freq;
    struct arvo *esq, *dir;
}arvcod;

typedef struct MinHeap
{
    unsigned tam;
    unsigned capacidade;
    arvcod **heap;
}mh;

//Alocar novo nó
arvcod* criano(char simb, unsigned freq)
{
    arvcod* temp = (arvcod*) malloc(sizeof(arvcod));
    temp->esq = temp->dir = NULL;
    temp->simb = simb;
    temp->freq = freq;
    return temp;
}

 mh* criarheap(unsigned capacidade)
{
    mh* minHeap = (mh*) malloc(sizeof(mh));
    minHeap->tam = 0;
    minHeap->capacidade = capacidade;//final do heap
    minHeap->heap = (arvcod**)malloc(minHeap->capacidade * sizeof(arvcod*));
    return minHeap;
}

void trocar_cod(arvcod** a, arvcod** b)
{
    arvcod* t = *a;
    *a = *b;
    *b = t;
}

// função para arrumar as propiedades do heap mínimo
void Heapify(mh* minHeap, int idx)
{
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < minHeap->tam && minHeap->heap[esq]->freq < minHeap->heap[menor]->freq)
      menor = esq;

    if (dir < minHeap->tam && minHeap->heap[dir]->freq < minHeap->heap[menor]->freq)
      menor = dir;

    if (menor != idx)
    {
        trocar_cod(&minHeap->heap[menor], &minHeap->heap[idx]);
        Heapify(minHeap, menor);
    }
}
//checar se o tamanho é um
int tam_um(mh* minHeap)
{
    return (minHeap->tam == 1);
}

// pegar o nó de menor valor e retirar do heap
arvcod* pegar_menor(mh* minHeap)
{
    arvcod* temp = minHeap->heap[0];
    minHeap->heap[0] = minHeap->heap[minHeap->tam - 1];
    --minHeap->tam;//retira o nó atualizando o tamanho
    Heapify(minHeap, 0);//atualiza as propiedades
    return temp;
}

// Inserir novo nó
void inserir(mh* minHeap, arvcod* arvcod)
{
    ++minHeap->tam;
    int i = minHeap->tam - 1;
    while (i && arvcod->freq < minHeap->heap[(i - 1)/2]->freq)
    {
        minHeap->heap[i] = minHeap->heap[(i - 1)/2]; //busca o local correto para inserção no heap
        i = (i - 1)/2;
    }
    minHeap->heap[i] = arvcod;
}

//faz as atualizações finais no heap
void construir_arv(mh* minHeap)
{
    int n = minHeap->tam - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        Heapify(minHeap, i);
}

void imprimi_caminho(char caminho[], int n,FILE *f)
{
    int i;
    if(n==0){
        fprintf(f,"R");
    }
    else{
        for (i = 0; i < n; ++i)
            fprintf(f,"%c", caminho[i]);
    }
    fprintf(f,"\n");
}

mh* criar_e_const_arv(char simb[], int freq[], int tam)
{
    mh* minHeap = criarheap(tam); //cria o heap
    for(int i = 0; i < tam; ++i)
        minHeap->heap[i] = criano(simb[i], freq[i]); //guarda os nós contendo as frequências e símbolos
    minHeap->tam = tam;
    construir_arv(minHeap);
    return minHeap;
}

arvcod* arvore_de_HUFFMAN(char simb[], int freq[], int tam)
{
    arvcod *esq, *dir, *top;
    mh* minHeap = criar_e_const_arv(simb, freq, tam); //heap pronto aqui
    while (!tam_um(minHeap))
    {
        esq = pegar_menor(minHeap);printf("%d\n",esq->freq);system("pause"); //coloca o nó da posição zero do heap a esquerda e atualiza o heap
        dir = pegar_menor(minHeap); //coloca o nó da posição zero do heap a direita e atualiza o heap
        top = criano(' ', esq->freq + dir->freq); //cria um nó pai para os dois acima
        top->esq = esq;
        top->dir = dir;
        inserir(minHeap, top);
    }
    return pegar_menor(minHeap);
}

//percorre a arvore e imprime os caminhos
void imprimi_codigos(arvcod* raiz, char caminho[], int top, FILE *f)
{
    caminho[top]='R';
        fprintf(f,"%c %d ", raiz->simb,raiz->freq);
        imprimi_caminho(caminho, top,f);

    if (raiz->esq)
    {
        caminho[top] = 'E';
        imprimi_codigos(raiz->esq, caminho, top + 1,f);
    }

    if (raiz->dir)
    {
        caminho[top] = 'D';
        imprimi_codigos(raiz->dir, caminho, top + 1,f);
    }

}

void Huffman(char simb[], int freq[], int tam,FILE *f)
{
    arvcod* raiz = arvore_de_HUFFMAN(simb, freq, tam);
   //Imprimir no arquivo
   char caminho[MAX], top = 0;
   imprimi_codigos(raiz, caminho, top,f);
   printf("\nArvore criada!!\n");
}


int main()
{
    char simb[50],arqin[50],arqout[50];
    int freq[50],i=0;
    FILE *f,*f2;
    printf("Digite o nome do arquivo de entrada: ");
    scanf("%s",arqin);
    printf("Digte o nome do arquivo de saida: ");
    scanf("%s",arqout);
    if ( (f=fopen(arqin,"r")) == NULL ) {
    printf("Erro na abertura do arquivo");
    exit(1);
    }
    if ( (f2=fopen(arqout,"w")) == NULL ) {
    printf("Erro na abertura do arquivo");
    exit(1);
    }
    //leitura do arquivo
    while(fscanf(f,"%c %d\n",&simb[i],&freq[i])!=EOF)
        i++;
    Huffman(simb, freq, i,f2);
    fclose(f);
    fclose(f2);
    return 0;
}
