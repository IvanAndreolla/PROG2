/**
 * @file fila.c
 * @brief Implementação da estrutura de dados de fila circular.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

/**
 * @brief Verifica se a fila está cheia.
 * @param[in] fila Ponteiro para a fila.
 * @return 1 se a fila estiver cheia, 0 caso contrário.
 */
static int fila_cheia(Fila* fila) {
    if (!fila) return 0;
    return fila->tamanho == MAX_PROPAGANDAS;
}

/**
 * @brief Verifica se a fila está vazia.
 * @param[in] fila Ponteiro para a fila.
 * @return 1 se a fila estiver vazia, 0 caso contrário.
 */
static int fila_vazia(Fila* fila) {
    if (!fila) return 1;
    return fila->tamanho == 0;
}

/**
 * @brief Inicializa a fila.
 * @param[out] fila Ponteiro para a fila a ser inicializada.
 */
void inicializar_fila(Fila* fila) {
    if (!fila) return;
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

/**
 * @brief Adiciona uma mensagem ao final da fila (enfileirar).
 * @param[in,out] fila Ponteiro para a fila.
 * @param[in] mensagem Mensagem a ser adicionada.
 */
void enfileirar(Fila* fila, const char* mensagem) {
    if (!fila || !mensagem || fila_cheia(fila)) {
        if (fila_cheia(fila)) fprintf(stderr, "Aviso: Fila de propagandas cheia.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % MAX_PROPAGANDAS;
    strncpy(fila->mensagens[fila->tras], mensagem, TAMANHO_MAX_MSG - 1);
    fila->mensagens[fila->tras][TAMANHO_MAX_MSG - 1] = '\0';
    fila->tamanho++;
}

/**
 * @brief Remove e retorna a mensagem da frente da fila (desenfileirar).
 * @param[in,out] fila Ponteiro para a fila.
 * @return Ponteiro para a mensagem removida, ou NULL se a fila estiver vazia.
 */
const char* desenfileirar(Fila* fila) {
    if (!fila || fila_vazia(fila)) {
        return NULL;
    }
    const char* mensagem = fila->mensagens[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_PROPAGANDAS;
    fila->tamanho--;
    return mensagem;
}

/**
 * @brief Retorna a mensagem na frente da fila sem removê-la.
 * @param[in] fila Ponteiro para a fila.
 * @return Ponteiro para a mensagem da frente, ou NULL se a fila estiver vazia.
 */
const char* frente_fila(Fila* fila) {
    if (!fila || fila_vazia(fila)) {
        return NULL;
    }
    return fila->mensagens[fila->frente];
}