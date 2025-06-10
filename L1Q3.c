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
    Real* listaReais; 
    struct Go* prox;
    struct Go* ant;  
} Go;

Go* criarGo(int valor) {
    Go* novo = (Go*)malloc(sizeof(Go));
    novo->chave = valor;
    novo->listaReais = NULL;
    novo->prox = NULL;
    novo->ant = NULL;
    return novo;
}

Go* inserirFinal(Go* inicio, int valor) {
    Go* novo = criarGo(valor);
    if (inicio == NULL) {
        return novo;
    } else {
        Go* atual = inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
        novo->ant = atual; 
        return inicio;
    }
}

void inserirRealOrdenadoDecrescente(Go* no, float valor) {
    Real* novo = (Real*)malloc(sizeof(Real));
    novo->valor = valor;

    if (no->listaReais == NULL) {
        novo->prox = novo;
        no->listaReais = novo;
        return;
    }

    Real* atual = no->listaReais;
    Real* ant = NULL;

    do {
        if (valor > atual->valor) {
            break;
        }
        ant = atual;
        atual = atual->prox;
    } while (atual != no->listaReais);

    if (ant == NULL) {
      Real* ultimo = no->listaReais;
        while (ultimo->prox != no->listaReais)
            ultimo = ultimo->prox;

        novo->prox = no->listaReais;
        ultimo->prox = novo;
        no->listaReais = novo;     
    } else {
        ant->prox = novo;
        novo->prox = atual;
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
    for (int i = 0; i < qtd; i++) {
        Go* atual = lista;
        while (atual != NULL) {
            if (pertence(atual->chave, reais[i])) {
                inserirRealOrdenadoDecrescente(atual, reais[i]);
                break;        
            }
            atual = atual->prox;
        }
    }
}

void imprimirLista(Go* lista) {
    printf("[");
    Go* atual = lista;
    while (atual != NULL) {
        printf("%d(", atual->chave);
        if (atual->listaReais != NULL) {
            Real* inicio = atual->listaReais;
            Real* r = inicio;
            do {
                printf("%.2f", r->valor);
                r = r->prox;
                if (r != inicio) printf(" - >");
            } while (r != inicio);
        }
        printf(")");
        if (atual->prox != NULL) printf(" - > ");
        atual = atual->prox;
    }
    printf("]\n");
}

int main() {
    freopen("L1Q3.out", "w", stdout);

    FILE *arquivo_in = fopen("L1Q3.in", "r");

    if (arquivo_in == NULL) {
        perror("Erro ao abrir o arquivo L1Q3.in");
        return 1;
    }

    char linha[MAX];

    while (fgets(linha, MAX, arquivo_in)) {
        Go* LE = NULL;
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
                } else {
                    if (lendo == 1) {
                        LE = inserirFinal(LE, (int)valor);
                    }
                }
            }
            token = strtok(NULL, " \n");
        }

        LE = ordenar(LE, 1);  
        associarReais(LE, reais, qtdReais);

        imprimirLista(LE);
    }

    fclose(arquivo_in);

    return 0;
}