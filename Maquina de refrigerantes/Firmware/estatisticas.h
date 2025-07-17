/**
 * @file estatisticas.h
 * @brief Protótipos e estruturas para as funções de geração de estatísticas.
 */

#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "log_vendas.h"

/**
 * @brief Estrutura para armazenar os dados das estatísticas calculadas.
 */
typedef struct {
    int total_vendas;
    float valor_arrecadado;
    float valor_medio;
    int id_mais_vendido;
    int qtd_mais_vendido;
} EstatisticasData;

/**
 * @brief Calcula as estatísticas com base nos logs e preenche uma struct.
 * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @param[out] data Ponteiro para a struct que será preenchida com os dados.
 */
void calcular_estatisticas(Log* logs, EstatisticasData* data);

/**
 * @brief Salva as estatísticas de vendas em um arquivo CSV.
 * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @param[in] filename O nome do arquivo CSV onde as estatísticas serão salvas.
 */
void salvar_estatisticas_csv(Log* logs, const char* filename);

#endif