/**
 * @file log_vendas.h
 * @brief Definições e protótipos para a lista simplesmente encadeada de logs de vendas.
 */

#ifndef LOG_VENDAS_H
#define LOG_VENDAS_H

/**
 * @brief Estrutura para representar um registro (log) de uma venda.
 */
typedef struct Log {
    char timestamp[20]; /**< Data e hora da transação. */
    int produto_id;     /**< ID do produto vendido. */
    float valor_pago;   /**< Valor total pago pelo cliente. */
    float troco;        /**< Valor do troco devolvido. */
    struct Log* prox;   /**< Ponteiro para o próximo log na lista. */
} Log;

/**
 * @brief Adiciona um novo log de venda ao início da lista.
 * @param[in] head Ponteiro para a cabeça da lista de logs.
 * @param[in] timestamp String com a data e hora da venda.
 * @param[in] produto_id ID do produto vendido.
 * @param[in] valor_pago Valor inserido pelo cliente.
 * @param[in] troco Valor do troco.
 * @return Ponteiro para a nova cabeça da lista de logs.
 */
Log* adicionar_log(Log* head, const char* timestamp, int produto_id, float valor_pago, float troco);

/**
 * @brief Salva a lista de logs em um arquivo binário.
 * @param[in] filename Nome do arquivo de destino.
 * @param[in] head Ponteiro para a cabeça da lista de logs.
 */
void salvar_logs(const char* filename, Log* head);

/**
 * @brief Carrega a lista de logs a partir de um arquivo binário.
 * @param[in] filename Nome do arquivo a ser lido.
 * @return Ponteiro para a cabeça da nova lista de logs carregada.
 */
Log* carregar_logs(const char* filename);

/**
 * @brief Libera toda a memória alocada para a lista de logs.
 * @param[in] head Ponteiro para a cabeça da lista a ser liberada.
 */
void liberar_logs(Log* head);

/**
 * @brief Exibe todos os logs de vendas no terminal.
 * @param[in] head Ponteiro para a cabeça da lista de logs.
 */
void exibir_logs(Log* head);

#endif