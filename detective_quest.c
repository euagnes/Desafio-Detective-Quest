#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================
// ESTRUTURAS DE DADOS
// =============================================================

// Estrutura para a Árvore de Pistas (BST - Binary Search Tree)
typedef struct NoPista {
    char texto[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Estrutura para a Árvore do Mapa (Sala)
typedef struct Sala {
    char nome[50];
    char pista[100];         // Pista escondida na sala (pode ser vazia)
    struct Sala *esquerda;   // Porta da esquerda
    struct Sala *direita;    // Porta da direita
} Sala;

// =============================================================
// FUNÇÕES: GESTÃO DE PISTAS (BST)
// =============================================================

/**
 * Função: inserirPista
 * Objetivo: Insere uma nova pista na árvore BST mantendo a ordem alfabética.
 * Lógica: Se o texto for "menor" (alfabeticamente), vai para esquerda; se "maior", direita.
 */
NoPista* inserirPista(NoPista* raiz, char* texto) {
    // Caso base: se chegamos num ponto vazio, criamos o nó aqui
    if (raiz == NULL) {
        NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    // Se a pista já existe, não duplicamos (opcional)
    if (strcmp(texto, raiz->texto) == 0) {
        return raiz; 
    }

    // Decisão de navegação na BST: Esquerda ou Direita?
    if (strcmp(texto, raiz->texto) < 0) {
        // Texto novo vem ANTES do atual no alfabeto -> Esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else {
        // Texto novo vem DEPOIS do atual no alfabeto -> Direita
        raiz->direita = inserirPista(raiz->direita, texto);
    }

    return raiz;
}

/**
 * Função: exibirPistas
 * Objetivo: Percorre a BST "Em Ordem" (In-Order) para imprimir alfabeticamente.
 * Ordem: Visita filho esquerdo -> Imprime Raiz -> Visita filho direito.
 */
void exibirPistas(NoPista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// =============================================================
// FUNÇÕES: GESTÃO DO MAPA E NAVEGAÇÃO
// =============================================================

/**
 * Função: criarSala
 * Objetivo: Cria um nó do mapa (Sala) com nome e uma pista opcional.
 */
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista); // Copia a pista (pode ser string vazia "")
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/**
 * Função: explorarSalasComPistas
 * Objetivo: Loop principal do jogo. Navega pelo mapa e coleta pistas.
 */
void explorarSalasComPistas(Sala* mapaInicial) {
    Sala* atual = mapaInicial;
    NoPista* inventarioPistas = NULL; // Começamos sem pistas
    char escolha = ' ';

    printf("\n=== DETECTIVE QUEST: INICIO DA INVESTIGACAO ===\n");

    // Loop continua até o usuário escolher sair ('s')
    while (escolha != 's' && escolha != 'S') {
        if (atual == NULL) {
            printf("\n(!) Erro critico: Voce caiu no limbo (sala nula)!\n");
            break;
        }

        printf("\n------------------------------------------------");
        printf("\nVOCE ESTA EM: [ %s ]", atual->nome);
        
        // 1. Verificar e Coletar Pista
        if (strlen(atual->pista) > 0) {
            printf("\n(i) Voce encontrou uma pista: \"%s\"", atual->pista);
            // Insere na BST automaticamente
            inventarioPistas = inserirPista(inventarioPistas, atual->pista);
            // Opcional: Limpar a pista da sala para não coletar de novo
            // strcpy(atual->pista, ""); 
        } else {
            printf("\n(i) Nenhuma pista encontrada neste local.");
        }
        printf("\n------------------------------------------------");

        // 2. Menu de Navegação
        printf("\nOpcoes de caminho:");
        if (atual->esquerda) printf("\n (e) Ir para esquerda: %s", atual->esquerda->nome);
        else                 printf("\n (e) Esquerda: Sem saida");
        
        if (atual->direita)  printf("\n (d) Ir para direita:  %s", atual->direita->nome);
        else                 printf("\n (d) Direita:  Sem saida");
        
        printf("\n (s) Sair e ver relatorio final");
        
        printf("\n\nSua escolha: ");
        scanf(" %c", &escolha);

        // 3. Lógica de Movimento
        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("\n(!) Beco sem saida!\n");
        }
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("\n(!) Beco sem saida!\n");
        }
        else if (escolha != 's' && escolha != 'S') {
            printf("\n(!) Comando invalido.\n");
        }
    }

    // Fim da exploração - Relatório
    printf("\n\n================================================");
    printf("\n              RELATORIO DO DETETIVE             ");
    printf("\n================================================\n");
    if (inventarioPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas coletadas (Ordem Alfabetica):\n");
        exibirPistas(inventarioPistas);
    }
    printf("================================================\n");
}

/**
 * Função: main
 * Objetivo: Configura o cenário e inicia o jogo.
 */
int main() {
    // 1. Construção do Mapa (Árvore Binária Fixa)
    // Nível 0
    Sala* mapa = criarSala("Hall de Entrada", "");

    // Nível 1
    mapa->esquerda = criarSala("Cozinha", "Faca suja de geleia");
    mapa->direita = criarSala("Biblioteca", "");

    // Nível 2 - Lado da Cozinha
    mapa->esquerda->esquerda = criarSala("Despensa", "Pegadas de lama tamanho 42");
    mapa->esquerda->direita = criarSala("Sala de Jantar", "");

    // Nível 2 - Lado da Biblioteca
    mapa->direita->esquerda = criarSala("Escritorio", "Bilhete rasgado com a letra A");
    mapa->direita->direita = criarSala("Jardim de Inverno", "Luva perdida");

    // Nível 3 - Profundidade extra
    mapa->direita->esquerda->direita = criarSala("Cofre Secreto", "Documento confidencial");

    // 2. Iniciar Jogo
    explorarSalasComPistas(mapa);

    return 0;
}