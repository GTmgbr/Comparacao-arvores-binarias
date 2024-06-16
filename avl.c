#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Estrutura de um nó da árvore AVL
typedef struct No {
    int chave;
    struct No *esquerda;
    struct No *direita;
    int altura;
} No;

int rotacoes = 0;

//Função para obter a altura de um nó
int altura(No *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

//Função para obter o valor máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

//Função para criar um novo nó
No* novoNo(int chave) {
    No* no = (No*)malloc(sizeof(No));
    no->chave = chave;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1; //Novo nó é inicialmente adicionado na folha
    return(no);
}

//Função para realizar uma rotação à direita
No *rotacaoDireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    //Realiza rotação
    x->direita = y;
    y->esquerda = T2;

    //Atualiza alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    rotacoes++;
    return x;
}

//Função para realizar uma rotação à esquerda
No *rotacaoEsquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    //Realiza rotação
    y->esquerda = x;
    x->direita = T2;

    //Atualiza alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    rotacoes++;
    return y;
}

//Função para obter o fator de balanceamento de um nó
int obterBalanceamento(No *N) {
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

//Função para inserir um novo nó na árvore AVL
No* inserir(No* no, int chave) {
    //Passo 1: Realiza a inserção normal na árvore BST
    if (no == NULL)
        return(novoNo(chave));

    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave);
    else //Valores iguais não são permitidos na árvore BST
        return no;

    //Passo 2: Atualiza a altura do nó ancestral
    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));

    //Passo 3: Obtém o fator de balanceamento do nó ancestral para verificar se ele se tornou desbalanceado
    int balanceamento = obterBalanceamento(no);

    //Se o nó se tornou desbalanceado, então existem 4 casos

    //Caso Esquerda Esquerda
    if (balanceamento > 1 && chave < no->esquerda->chave)
        return rotacaoDireita(no);

    //Caso Direita Direita
    if (balanceamento < -1 && chave > no->direita->chave)
        return rotacaoEsquerda(no);

    //Caso Esquerda Direita
    if (balanceamento > 1 && chave > no->esquerda->chave) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }

    //Caso Direita Esquerda
    if (balanceamento < -1 && chave < no->direita->chave) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;
}

//Função para encontrar o nó com o menor valor (utilizado na remoção)
No * menorValorNo(No* no) {
    No* atual = no;

    //Loop para encontrar a folha mais à esquerda
    while (atual->esquerda != NULL)
        atual = atual->esquerda;

    return atual;
}

//Função para remover um nó na árvore AVL
No* removerNo(No* raiz, int chave) {
    //Passo 1: Realiza a remoção normal na árvore BST
    if (raiz == NULL)
        return raiz;

    if (chave < raiz->chave)
        raiz->esquerda = removerNo(raiz->esquerda, chave);

    else if (chave > raiz->chave)
        raiz->direita = removerNo(raiz->direita, chave);

    else {
        //Nó com apenas um filho ou nenhum filho
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            //Caso sem filhos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            }
            else //Caso com um filho
                *raiz = *temp; //Cópia dos conteúdos do filho não vazio

            free(temp);
        }
        else {
            //Nó com dois filhos: obtém o sucessor em-ordem (menor na subárvore à direita)
            No* temp = menorValorNo(raiz->direita);

            //Copia o valor do sucessor em-ordem para este nó
            raiz->chave = temp->chave;

            //Remove o sucessor em-ordem
            raiz->direita = removerNo(raiz->direita, temp->chave);
        }
    }

    //Se a árvore tinha apenas um nó, então retorna
    if (raiz == NULL)
        return raiz;

    //Passo 2: Atualiza a altura do nó atual
    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

    //Passo 3: Obtém o fator de balanceamento deste nó (para verificar se este nó se tornou desbalanceado)
    int balanceamento = obterBalanceamento(raiz);

    //Se este nó se tornou desbalanceado, então existem 4 casos

    //Caso Esquerda Esquerda
    if (balanceamento > 1 && obterBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);

    //Caso Esquerda Direita
    if (balanceamento > 1 && obterBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    //Caso Direita Direita
    if (balanceamento < -1 && obterBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);

    //Caso Direita Esquerda
    if (balanceamento < -1 && obterBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

//Função para imprimir uma árvore AVL (pré-ordem)
void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

//Função para procurar um elemento na árvore AVL
No* procurar(No* raiz, int chave) {
    //Caso base: a chave não está presente na árvore
    if (raiz == NULL || raiz->chave == chave)
        return raiz;

    //A chave é maior que a chave no nó, ir para a subárvore direita
    if (raiz->chave < chave)
        return procurar(raiz->direita, chave);

    //A chave é menor que a chave no nó, ir para a subárvore esquerda
    return procurar(raiz->esquerda, chave);
}

//Função para executar o terceiro teste: encontrar 1000 elementos aleatórios na árvore AVL
void testarBuscaAleatoria(No* raiz, int* arr, int n) {
    clock_t inicio, fim;
    double tempo_usado;

    inicio = clock();

    for (int i = 0; i < 1000; i++) {
        int indice_aleatorio = rand() % n;
        int chave_procurar = arr[indice_aleatorio];
        No* resultado = procurar(raiz, chave_procurar);
        //Pode-se fazer algo com o resultado, mas aqui só estamos buscando
        //e não fazendo nada com o resultado
    }

    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo de execução para encontrar 1000 elementos aleatórios: %f segundos\n", tempo_usado);
}

int main() {
    No *raiz = NULL;
    int n, i;

    printf("Digite a quantidade de elementos: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    //Teste 1: Inserção de elementos ordenados
    rotacoes = 0;
    clock_t inicio, fim;
    double tempo_usado;

    inicio = clock();
    for (i = 0; i < n; i++) {
        raiz = inserir(raiz, arr[i]);
    }
    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Quantidade de rotações necessárias para inserir: %d\n", rotacoes);
    printf("Tempo de execução para inserção: %f segundos\n", tempo_usado);

    //Teste 2: Remoção de elementos ordenados
    rotacoes = 0;

    inicio = clock();
    for (i = 0; i < n; i++) {
        raiz = removerNo(raiz, arr[i]);
    }
    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Quantidade de rotações necessárias para remover: %d\n", rotacoes);
    printf("Tempo de execução para remoção: %f segundos\n", tempo_usado);

    //Teste 3: Encontrar 1000 elementos aleatórios
    testarBuscaAleatoria(raiz, arr, n);

    free(arr);
    return 0;
}
