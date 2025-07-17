/**
 * @file states.h
 * @brief Definição dos estados da máquina e protótipos das funções de estado.
 */

#ifndef STATES_H
#define STATES_H

/**
 * @brief Enumeração dos possíveis estados da máquina de estados finitos (FSM).
 */
typedef enum {
    MENU_PRINCIPAL,
    SELECAO_PRODUTO,
    PAGAMENTO,
    SAIR,
    VOLTAR,
    ESTATISTICAS,
    TELA_SENHA,
    CONFIGURACAO_MENU,
    CONFIG_LISTAR_PRODUTOS,
    CONFIG_ADICIONAR_PRODUTO,
    CONFIG_REMOVER_PRODUTO,
    CONFIG_ATUALIZAR_ESTOQUE,
    ANIMACAO_QUEDA,
    ANIMACAO_PORTA,
    ANIMACAO_ENTREGA
} State;

// Protótipos das funções de estado
State estado_menu_principal();
State estado_selecao_produto();
State estado_pagamento();
State estado_configuracao();

#endif