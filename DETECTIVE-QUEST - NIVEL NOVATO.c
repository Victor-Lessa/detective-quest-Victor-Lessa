#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função que cria uma sala com nome e retorna ponteiro
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Função que permite a exploração da mansão
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho. Não há mais salas para explorar.\n");
            break;
        }

        printf("Escolha o caminho:\n");
        if (atual->esquerda != NULL) printf("e - Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("d - Ir para a direita (%s)\n", atual->direita->nome);
        printf("s - Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

// Função principal que monta o mapa e inicia a exploração
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de entrada");
    Sala *salaEstar = criarSala("Sala de estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *quarto = criarSala("Quarto secreto");

    // Montagem da árvore binária
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;

    // Início da exploração
    printf("Bem-vindo ao Detective Quest!\nExplore a mansão e descubra seus segredos...\n");
    explorarSalas(hall);

    // Liberação da memória
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(quarto);

    return 0;
}
