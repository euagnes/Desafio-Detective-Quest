#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 11 // Tamanho primo para melhor distribuição

// =============================================================
// ESTRUTURAS DE DADOS
// =============================================================

// 1. Tabela Hash (Associação: Pista -> Suspeito)
typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *proximo; // Tratamento de colisão (Encadeamento)
} HashItem;

// 2. BST (Inventário de Pistas Coletadas)
typedef struct NoPista {
    char texto[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// 3. Árvore Binária (Mapa da Mansão)
typedef struct Sala {
    char nome[50];
    char pista[100];         
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// =============================================================
// MÓDULO: TABELA HASH (INTELIGÊNCIA)
// =============================================================

// Função de Hash simples (Soma ASCII)
int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

/**
 * Função: inserirNaHash
 * Objetivo: Mapeia uma pista a um suspeito na tabela hash.
 */
void inserirNaHash(HashItem* tabela[], char* pista, char* suspeito) {
    int indice = funcaoHash(pista);
    
    HashItem* novo = (HashItem*)malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;

    // Inserção no início da lista encadeada (caso haja colisão)
    if (tabela[indice] == NULL) {
        tabela[indice] = novo;
    } else {
        novo->proximo = tabela[indice];
        tabela[indice] = novo;
    }
}

/**
 * Função: encontrarSuspeito
 * Objetivo: Dada uma pista, retorna o nome do suspeito associado.
 */
char* encontrarSuspeito(HashItem* tabela[], char* pistaBusca) {
    int indice = funcaoHash(pistaBusca);
    HashItem* atual = tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pistaBusca) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return "Desconhecido";
}

// =============================================================
// MÓDULO: BST (INVENTÁRIO)
// =============================================================

/**
 * Função: inserirPista (BST)
 * Objetivo: Insere pista coletada em ordem alfabética.
 */
NoPista* inserirPista(NoPista* raiz, char* texto) {
    if (raiz == NULL) {
        NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->direita = inserirPista(raiz->direita, texto);
    
    return raiz;
}

void exibirPistasOrdem(NoPista* raiz) {
    if (raiz != NULL) {
        exibirPistasOrdem(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistasOrdem(raiz->direita);
    }
}

// =============================================================
// MÓDULO: MAPA E LÓGICA DE JOGO
// =============================================================

/**
 * Função: criarSala
 * Objetivo: Cria dinamicamente um cômodo do mapa.
 */
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/**
 * Função Auxiliar: contarEvidencias
 * Objetivo: Percorre a BST recursivamente e conta quantas pistas apontam para o acusado.
 */
int contarEvidencias(NoPista* raizBST, HashItem* tabelaHash[], char* acusado) {
    if (raizBST == NULL) return 0;

    int contagem = 0;
    
    // Verifica a pista do nó atual na Hash Table
    char* suspeitoReal = encontrarSuspeito(tabelaHash, raizBST->texto);
    
    // Comparação simples de strings (ignora case sensitive neste exemplo simples)
    if (strcmp(suspeitoReal, acusado) == 0) {
        contagem = 1;
    }

    return contagem + 
           contarEvidencias(raizBST->esquerda, tabelaHash, acusado) + 
           contarEvidencias(raizBST->direita, tabelaHash, acusado);
}

/**
 * Função: verificarSuspeitoFinal
 * Objetivo: Fase de julgamento.
 */
void verificarSuspeitoFinal(NoPista* raizBST, HashItem* tabelaHash[]) {
    char acusado[50];
    
    printf("\n\n=== TRIBUNAL DO DETECTIVE ===\n");
    printf("Baseado nas pistas coletadas:\n");
    exibirPistasOrdem(raizBST);
    
    printf("\nQuem e o culpado? (Digite o nome exato: Mordomo, Jardineiro, Cozinheira): ");
    scanf("%s", acusado);

    int provas = contarEvidencias(raizBST, tabelaHash, acusado);

    printf("\nAnalisando evidencias contra %s...\n", acusado);
    printf("Total de provas encontradas: %d\n", provas);

    if (provas >= 2) {
        printf("\n>>> SUCESSO! Voce reuniu provas suficientes. %s foi preso(a)! <<<\n", acusado);
    } else {
        printf("\n>>> FRACASSO! Provas insuficientes. O culpado escapou por falta de evidencias. <<<\n");
    }
}

/**
 * Função: explorarSalas
 * Objetivo: Loop principal de navegação.
 */
void explorarSalas(Sala* mapa, HashItem* tabelaHash[]) {
    Sala* atual = mapa;
    NoPista* inventario = NULL;
    char escolha = ' ';

    while (escolha != 's' && escolha != 'S') {
        printf("\n--- Local: %s ---\n", atual->nome);
        
        // Coleta automática
        if (strlen(atual->pista) > 0) {
            printf("[!] Pista encontrada: '%s'\n", atual->pista);
            inventario = inserirPista(inventario, atual->pista);
            strcpy(atual->pista, ""); // Remove pista para não duplicar na BST se voltar
        }

        printf("Caminhos: (e)sq, (d)ir ou (s)air para acusar: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita) atual = atual->direita;
    }

    verificarSuspeitoFinal(inventario, tabelaHash);
}

// =============================================================
// FUNÇÃO PRINCIPAL
// =============================================================
int main() {
    // 1. Inicializar Tabela Hash
    HashItem* tabelaSuspeitos[TAM_HASH] = {NULL};

    // Configurando a "Verdade" do jogo (Pista -> Dono)
    inserirNaHash(tabelaSuspeitos, "Faca suja", "Cozinheira");
    inserirNaHash(tabelaSuspeitos, "Receita de veneno", "Cozinheira");
    inserirNaHash(tabelaSuspeitos, "Luva de couro", "Jardineiro");
    inserirNaHash(tabelaSuspeitos, "Pala com lama", "Jardineiro");
    inserirNaHash(tabelaSuspeitos, "Relogio de ouro", "Mordomo");

    // 2. Construir Mapa (Árvore Binária)
    // O culpado neste cenário será a Cozinheira (se o jogador achar as pistas certas)
    Sala* mansao = criarSala("Hall", "");
    
    mansao->esquerda = criarSala("Cozinha", "Faca suja");
    mansao->direita = criarSala("Biblioteca", "Relogio de ouro"); // Pista falsa/outro suspeito
    
    mansao->esquerda->esquerda = criarSala("Despensa", "Receita de veneno"); // Pista chave!
    mansao->esquerda->direita = criarSala("Sala de Jantar", "");
    
    mansao->direita->direita = criarSala("Jardim", "Luva de couro");

    // 3. Iniciar Jogo
    printf("Bem-vindo ao Detective Quest: Nivel Mestre\n");
    explorarSalas(mansao, tabelaSuspeitos);

    return 0;
}