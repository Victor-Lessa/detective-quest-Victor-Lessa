#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// Estrutura de uma sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura da tabela hash
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// Função que cria uma sala
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função que insere uma pista na BST
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// Função que exibe pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função de hash simples
int hash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

// Insere pista e suspeito na tabela hash
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int idx = hash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

// Busca suspeito associado à pista
char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int idx = hash(pista);
    HashNode *atual = tabela[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Conta quantas pistas apontam para o suspeito acusado
int contarPistasSuspeito(PistaNode *raiz, HashNode *tabela[], const char *suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    char *s = encontrarSuspeito(tabela, raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        cont++;
    return cont + contarPistasSuspeito(raiz->esquerda, tabela, suspeito)
               + contarPistasSuspeito(raiz->direita, tabela, suspeito);
}

// Exploração da mansão e coleta de pistas
void explorarSalas(Sala *atual, PistaNode **pistas, HashNode *tabela[]) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("Escolha o caminho:\n");
        if (atual->esquerda) printf("e - Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita) printf("d - Ir para a direita (%s)\n", atual->direita->nome);
        printf("s - Sair da exploração\nOpção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita) atual = atual->direita;
        else if (escolha == 's') break;
        else printf("Opção inválida.\n");
    }
}

// Verifica acusação final
void verificarSuspeitoFinal(PistaNode *pistas, HashNode *tabela[]) {
    char acusado[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    int cont = contarPistasSuspeito(pistas, tabela, acusado);
    printf("\nPistas que apontam para %s: %d\n", acusado, cont);
    if (cont >= 2)
        printf("Acusação válida! %s é considerado culpado.\n", acusado);
    else
        printf("Acusação fraca. Poucas evidências contra %s.\n", acusado);
}

// Função principal
int main() {
    // Criação da mansão
    Sala *hall = criarSala("Hall de Entrada", "Pegadas misteriosas");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala *cozinha = criarSala("Cozinha", "Faca suja");
    Sala *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala *jardim = criarSala("Jardim", "");
    Sala *quarto = criarSala("Quarto Secreto", "Diário escondido");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;

    // Inicialização da BST e tabela hash
    PistaNode *pistasColetadas = NULL;
    HashNode *tabela[TAM_HASH] = {NULL};

    // Associação pista → suspeito
    inserirNaHash(tabela, "Pegadas misteriosas", "Carlos");
    inserirNaHash(tabela, "Copo quebrado", "Ana");
    inserirNaHash(tabela, "Faca suja", "Carlos");
    inserirNaHash(tabela, "Livro fora do lugar", "Bruno");
    inserirNaHash(tabela, "Diário escondido", "Carlos");

    // Início do jogo
    printf("Bem-vindo ao capítulo final de Detective Quest!\nExplore a mansão e descubra o culpado...\n");
    explorarSalas(hall, &pistasColetadas, tabela);

    // Exibe pistas coletadas
    printf("\nPistas coletadas:\n");
    exibirPistas(pistasColetadas);

    // Julgamento final
    verificarSuspeitoFinal(pistasColetadas, tabela);

    return 0;
}
