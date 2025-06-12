#ifndef STATES_H
#define STATES_H

typedef enum {
    MENU_PRINCIPAL,
    SELECAO_PRODUTO,
    PAGAMENTO,
    CONFIGURACAO,
    SAIR
} State;

State estado_menu_principal();
State estado_selecao_produto();
State estado_pagamento();
State estado_configuracao();

#endif
