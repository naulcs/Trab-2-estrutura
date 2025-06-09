#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 3000

typedef struct Real {
    float valor;
    struct Real* prox;
} Real;

typedef struct Go {
    int chave;
    Real* listaReais; // Lista de reais associada
    struct Go* prox;
} Go;

Go* criarGo(int valor) {
    Go* new = (Go*)malloc(sizeof(Go));
    new->chave = valor;
    new->listaReais = NULL;
    new->prox = NULL;
    return new;
}

Go* inserirFinal(Go* inicio, int valor) {
    Go* new = criarGo(valor);
    if (inicio == NULL) {
        return new;
    } else {
        Go* atual = inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = new;
        return inicio;
    }
}

Real* criarReal(float valor) {
    Real* new = (Real*)malloc(sizeof(Real));
    new->valor = valor;
    new->prox = NULL;
    return new;
}

void inserirReal(Go* Go, float valor) {
    Real* new = criarReal(valor);
    if (Go->listaReais == NULL) {
        new->prox = new;
        Go->listaReais = new;
    } else {
        Real* atual = Go->listaReais;
        while (atual->prox != Go->listaReais) {
            atual = atual->prox;
        }
        atual->prox = new;
        new->prox = Go->listaReais;
    }
}

Go* ordenar(Go* inicio, int crescente) {
    if (inicio == NULL) return NULL;
    int trocou;

    do {
        trocou = 0;
        Go* atual = inicio;
        while (atual->prox != NULL) {
            int precisaTrocar = (crescente && atual->chave > atual->prox->chave) ||
                                (!crescente && atual->chave < atual->prox->chave);
            if (precisaTrocar) {
                int temp = atual->chave;
                Real* tempLista = atual->listaReais;
                atual->chave = atual->prox->chave;
                atual->listaReais = atual->prox->listaReais;
                atual->prox->chave = temp;
                atual->prox->listaReais = tempLista;
                trocou = 1;
            }
            atual = atual->prox;
        }
    } while (trocou);

    return inicio;
}

int pertence(int inteiro, float real) {
    return fabs(inteiro - real) < 1.0;
}

void associarReais(Go* lista, float* reais, int qtd) {
    while (lista != NULL) {
        for (int i = 0; i < qtd; i++) {
            if (pertence(lista->chave, reais[i])) {
                inserirReal(lista, reais[i]);
            }
        }
        lista = lista->prox;
    }
}

void imprimirLista(Go* lista) {
    while (lista != NULL) {
        printf("[%d(", lista->chave);
        if (lista->listaReais != NULL) {
            Real* inicio = lista->listaReais;
            Real* atual = inicio;
            do {
                printf("%.2f", atual->valor);
                atual = atual->prox;
                if (atual != inicio) {
                    printf("->");
                }
            } while (atual != inicio);
        }
        printf(")]");
        if (lista->prox != NULL)
            printf("->");
        lista = lista->prox;
    }
    printf("\n");
}


int main() {
    char linha[MAX];

    while (fgets(linha, MAX, stdin)) {
        No* LE = NULL;
        No* LI = NULL;
        float reais[1000];
        int qtdReais = 0;
        int lendo = 0; 
        char* token = strtok(linha, " \n");

        while (token != NULL) {
            if (strcmp(token, "LE") == 0) {
                lendo = 1;
            } else if (strcmp(token, "LI") == 0) {
                lendo = 2;
            } else {
                float valor;
                sscanf(token, "%f", &valor);

                if (strchr(token, '.')) {
                    reais[qtdReais++] = valor;
                    lendo = 3;
                } else {
                    if (lendo == 1) {
                        LE = inserirFinal(LE, (int)valor);
                    } else if (lendo == 2) {
                        LI = inserirFinal(LI, (int)valor);
                    }
                }
            }

            token = strtok(NULL, " \n");
        }

        LE = ordenar(LE, 1);   
        LI = ordenar(LI, 0);   

        associarReais(LE, reais, qtdReais);
        associarReais(LI, reais, qtdReais);

        imprimirLista(LE);
        imprimirLista(LI);
    }

    return 0;
}