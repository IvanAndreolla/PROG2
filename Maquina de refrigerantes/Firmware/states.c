/**
 * @file states.c
 * @brief Implementação das funções de lógica para cada estado da FSM.
 * @note Na versão Allegro, a maior parte da lógica foi movida para o main.c.
 * Estas funções são mantidas para compatibilidade, mas estão vazias.
 */

#include "states.h"
#include "config.h" // Incluído para a definição de Contexto


State estado_menu_principal(Contexto *ctx) {
    // Lógica agora no main.c
    return MENU_PRINCIPAL;
}

State estado_selecao_produto(Contexto *ctx) {
    // Lógica agora no main.c
    return SELECAO_PRODUTO;
}

State estado_pagamento(Contexto *ctx) {
    // Lógica agora no main.c
    return PAGAMENTO;
}

State estado_configuracao(Contexto *ctx) {
    // Lógica agora no main.c
    return CONFIGURACAO_MENU;
}