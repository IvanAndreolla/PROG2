/**
 * @file pilha.h
 * @brief Definições e protótipos para estruturas de dados de pilha.
 * * Contém implementações para uma pilha de floats (usada para moedas) e uma
 * pilha de estados (usada para o histórico de navegação).
 */

#ifndef PILHA_H
#define PILHA_H

#include "states.h" 

#define MAX_PILHA 100

/**
 * @brief Estrutura para a pilha que armazena as moedas (valores float).
 */
typedef struct {
    float valores[MAX_PILHA]; /**< Array para armazenar os valores. */
    int topo;                 /**< Índice do topo da pilha. */
} Pilha;

void inicializar_pilha(Pilha *pilha);
int pilha_vazia(Pilha *pilha);
int pilha_cheia(Pilha *pilha);
void empilhar(Pilha *pilha, float valor);
float desempilhar(Pilha *pilha);

/**
 * @brief Estrutura para a pilha que armazena o histórico de estados da FSM.
 */
typedef struct {
    State estados[MAX_PILHA]; /**< Array para armazenar os estados. */
    int topo;                 /**< Índice do topo da pilha. */
} PilhaEstados;

void inicializar_pilha_estados(PilhaEstados *pilha);
int pilha_estados_vazia(PilhaEstados *pilha);
int pilha_estados_cheia(PilhaEstados *pilha);
void empilhar_estado(PilhaEstados *pilha, State estado);
State desempilhar_estado(PilhaEstados *pilha);

#endif