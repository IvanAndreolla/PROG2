/**
 * @file main.c
 * @brief Ponto de entrada principal e loop da aplicação da Máquina de Refrigerantes.
 * * Este arquivo inicializa todas as estruturas de dados, gerencia a máquina de
 * estados finitos (FSM) e controla o fluxo principal da aplicação.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ui.h"
#include "states.h"
#include "lista_produtos.h"
#include "log_vendas.h"
#include "fila.h"
#include "pilha.h"
#include "config.h"

/**
 * @brief Função principal que executa a máquina de refrigerantes.
 * * Inicializa o contexto, as estruturas de dados, carrega os dados de
 * arquivos e entra no loop da máquina de estados. Ao sair, salva os dados
 * e libera a memória alocada.
 * @return 0 em caso de sucesso, 1 em caso de erro de alocação.
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

    PilhaEstados estados_anteriores;
    inicializar_pilha_estados(&estados_anteriores);

    Fila propagandas;
    inicializar_fila(&propagandas);
    enfileirar(&propagandas, "Compre Pureza e refresque seu dia!");
    enfileirar(&propagandas, "Guaraná Jesus: o refrigerante do coração!");
    enfileirar(&propagandas, "Beba Coca-Cola, sinta a emoção!");

    ctx->logs_vendas = carregar_logs("vendas.bin");
    ctx->lista_produtos = carregar_produtos_csv("produtos.csv");
    if (!ctx->lista_produtos) {
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 1, "Pureza", 1.50, 10);
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 2, "Guaraná Jesus", 1.50, 10);
    }
    
    inicializar_interface(&propagandas);

    State estado_atual = MENU_PRINCIPAL;
    int running = 1;

    time_t ultima_atualizacao_propaganda = time(NULL);
    const char* propaganda_atual = frente_fila(&propagandas);

    while (running) {
        limpar_tela();

        time_t agora = time(NULL);
        if (difftime(agora, ultima_atualizacao_propaganda) >= 60.0) {
            const char* propaganda_movida = desenfileirar(&propagandas);
            if (propaganda_movida) {
                char buffer_temp[TAMANHO_MAX_MSG];
                strncpy(buffer_temp, propaganda_movida, TAMANHO_MAX_MSG);
                buffer_temp[TAMANHO_MAX_MSG - 1] = '\0';
                enfileirar(&propagandas, buffer_temp);
                propaganda_atual = frente_fila(&propagandas);
            }
            ultima_atualizacao_propaganda = agora;
        }

        if (propaganda_atual) {
            printf("Propaganda: %s\n\n", propaganda_atual);
        } else {
            printf("Nenhuma propaganda disponível.\n\n");
        }
        
        State proximo_estado;
        switch (estado_atual) {
            case MENU_PRINCIPAL:
                proximo_estado = estado_menu_principal(ctx);
                break;
            case SELECAO_PRODUTO:
                proximo_estado = estado_selecao_produto(ctx);
                break;
            case PAGAMENTO:
                proximo_estado = estado_pagamento(ctx);
                break;
            case CONFIGURACAO:
                proximo_estado = estado_configuracao(ctx);
                break;
            case SAIR:
                running = 0;
                continue;
            default:
                proximo_estado = MENU_PRINCIPAL;
        }

        if (proximo_estado == VOLTAR) {
            if (!pilha_estados_vazia(&estados_anteriores)) {
                estado_atual = desempilhar_estado(&estados_anteriores);
            } else {
                estado_atual = MENU_PRINCIPAL;
            }
        } else if (proximo_estado != estado_atual) {
            empilhar_estado(&estados_anteriores, estado_atual);
            estado_atual = proximo_estado;
        } else {
            if (estado_atual == MENU_PRINCIPAL) {
                 inicializar_pilha_estados(&estados_anteriores);
            }
        }
    }

    salvar_produtos_csv(ctx->lista_produtos, "produtos.csv");
    salvar_logs("vendas.bin", ctx->logs_vendas);
    
    liberar_logs(ctx->logs_vendas);
    liberar_lista_produtos(ctx->lista_produtos);
    free(ctx);

    limpar_tela();
    printf("\nSistema encerrado. Até a próxima!\n\n");

    return 0;
}