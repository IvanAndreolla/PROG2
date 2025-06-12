#include <stdio.h>
#include <string.h>
#include "estatisticas.h"
#include "lista_produtos.h"

/**
 * @brief Gera e exibe estatísticas com base nos logs de vendas.
 * @param logs Ponteiro para a lista de logs de vendas.
 */
void gerar_estatisticas(Log* logs) {
    if (!logs) {
        printf("Nenhum log de vendas disponível.\n");
        return;
    }

    int total_vendas = 0;
    float valor_arrecadado = 0.0f;
    int produtos[100] = {0}; // Suporta até 100 IDs de produtos
    int max_vendas = 0;
    int id_mais_vendido = -1;

    Log* atual = logs;
    while (atual) {
        total_vendas++;
        valor_arrecadado += atual->valor_pago;
        produtos[atual->produto_id]++;
        if (produtos[atual->produto_id] > max_vendas) {
            max_vendas = produtos[atual->produto_id];
            id_mais_vendido = atual->produto_id;
        }
        atual = atual->prox;
    }

    float valor_medio = total_vendas > 0 ? valor_arrecadado / total_vendas : 0.0f;
    char nome_mais_vendido[50] = "N/A";
    if (id_mais_vendido != -1) {
        // Supõe que a lista de produtos está acessível globalmente ou via ctx
        // Aqui, apenas usamos um placeholder, já que ctx não é passado
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
 * @brief Salva as estatísticas em um arquivo CSV.
 * @param logs Ponteiro para a lista de logs de vendas.
 * @param filename Nome do arquivo CSV.
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
        valor_arrecadado += atual->valor_pago;
        produtos[atual->produto_id]++;
        if (produtos[atual->produto_id] > max_vendas) {
            max_vendas = produtos[atual->produto_id];
            id_mais_vendido = atual->produto_id;
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