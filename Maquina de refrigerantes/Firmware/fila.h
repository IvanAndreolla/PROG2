#ifndef FILA_H
#define FILA_H

#define MAX_PROPAGANDAS 10
#define TAMANHO_MAX_MSG 100

/**
 * @brief Estrutura para representar uma fila circular de propagandas.
 */
typedef struct Fila {
    char mensagens[MAX_PROPAGANDAS][TAMANHO_MAX_MSG]; /**< Array de mensagens. */
    int frente;                                      /**< Índice da frente da fila. */
    int tras;                                        /**< Índice da traseira da fila. */
    int tamanho;                                     /**< Número de elementos na fila. */
} Fila;

/**
 * @brief Inicializa a fila de propagandas.
 * @param fila Ponteiro para a fila.
 */
void inicializar_fila(Fila* fila);

/**
 * @brief Adiciona uma mensagem à fila.
 * @param fila Ponteiro para a fila.
 * @param mensagem Mensagem a ser adicionada.
 */
void enfileirar(Fila* fila, const char* mensagem);

/**
 * @brief Remove e retorna a mensagem da frente da fila.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para a mensagem ou NULL se vazia.
 */
const char* desenfileirar(Fila* fila);

/**
 * @brief Retorna a mensagem na frente da fila sem removê-la.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para a mensagem ou NULL se vazia.
 */
const char* frente_fila(Fila* fila);

#endif