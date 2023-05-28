#include <stdio.h>
#include <stdlib.h>
#define N 10
#define M 10

typedef struct node{
    int info, linha, coluna;
    struct node *direita;
    struct node *baixo;
}node;

typedef struct MatrizEsparsa{
    node* linha[N];
    node* coluna[M];
}MatrizEsparsa;

void inicializarMatriz (MatrizEsparsa *matriz){
    int i;
    for (i=0; i<N; i++){
        matriz->linha[i] = NULL;
    }
    for (i=0; i<M; i++){
        matriz->coluna[i] = NULL;
    }
}

void inserir (MatrizEsparsa *matriz, int lin, int col, int info){
    node *novoNo, *pontlin, *pontcol, *ant;
    novoNo = malloc(sizeof(node));
    novoNo->info = info;
    novoNo->linha = lin;
    novoNo->coluna = col;
    novoNo->direita = NULL;
    novoNo->baixo = NULL;
    pontcol = matriz->coluna[col];
    pontlin = matriz->linha[lin];

    if (pontcol == NULL){ // NÃO HÁ ELEMENTOS NA COLUNA
        matriz->coluna[col] = novoNo;
    }else{ //INSERÇÃO ORDENADA NA COLUNA CONSIDERANDO O VALOR DA LINHA
        ant = NULL;
        while ((pontcol != NULL) && (pontcol->linha < lin))
        {
            ant = pontcol;
            pontcol = pontcol->baixo;
        }
        if (pontcol != NULL){
            if (pontcol->linha == lin){
                printf("Ja existe elemento na linha\n");
            }else{
                if (ant == NULL){ // INSERCAO NO INICIO
                    novoNo->baixo = matriz->coluna[col];
                    matriz->coluna[col] = novoNo;
                }else{ // INSERCAO NO MEIO
                    novoNo->baixo = pontcol;
                    ant->baixo = novoNo;
                }
            }
        }else{
            ant->baixo = novoNo;
        }
    }

    if (pontlin == NULL){ // NÃO HÁ ELEMENTOS NA LINHA
        matriz->linha[lin] = novoNo;
    }else{  // INSERÇÃO ORDENADA NA LINHA CONSIDERANDO O VALOR DA COLUNA
        ant = NULL;
        while ((pontlin != NULL) && (pontlin->coluna < col))
        {
            ant = pontlin;
            pontlin = pontlin->direita;
        }
        if (pontlin != NULL){
            if (pontlin->coluna == col){
                printf("Ja existe elemento na coluna\n");
            }else{
                if (ant == NULL){ // INSERCAO NO INICIO
                    novoNo->direita = matriz->linha[lin];
                    matriz->linha[lin] = novoNo;
                }else{ // INSERCAO NO MEIO
                    novoNo->direita = pontlin;
                    ant->direita = novoNo;
                }
            }
        }else{
            ant->direita = novoNo;
        }
    }
}

void remover(MatrizEsparsa *m, int lin, int col) {
    node *pontlin, *pontcol, *ant;
    pontcol = m->coluna[col];
    pontlin = m->linha[lin];
    ant = NULL;
    while (pontcol != NULL && pontcol->linha != lin) {
        ant = pontcol;
        pontcol = pontcol->direita;
    }
    if (pontcol != NULL) {
        if (ant == NULL) {
            m->coluna[col] = pontcol->direita;
        } else {
        ant->direita = pontcol->direita;
        }
        free(pontcol);
    }
    ant = NULL;
    while (pontlin != NULL && pontlin->coluna != col) {
        ant = pontlin;
        pontlin = pontlin->direita;
    }
    if (pontlin != NULL) {
        if (ant == NULL) {
            m->linha[lin] = pontlin->direita;
        } else {
            ant->direita = pontlin->direita;
        } free(pontlin);
    }
}

void somar (MatrizEsparsa *matriz, int lin, int k){
    node* ptr;
    int i;
    ptr = matriz->linha[lin];
    if (ptr == NULL){
        for (i=0; i<M; i++){
            inserir(matriz, lin, i, k);
        }
        return;
    }

    for (i=0; i<M; i++){
        if (i != ptr->coluna){
            inserir(matriz, lin, i, k);
        }else{
            if (ptr->info + k == 0){
                if (ptr->direita != NULL){
                    ptr = ptr->direita;
                }
                remover(matriz, lin, i);
            }else{
                ptr->info = ptr->info + k;
                if (ptr->direita != NULL){
                    ptr = ptr->direita;
                }
            }
        }
    }
}

void printarMatriz (MatrizEsparsa *matriz){
    node *aux;
    int lin, col;
    for (lin=0; lin < N; lin++){
        aux = matriz->linha[lin];
        printf("\n");
        for (col = 0; col < M; col++){
            if ((aux != NULL) && (aux->coluna == col)){
                printf("%d\t", aux->info);
                aux = aux->direita;
            }
            else{
                printf("0\t");
            }
        }
    }
    printf("\n");
    printf("\n");
}

int main()
{
    MatrizEsparsa matriz;
    inicializarMatriz(&matriz);
    inserir(&matriz, 0, 2, -10);
    inserir(&matriz, 0, 0, 100);
    inserir(&matriz, 0, 1, 10);
    inserir(&matriz, 1, 0, -5);
    inserir(&matriz, 1, 2, -5);
    inserir(&matriz, 1, 5, 6);
    inserir(&matriz, 5, 3, 13);
    inserir(&matriz, 8, 2, -12);
    inserir(&matriz, 9, 1, 23);
    inserir(&matriz, 9, 9, 65);
    inserir(&matriz, 3, 3, 26);
    inserir(&matriz, 4, 2, 21);
    printarMatriz(&matriz);
    somar(&matriz, 1, 10);
    printarMatriz(&matriz);
    somar(&matriz, 2, 10);
    printarMatriz(&matriz);
    somar(&matriz, 5, 10);
    printarMatriz(&matriz);
    return 0;
}
