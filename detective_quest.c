#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da Sala (Nó da árvore)
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    struct Sala *esquerda;   // Caminho para a esquerda
    struct Sala *direita;    // Caminho para a direita
} Sala;

/**
 * Função: criarSala
 * Objetivo: Aloca memória dinamicamente para uma nova sala e define seus valores iniciais.
 * Retorno: Um ponteiro para a nova sala criada.
 */
Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL; // Inicialmente sem conexões
    novaSala->direita = NULL;  // Inicialmente sem conexões
    
    return novaSala;
}

/**
 * Função: explorarSalas
 * Objetivo: Gerencia a navegação interativa do jogador pela árvore.
 * Lógica: Loop continua enquanto houver pelo menos um caminho (esquerda ou direita).
 */
void explorarSalas(Sala* atual) {
    char escolha;

    printf("\n=== INICIO DA EXPLORACAO ===\n");

    // Enquanto o nó atual não for uma folha (tiver pelo menos um caminho)
    while (atual->esquerda != NULL || atual->direita != NULL) {
        printf("\nVoce esta em: [ %s ]\n", atual->nome);
        printf("Para onde deseja ir?\n");
        
        // Verifica quais caminhos existem para mostrar no menu
        if (atual->esquerda != NULL) printf(" (e) Esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf(" (d) Direita:  %s\n", atual->direita->nome);
        
        printf("Escolha: ");
        scanf(" %c", &escolha); // O espaço antes de %c ignora quebras de linha pendentes

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move o ponteiro para a sala da esquerda
            } else {
                printf("(!) Nao ha caminho a esquerda.\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita; // Move o ponteiro para a sala da direita
            } else {
                printf("(!) Nao ha caminho a direita.\n");
            }
        } 
        else {
            printf("(!) Opcao invalida. Tente 'e' ou 'd'.\n");
        }
    }

    // Chegamos em um nó folha (sem saídas)
    printf("\n------------------------------------------------");
    printf("\nVoce chegou em: [ %s ]", atual->nome);
    printf("\nNao ha mais saidas aqui. Voce encontrou uma pista final!");
    printf("\n------------------------------------------------\n");
}

/**
 * Função: main
 * Objetivo: Monta a estrutura do mapa (árvore) e inicia o jogo.
 */
int main() {
    // 1. Criação do Mapa (Árvore Binária)
    // Raiz
    Sala* mapa = criarSala("Hall de Entrada");

    // Nível 1
    mapa->esquerda = criarSala("Sala de Estar");
    mapa->direita = criarSala("Cozinha");

    // Nível 2 (Conexões da Sala de Estar)
    mapa->esquerda->esquerda = criarSala("Biblioteca");
    mapa->esquerda->direita = criarSala("Jardim de Inverno");

    // Nível 2 (Conexões da Cozinha)
    mapa->direita->esquerda = criarSala("Despensa");
    mapa->direita->direita = criarSala("Sala de Jantar");

    // Nível 3 (Adicionando profundidade para teste)
    mapa->esquerda->esquerda->esquerda = criarSala("Passagem Secreta (FIM)");
    
    // 2. Início da Exploração
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Sua missao e explorar a mansao ate encontrar um local sem saida.\n");
    
    explorarSalas(mapa);

    // Nota: Em um programa real complexo, deveríamos criar uma função para liberar
    // a memória (free) de todos os nós antes de sair.
    
    return 0;
}