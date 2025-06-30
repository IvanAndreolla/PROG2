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
    MENU_PRINCIPAL,     /**< Estado do menu principal. */
    SELECAO_PRODUTO,    /**< Estado de seleção de produto. */
    PAGAMENTO,          /**< Estado de pagamento. */
    CONFIGURACAO,       /**< Estado do menu de configuração. */
    SAIR,               /**< Estado final para encerrar a aplicação. */
    VOLTAR              /**< Estado especial para acionar a funcionalidade "voltar". */
} State;

/**
 * @brief Função que implementa a lógica do estado MENU_PRINCIPAL.
 * @return O próximo estado para o qual a máquina deve transitar.
 */
State estado_menu_principal();

/**
 * @brief Função que implementa a lógica do estado SELECAO_PRODUTO.
 * @return O próximo estado para o qual a máquina deve transitar.
 */
State estado_selecao_produto();

/**
 * @brief Função que implementa a lógica do estado PAGAMENTO.
 * @return O próximo estado para o qual a máquina deve transitar.
 */
State estado_pagamento();

/**
 * @brief Função que implementa a lógica do estado CONFIGURACAO.
 * @return O próximo estado para o qual a máquina deve transitar.
 */
State estado_configuracao();

#endif