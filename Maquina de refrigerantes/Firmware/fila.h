/**
 * @file fila.h
 * @brief Definições e protótipos para uma estrutura de dados de fila circular.
 * * Usada para gerenciar as propagandas da máquina.
 */

#ifndef FILA_H
#define FILA_H

#define MAX_PROPAGANDAS 10
#define TAMANHO_MAX_MSG 100

/**
 * @brief Estrutura para representar uma fila circular de propagandas.
 */
typedef struct Fila {
    char mensagens[MAX_PROPAGANDAS][TAMANHO_MAX_MSG]; /**< Array de strings para as mensagens. */
    int frente;                                      /**< Índice da frente da fila. */
    int tras;                                        /**< Índice da traseira da fila. */
    int tamanho;                                     /**< Número de elementos na fila. */
} Fila;

/**
 * @brief Inicializa a fila.
 * @param[out] fila Ponteiro para a fila a ser inicializada.
 */
void inicializar_fila(Fila* fila);

/**
 * @brief Adiciona uma mensagem ao final da fila (enfileirar).
 * @param[in,out] fila Ponteiro para a fila.
 * @param[in] mensagem Mensagem a ser adicionada.
 */
void enfileirar(Fila* fila, const char* mensagem);

/**
 * @brief Remove e retorna a mensagem da frente da fila (desenfileirar).
 * @param[in,out] fila Ponteiro para a fila.
 * @return Ponteiro para a mensagem removida, ou NULL se a fila estiver vazia.
 */
const char* desenfileirar(Fila* fila);

/**
 * @brief Retorna a mensagem na frente da fila sem removê-la.
 * @param[in] fila Ponteiro para a fila.
 * @return Ponteiro para a mensagem da frente, ou NULL se a fila estiver vazia.
 */
const char* frente_fila(Fila* fila);

#endif