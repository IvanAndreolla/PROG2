#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "states.h"
#include "lista_produtos.h"
#include "log_vendas.h"
#include "fila.h"

/**
 * @brief Estrutura para armazenar o contexto da aplicação.
 */
typedef struct {
    Produto* lista_produtos;     /**< Lista de produtos disponíveis. */
    Log* logs_vendas;           /**< Lista de logs de vendas. */
    Produto* produto_selecionado; /**< Produto selecionado para compra. */
} Contexto;

/**
 * @brief Função principal da máquina de refrigerantes.
 * @return Código de saída do programa.
 */
int main() {
    Contexto* ctx = (Contexto*)malloc(sizeof(Contexto));
    if (!ctx) {
        fprintf(stderr, "Erro de alocação de memória para Contexto.\n");
        return 1;
    }
    ctx->lista_produtos = NULL;
    ctx->logs_vendas = NULL;
    ctx->produto_selecionado = NULL;

    Fila propagandas;
    int running = 1;
    State estado_atual = MENU_PRINCIPAL;

    inicializar_fila(&propagandas);
    enfileirar(&propagandas, "Compre Pureza e refresque seu dia!");
    enfileirar(&propagandas, "Guaraná Jesus: o refrigerante do coração!");
    inicializar_interface(&propagandas);

    ctx->logs_vendas = carregar_logs("vendas.bin");
    ctx->lista_produtos = carregar_produtos_csv("produtos.csv");
    if (!ctx->lista_produtos) {
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 1, "Pureza", 1.50, 10);
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 2, "Guaraná Jesus", 1.50, 10);
    }

    while (running) {
        limpar_tela();
        const char* propaganda = frente_fila(&propagandas);
        if (propaganda) {
            printf("Propaganda: %s\n\n", propaganda);
            desenfileirar(&propagandas);
            enfileirar(&propagandas, propaganda);
        } else {
            printf("Nenhuma propaganda disponível.\n\n");
        }
        switch (estado_atual) {
            case MENU_PRINCIPAL:
                estado_atual = estado_menu_principal(ctx);
                break;
            case SELECAO_PRODUTO:
                estado_atual = estado_selecao_produto(ctx);
                break;
            case PAGAMENTO:
                estado_atual = estado_pagamento(ctx);
                break;
            case CONFIGURACAO:
                estado_atual = estado_configuracao(ctx);
                break;
            case SAIR:
                running = 0;
                break;
        }
    }

    salvar_produtos_csv(ctx->lista_produtos, "produtos.csv");
    salvar_logs("vendas.bin", ctx->logs_vendas);
    liberar_logs(ctx->logs_vendas);
    liberar_lista_produtos(ctx->lista_produtos);

    limpar_tela();
    printf("\nSistema encerrado. Até a próxima!\n\n");

    free(ctx);
    return 0;
}