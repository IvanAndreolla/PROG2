/**
 * @file estatisticas.c
 * @brief Implementação das funções de geração de estatísticas de vendas.
 */

#include <stdio.h>
#include <string.h>
#include "estatisticas.h"
#include "lista_produtos.h" // Necessário para buscar nomes de produtos no futuro.

/**
 * @brief Gera e exibe estatísticas com base nos logs de vendas.
 * * Calcula o total de vendas, valor arrecadado, valor médio e o produto mais vendido.
 * Também invoca a função para salvar as estatísticas em um arquivo.
 * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @note Atualmente, o nome do produto mais vendido é um placeholder. Uma
 * futura implementação poderia usar o contexto para buscar o nome real
 * do produto a partir da lista de produtos.
 */
void gerar_estatisticas(Log* logs) {
    if (!logs) {
        printf("Nenhum log de vendas disponível.\n");
        return;
    }

    int total_vendas = 0;
    float valor_arrecadado = 0.0f;
    int produtos[100] = {0};
    int max_vendas = 0;
    int id_mais_vendido = -1;

    Log* atual = logs;
    while (atual) {
        total_vendas++;
        valor_arrecadado += atual->produto_id;
        if(atual->produto_id < 100 && atual->produto_id >= 0) {
            produtos[atual->produto_id]++;
            if (produtos[atual->produto_id] > max_vendas) {
                max_vendas = produtos[atual->produto_id];
                id_mais_vendido = atual->produto_id;
            }
        }
        atual = atual->prox;
    }

    float valor_medio = total_vendas > 0 ? valor_arrecadado / total_vendas : 0.0f;
    char nome_mais_vendido[50] = "N/A";
    if (id_mais_vendido != -1) {
        snprintf(nome_mais_vendido, sizeof(nome_mais_vendido), "Produto ID %d", id_mais_vendido);
    }

    printf("\n--- Estatísticas de Vendas ---\n");
    printf("Total de vendas: %d\n", total_vendas);
    printf("Valor arrecadado: R$%.2f\n", valor_arrecadado);
    printf("Valor médio por venda: R$%.2f\n", valor_medio);
    printf("Produto mais vendido: %s (%d vendas)\n", nome_mais_vendido, max_vendas);

    salvar_estatisticas_csv(logs, "estatisticas.csv");
}

/**
 * @brief Salva as estatísticas de vendas em um arquivo CSV.
 * * @param[in] logs Ponteiro para a cabeça da lista de logs de vendas.
 * @param[in] filename O nome do arquivo CSV onde as estatísticas serão salvas.
 */
void salvar_estatisticas_csv(Log* logs, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    int total_vendas = 0;
    float valor_arrecadado = 0.0f;
    int produtos[100] = {0};
    int max_vendas = 0;
    int id_mais_vendido = -1;

    Log* atual = logs;
    while (atual) {
        total_vendas++;
        valor_arrecadado += atual->valor_pago - atual->troco;
        if(atual->produto_id < 100 && atual->produto_id >= 0) {
            produtos[atual->produto_id]++;
            if (produtos[atual->produto_id] > max_vendas) {
                max_vendas = produtos[atual->produto_id];
                id_mais_vendido = atual->produto_id;
            }
        }
        atual = atual->prox;
    }

    float valor_medio = total_vendas > 0 ? valor_arrecadado / total_vendas : 0.0f;
    char nome_mais_vendido[50] = "N/A";
    if (id_mais_vendido != -1) {
        snprintf(nome_mais_vendido, sizeof(nome_mais_vendido), "Produto ID %d", id_mais_vendido);
    }

    fprintf(f, "TotalVendas,ValorArrecadado,ValorMedio,ProdutoMaisVendidoID,ProdutoMaisVendidoNome\n");
    fprintf(f, "%d,%.2f,%.2f,%d,%s\n", total_vendas, valor_arrecadado, valor_medio, id_mais_vendido, nome_mais_vendido);

    fclose(f);
    printf("Estatísticas salvas em %s.\n", filename);
}