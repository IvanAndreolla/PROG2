/**
 * @file pilha.c
 * @brief Implementação da estrutura de dados de pilha.
 */

#include "pilha.h"
#include <stdio.h>

/* --- Funções da Pilha de Moedas (float) --- */

void inicializar_pilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilha_vazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilha_cheia(Pilha *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

void empilhar(Pilha *pilha, float valor) {
    if (!pilha_cheia(pilha)) {
        pilha->valores[++(pilha->topo)] = valor;
    }
}

float desempilhar(Pilha *pilha) {
    if (!pilha_vazia(pilha)) {
        return pilha->valores[(pilha->topo)--];
    }
    return 0.0f;
}

/* --- Funções da Pilha de Estados (enum/int) --- */

void inicializar_pilha_estados(PilhaEstados *pilha) {
    pilha->topo = -1;
}

int pilha_estados_vazia(PilhaEstados *pilha) {
    return pilha->topo == -1;
}

int pilha_estados_cheia(PilhaEstados *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

void empilhar_estado(PilhaEstados *pilha, State estado) {
    if (!pilha_estados_cheia(pilha)) {
        pilha->estados[++(pilha->topo)] = estado;
    } else {
        fprintf(stderr, "Aviso: Pilha de estados cheia.\n");
    }
}

State desempilhar_estado(PilhaEstados *pilha) {
    if (!pilha_estados_vazia(pilha)) {
        return pilha->estados[(pilha->topo)--];
    }
    return MENU_PRINCIPAL;
}