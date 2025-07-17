/**
 * @file estatisticas.c
 * @brief Implementação das funções de geração de estatísticas de vendas.
 */

#include <stdio.h>
#include <string.h>
#include "estatisticas.h"

/**
 * @brief Calcula as estatísticas com base nos logs e preenche uma struct.
 * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @param[out] data Ponteiro para a struct que será preenchida com os dados.
 */
void calcular_estatisticas(Log* logs, EstatisticasData* data) {
    // Inicializa a struct com valores padrão
    data->total_vendas = 0;
    data->valor_arrecadado = 0.0f;
    data->valor_medio = 0.0f;
    data->id_mais_vendido = -1;
    data->qtd_mais_vendido = 0;

    if (!logs) {
        return;
    }

    int produtos[100] = {0};

    Log* atual = logs;
    while (atual) {
        data->total_vendas++;
        data->valor_arrecadado += (atual->valor_pago - atual->troco);
        
        if (atual->produto_id >= 0 && atual->produto_id < 100) {
            produtos[atual->produto_id]++;
            if (produtos[atual->produto_id] > data->qtd_mais_vendido) {
                data->qtd_mais_vendido = produtos[atual->produto_id];
                data->id_mais_vendido = atual->produto_id;
            }
        }
        atual = atual->prox;
    }

    if (data->total_vendas > 0) {
        data->valor_medio = data->valor_arrecadado / data->total_vendas;
    }
}


/**
 * @brief Salva as estatísticas de vendas em um arquivo CSV.
 * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @param[in] filename O nome do arquivo CSV onde as estatísticas serão salvas.
 */
void salvar_estatisticas_csv(Log* logs, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    EstatisticasData data;
    calcular_estatisticas(logs, &data); // Reutiliza a função de cálculo

    fprintf(f, "TotalVendas,ValorArrecadado,ValorMedio,ProdutoMaisVendidoID,ProdutoMaisVendidoQtd\n");
    fprintf(f, "%d,%.2f,%.2f,%d,%d\n", data.total_vendas, data.valor_arrecadado, data.valor_medio, data.id_mais_vendido, data.qtd_mais_vendido);

    fclose(f);
}