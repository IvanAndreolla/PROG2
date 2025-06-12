#include "pilha.h"

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
