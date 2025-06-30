/**
 * @file estatisticas.h
 * @brief Protótipos para as funções de geração de estatísticas de vendas.
 */

#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "log_vendas.h"

/**
 * @brief Gera e exibe estatísticas com base nos logs de vendas.
 * * Calcula o total de vendas, valor arrecadado, valor médio e o produto mais vendido.
 * Também invoca a função para salvar as estatísticas em um arquivo.
 * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 */
void gerar_estatisticas(Log* logs);

/**
 * @brief Salva as estatísticas de vendas em um arquivo CSV.
 * * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @param[in] filename O nome do arquivo CSV onde as estatísticas serão salvas.
 */
void salvar_estatisticas_csv(Log* logs, const char* filename);

#endif