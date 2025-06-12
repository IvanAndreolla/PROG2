#ifndef PILHA_H
#define PILHA_H

#define MAX_PILHA 100

typedef struct {
    float valores[MAX_PILHA];
    int topo;
} Pilha;

void inicializar_pilha(Pilha *pilha);
int pilha_vazia(Pilha *pilha);
int pilha_cheia(Pilha *pilha);
void empilhar(Pilha *pilha, float valor);
float desempilhar(Pilha *pilha);

#endif
