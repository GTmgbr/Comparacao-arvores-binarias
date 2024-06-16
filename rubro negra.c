#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Estrutura do Nó da Árvore AVL
typedef struct No {
    int dado;
    int cor; // 0 -> Preto, 1 -> Vermelho
    struct No *esquerda, *direita, *pai;
} No;

//Definição de Cores
#define VERMELHO 1
#define PRETO 0

No *raiz = NULL;
int rotacoes = 0;

//Função para criar um novo nó
No* novoNo(int dado) {
    No *no = (No*)malloc(sizeof(No));
    no->dado = dado;
    no->cor = VERMELHO;
    no->esquerda = no->direita = no->pai = NULL;
    return no;
}

//Função para realizar uma rotação à esquerda
void rotacaoEsquerda(No **raiz, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL) y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL) *raiz = y;
    else if (x == x->pai->esquerda) x->pai->esquerda = y;
    else x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
    rotacoes++;
}

//Função para realizar uma rotação à direita
void rotacaoDireita(No **raiz, No *x) {
    No *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != NULL) y->direita->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL) *raiz = y;
    else if (x == x->pai->direita) x->pai->direita = y;
    else x->pai->esquerda = y;
    y->direita = x;
    x->pai = y;
    rotacoes++;
}

//Função para corrigir a árvore após a inserção
void corrigirInsercao(No **raiz, No *z) {
    while (z != *raiz && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            No *y = z->pai->pai->direita;
            if (y != NULL && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            No *y = z->pai->pai->esquerda;
            if (y != NULL && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

//Função para inserir um novo nó na árvore
void inserir(No **raiz, int dado) {
    No *z = novoNo(dado);
    No *y = NULL;
    No *x = *raiz;
    while (x != NULL) {
        y = x;
        if (z->dado < x->dado) x = x->esquerda;
        else x = x->direita;
    }
    z->pai = y;
    if (y == NULL) *raiz = z;
    else if (z->dado < y->dado) y->esquerda = z;
    else y->direita = z;
    corrigirInsercao(raiz, z);
}

//Função para substituir dois nós
void transplante(No **raiz, No *u, No *v) {
    if (u->pai == NULL) *raiz = v;
    else if (u == u->pai->esquerda) u->pai->esquerda = v;
    else u->pai->direita = v;
    if (v != NULL) v->pai = u->pai;
}

//Função para corrigir a árvore após a remoção
void corrigirRemocao(No **raiz, No *x) {
    while (x != *raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            No *w = x->pai->direita;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                w = x->pai->direita;
            }
            if (w->esquerda->cor == PRETO && w->direita->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->direita->cor == PRETO) {
                    w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->direita->cor = PRETO;
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
            }
        } else {
            No *w = x->pai->esquerda;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                w = x->pai->esquerda;
            }
            if (w->direita->cor == PRETO && w->esquerda->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esquerda->cor == PRETO) {
                    w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esquerda->cor = PRETO;
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    x->cor = PRETO;
}

//Função para encontrar o nó mínimo
No* minimo(No *no) {
    while (no->esquerda != NULL) no = no->esquerda;
    return no;
}

//Função para remover um nó da árvore
void removerNo(No **raiz, No *z) {
    No *y = z;
    No *x;
    int cor_original_y = y->cor;
    if (z->esquerda == NULL) {
        x = z->direita;
        transplante(raiz, z, z->direita);
    } else if (z->direita == NULL) {
        x = z->esquerda;
        transplante(raiz, z, z->esquerda);
    } else {
        y = minimo(z->direita);
        cor_original_y = y->cor;
        x = y->direita;
        if (y->pai == z) {
            if (x != NULL) x->pai = y;
        } else {
            transplante(raiz, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        transplante(raiz, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    if (cor_original_y == PRETO && x != NULL) corrigirRemocao(raiz, x);
}

//Função para buscar um nó na árvore
No* buscar(No *raiz, int dado) {
    while (raiz != NULL && dado != raiz->dado) {
        if (dado < raiz->dado) raiz = raiz->esquerda;
        else raiz = raiz->direita;
    }
    return raiz;
}

//Função para medir o tempo em segundos
double obterTempoEmSegundos() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

//Função para realizar o teste de busca aleatória
void testeBuscaAleatoria(No *raiz, int n) {
    srand(time(NULL));  //Inicializa o gerador de números aleatórios
    double tempo_inicio, tempo_fim;
    int i;
    int contagem_encontrados = 0;

    tempo_inicio = obterTempoEmSegundos();

    for (i = 0; i < 1000; i++) {
        int alvo = rand() % n + 1;  //Gera um número aleatório entre 1 e n
        No *resultado = buscar(raiz, alvo);
        if (resultado != NULL) {
            contagem_encontrados++;
        }
    }

    tempo_fim = obterTempoEmSegundos();

    printf("Tempo de execução para buscar 1000 elementos aleatórios: %.6f segundos\n", tempo_fim - tempo_inicio);
}

//Função principal
int main() {
    int n, i, dado;
    double tempo_inicio, tempo_fim;
    printf("Digite a quantidade de elementos que a árvore terá: ");
    scanf("%d", &n);

    //Medir tempo de inserção
    rotacoes = 0;
    tempo_inicio = obterTempoEmSegundos();
    for (i = 1; i <= n; i++) {
        inserir(&raiz, i);
    }
    tempo_fim = obterTempoEmSegundos();
    printf("Rotações necessárias para inserção ordenada: %d\n", rotacoes);
    printf("Tempo de execução para inserção: %.6f segundos\n", tempo_fim - tempo_inicio);

    //Medir tempo de remoção
    rotacoes = 0;
    tempo_inicio = obterTempoEmSegundos();
    for (i = 1; i <= n; i++) {
        No *no = buscar(raiz, i);
        if (no != NULL) removerNo(&raiz, no);
    }
    tempo_fim = obterTempoEmSegundos();
    printf("Rotações necessárias para remoção ordenada: %d\n", rotacoes);
    printf("Tempo de execução para remoção: %.6f segundos\n", tempo_fim - tempo_inicio);

    testeBuscaAleatoria(raiz, n);

    return 0;
}
