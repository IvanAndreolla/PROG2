#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "log_vendas.h"

/**
 * @brief Gera e exibe estatísticas com base nos logs de vendas.
 * @param logs Ponteiro para a lista de logs de vendas.
 */
void gerar_estatisticas(Log* logs);

/**
 * @brief Salva as estatísticas em um arquivo CSV.
 * @param logs Ponteiro para a lista de logs de vendas.
 * @param filename Nome do arquivo CSV.
 */
void salvar_estatisticas_csv(Log* logs, const char* filename);

#endif