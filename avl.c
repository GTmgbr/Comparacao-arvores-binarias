#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Estrutura de um n� da �rvore AVL
typedef struct No {
    int chave;
    struct No *esquerda;
    struct No *direita;
    int altura;
} No;

int rotacoes = 0;

//Fun��o para obter a altura de um n�
int altura(No *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

//Fun��o para obter o valor m�ximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

//Fun��o para criar um novo n�
No* novoNo(int chave) {
    No* no = (No*)malloc(sizeof(No));
    no->chave = chave;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1; //Novo n� � inicialmente adicionado na folha
    return(no);
}

//Fun��o para realizar uma rota��o � direita
No *rotacaoDireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    //Realiza rota��o
    x->direita = y;
    y->esquerda = T2;

    //Atualiza alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    rotacoes++;
    return x;
}

//Fun��o para realizar uma rota��o � esquerda
No *rotacaoEsquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    //Realiza rota��o
    y->esquerda = x;
    x->direita = T2;

    //Atualiza alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    rotacoes++;
    return y;
}

//Fun��o para obter o fator de balanceamento de um n�
int obterBalanceamento(No *N) {
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

//Fun��o para inserir um novo n� na �rvore AVL
No* inserir(No* no, int chave) {
    //Passo 1: Realiza a inser��o normal na �rvore BST
    if (no == NULL)
        return(novoNo(chave));

    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave);
    else //Valores iguais n�o s�o permitidos na �rvore BST
        return no;

    //Passo 2: Atualiza a altura do n� ancestral
    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));

    //Passo 3: Obt�m o fator de balanceamento do n� ancestral para verificar se ele se tornou desbalanceado
    int balanceamento = obterBalanceamento(no);

    //Se o n� se tornou desbalanceado, ent�o existem 4 casos

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

//Fun��o para encontrar o n� com o menor valor (utilizado na remo��o)
No * menorValorNo(No* no) {
    No* atual = no;

    //Loop para encontrar a folha mais � esquerda
    while (atual->esquerda != NULL)
        atual = atual->esquerda;

    return atual;
}

//Fun��o para remover um n� na �rvore AVL
No* removerNo(No* raiz, int chave) {
    //Passo 1: Realiza a remo��o normal na �rvore BST
    if (raiz == NULL)
        return raiz;

    if (chave < raiz->chave)
        raiz->esquerda = removerNo(raiz->esquerda, chave);

    else if (chave > raiz->chave)
        raiz->direita = removerNo(raiz->direita, chave);

    else {
        //N� com apenas um filho ou nenhum filho
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            //Caso sem filhos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            }
            else //Caso com um filho
                *raiz = *temp; //C�pia dos conte�dos do filho n�o vazio

            free(temp);
        }
        else {
            //N� com dois filhos: obt�m o sucessor em-ordem (menor na sub�rvore � direita)
            No* temp = menorValorNo(raiz->direita);

            //Copia o valor do sucessor em-ordem para este n�
            raiz->chave = temp->chave;

            //Remove o sucessor em-ordem
            raiz->direita = removerNo(raiz->direita, temp->chave);
        }
    }

    //Se a �rvore tinha apenas um n�, ent�o retorna
    if (raiz == NULL)
        return raiz;

    //Passo 2: Atualiza a altura do n� atual
    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

    //Passo 3: Obt�m o fator de balanceamento deste n� (para verificar se este n� se tornou desbalanceado)
    int balanceamento = obterBalanceamento(raiz);

    //Se este n� se tornou desbalanceado, ent�o existem 4 casos

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

//Fun��o para imprimir uma �rvore AVL (pr�-ordem)
void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

//Fun��o para procurar um elemento na �rvore AVL
No* procurar(No* raiz, int chave) {
    //Caso base: a chave n�o est� presente na �rvore
    if (raiz == NULL || raiz->chave == chave)
        return raiz;

    //A chave � maior que a chave no n�, ir para a sub�rvore direita
    if (raiz->chave < chave)
        return procurar(raiz->direita, chave);

    //A chave � menor que a chave no n�, ir para a sub�rvore esquerda
    return procurar(raiz->esquerda, chave);
}

//Fun��o para executar o terceiro teste: encontrar 1000 elementos aleat�rios na �rvore AVL
void testarBuscaAleatoria(No* raiz, int* arr, int n) {
    clock_t inicio, fim;
    double tempo_usado;

    inicio = clock();

    for (int i = 0; i < 1000; i++) {
        int indice_aleatorio = rand() % n;
        int chave_procurar = arr[indice_aleatorio];
        No* resultado = procurar(raiz, chave_procurar);
        //Pode-se fazer algo com o resultado, mas aqui s� estamos buscando
        //e n�o fazendo nada com o resultado
    }

    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo de execu��o para encontrar 1000 elementos aleat�rios: %f segundos\n", tempo_usado);
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

    //Teste 1: Inser��o de elementos ordenados
    rotacoes = 0;
    clock_t inicio, fim;
    double tempo_usado;

    inicio = clock();
    for (i = 0; i < n; i++) {
        raiz = inserir(raiz, arr[i]);
    }
    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Quantidade de rota��es necess�rias para inserir: %d\n", rotacoes);
    printf("Tempo de execu��o para inser��o: %f segundos\n", tempo_usado);

    //Teste 2: Remo��o de elementos ordenados
    rotacoes = 0;

    inicio = clock();
    for (i = 0; i < n; i++) {
        raiz = removerNo(raiz, arr[i]);
    }
    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Quantidade de rota��es necess�rias para remover: %d\n", rotacoes);
    printf("Tempo de execu��o para remo��o: %f segundos\n", tempo_usado);

    //Teste 3: Encontrar 1000 elementos aleat�rios
    testarBuscaAleatoria(raiz, arr, n);

    free(arr);
    return 0;
}
