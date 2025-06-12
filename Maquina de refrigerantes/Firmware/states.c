#include <stdio.h>
#include <time.h>
#include "config.h"
#include "states.h"
#include "ui.h"
#include "lista_produtos.h"
#include "pagamento.h"
#include "log_vendas.h"
#include "estatisticas.h"

/**
 * @brief Lê um número inteiro com validação.
 * @return O número inteiro lido.
 */
static int ler_inteiro() {
    char buffer[50];
    int valor;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &valor) == 1) {
                return valor;
            }
        }
        printf("Entrada inválida. Digite um número: ");
    }
}

/**
 * @brief Estado do menu principal.
 * @param ctx Contexto da aplicação.
 * @return Próximo estado.
 */
State estado_menu_principal(Contexto *ctx) {
    printf("1. Selecionar produto\n2. Configurações\n3. Ver estatísticas\n4. Sair\nEscolha: ");
    int op = ler_inteiro();

    switch (op) {
        case 1: return SELECAO_PRODUTO;
        case 2: return CONFIGURACAO;
        case 3: gerar_estatisticas(ctx->logs_vendas); pausar(); return MENU_PRINCIPAL;
        case 4: return SAIR;
        default: return MENU_PRINCIPAL;
    }
}

/**
 * @brief Estado de seleção de produto.
 * @param ctx Contexto da aplicação.
 * @return Próximo estado.
 */
State estado_selecao_produto(Contexto *ctx) {
    listar_produtos(ctx->lista_produtos);
    printf("Digite o ID do produto: ");
    int id = ler_inteiro();
    Produto* produto = buscar_produto(ctx->lista_produtos, id);
    if (!produto) {
        printf("Produto não encontrado.\n");
        pausar();
        return MENU_PRINCIPAL;
    }
    if (produto->estoque <= 0) {
        printf("Produto fora de estoque.\n");
        pausar();
        return MENU_PRINCIPAL;
    }
    printf("Produto selecionado: %s (R$%.2f)\n", produto->nome, produto->preco);
    ctx->produto_selecionado = produto;
    pausar();
    return PAGAMENTO;
}

/**
 * @brief Estado de pagamento.
 * @param ctx Contexto da aplicação.
 * @return Próximo estado.
 */
State estado_pagamento(Contexto *ctx) {
    if (ctx->produto_selecionado == NULL) {
        printf("Nenhum produto selecionado.\n");
        pausar();
        return MENU_PRINCIPAL;
    }
    printf("Produto: %s | Preço: R$%.2f\n", ctx->produto_selecionado->nome, ctx->produto_selecionado->preco);
    float valor_inserido = inserir_moedas();
    float troco = calcular_troco(valor_inserido, ctx->produto_selecionado->preco);
    if (troco < 0) {
        printf("Valor insuficiente. Devolvendo R$%.2f\n", valor_inserido);
        pausar();
        return MENU_PRINCIPAL;
    }
    ctx->produto_selecionado->estoque--;
    char timestamp[20];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    ctx->logs_vendas = adicionar_log(ctx->logs_vendas, timestamp, ctx->produto_selecionado->id, valor_inserido, troco);
    printf("Compra realizada! Troco: R$%.2f\n", troco);
    ctx->produto_selecionado = NULL;
    pausar();
    return MENU_PRINCIPAL;
}

/**
 * @brief Estado de configuração.
 * @param ctx Contexto da aplicação.
 * @return Próximo estado.
 */
State estado_configuracao(Contexto *ctx) {
    menu_configuracao(ctx);
    pausar();
    return MENU_PRINCIPAL;
}