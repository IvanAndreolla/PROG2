/**
 * @file states.c
 * @brief Implementação das funções de lógica para cada estado da FSM.
 */

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
 * @brief Lê um número inteiro da entrada padrão com validação.
 * * Função de ajuda local para este módulo.
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
 * @brief Lógica do estado do menu principal.
 * @param[in] ctx Contexto da aplicação (usado para acessar logs).
 * @return Próximo estado da FSM.
 */
State estado_menu_principal(Contexto *ctx) {
    printf("1. Selecionar produto\n2. Configurações\n3. Ver estatísticas\n4. Sair\nEscolha: ");
    int op = ler_inteiro();

    switch (op) {
        case 1: return SELECAO_PRODUTO; // <-- CORRIGIDO
        case 2: return CONFIGURACAO;
        case 3: gerar_estatisticas(ctx->logs_vendas); pausar(); return MENU_PRINCIPAL;
        case 4: return SAIR;
        default: printf("Opção inválida.\n"); pausar(); return MENU_PRINCIPAL;
    }
}

/**
 * @brief Lógica do estado de seleção de produto.
 * @param[in,out] ctx Contexto da aplicação, o produto selecionado será armazenado aqui.
 * @return Próximo estado da FSM.
 */
State estado_selecao_produto(Contexto *ctx) {
    listar_produtos(ctx->lista_produtos);
    printf("Digite o ID do produto (ou 0 para voltar): ");
    int id = ler_inteiro();

    if (id == 0) {
        return VOLTAR;
    }

    Produto* produto = buscar_produto(ctx->lista_produtos, id);
    if (!produto) {
        printf("Produto não encontrado.\n");
        pausar();
        return SELECAO_PRODUTO;
    }
    if (produto->estoque <= 0) {
        printf("Produto fora de estoque.\n");
        pausar();
        return SELECAO_PRODUTO;
    }
    printf("Produto selecionado: %s (R$%.2f)\n", produto->nome, produto->preco);
    ctx->produto_selecionado = produto;
    pausar();
    return PAGAMENTO;
}

/**
 * @brief Lógica do estado de pagamento.
 * @param[in,out] ctx Contexto da aplicação, usado para obter o produto, decrementar estoque e registrar o log.
 * @return Próximo estado da FSM.
 */
State estado_pagamento(Contexto *ctx) {
    if (ctx->produto_selecionado == NULL) {
        printf("Nenhum produto selecionado. Retornando ao menu.\n");
        pausar();
        return MENU_PRINCIPAL;
    }

    printf("Produto: %s | Preço: R$%.2f\n", ctx->produto_selecionado->nome, ctx->produto_selecionado->preco);
    
    float valor_inserido = 0.0f;
    int status_pagamento = inserir_moedas(&valor_inserido);

    if (status_pagamento == 0) {
        if (valor_inserido > 0) {
            printf("Compra cancelada. Valor de R$%.2f devolvido.\n", valor_inserido);
        } else {
            printf("Compra cancelada.\n");
        }
        ctx->produto_selecionado = NULL;
        pausar();
        return VOLTAR; 
    }

    float troco = calcular_troco(valor_inserido, ctx->produto_selecionado->preco);
    if (troco < 0) {
        printf("Valor insuficiente. Devolvendo R$%.2f\n", valor_inserido);
        pausar();
        return PAGAMENTO;
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
 * @brief Lógica do estado de configuração.
 * @param[in,out] ctx Contexto da aplicação, pode ser modificado pelo menu de configuração.
 * @return Próximo estado da FSM (sempre MENU_PRINCIPAL ao sair do menu de config).
 */
State estado_configuracao(Contexto *ctx) {
    menu_configuracao(ctx);
    return MENU_PRINCIPAL;
}