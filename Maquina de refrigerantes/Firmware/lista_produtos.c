/**
 * @file lista_produtos.c
 * @brief Implementação das funções de gerenciamento da lista de produtos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_produtos.h"

/**
 * @brief Adiciona um novo produto ao início da lista duplamente encadeada.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] id ID do novo produto.
 * @param[in] nome Nome do novo produto.
 * @param[in] preco Preço do novo produto.
 * @param[in] estoque Quantidade em estoque do novo produto.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* adicionar_produto(Produto* head, int id, const char* nome, float preco, int estoque) {
    Produto* novo = (Produto*)malloc(sizeof(Produto));
    if (!novo) {
        fprintf(stderr, "Erro de alocação de memória para novo produto.\n");
        return head;
    }
    novo->id = id;
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->preco = preco;
    novo->estoque = estoque;
    novo->prox = head;
    novo->ant = NULL;
    if (head) {
        head->ant = novo;
    }
    return novo;
}

/**
 * @brief Remove um produto da lista pelo seu ID.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] id ID do produto a ser removido.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* remover_produto(Produto* head, int id) {
    Produto* atual = head;
    while (atual) {
        if (atual->id == id) {
            if (atual->ant) {
                atual->ant->prox = atual->prox;
            } else {
                head = atual->prox;
            }
            if (atual->prox) {
                atual->prox->ant = atual->ant;
            }
            printf("Produto com ID %d removido.\n", id);
            free(atual);
            return head;
        }
        atual = atual->prox;
    }
    printf("Produto com ID %d não encontrado para remoção.\n", id);
    return head;
}

/**
 * @brief Busca um produto na lista pelo seu ID.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] id ID do produto a ser buscado.
 * @return Ponteiro para o nó do produto encontrado, ou NULL se não for encontrado.
 */
Produto* buscar_produto(Produto* head, int id) {
    Produto* atual = head;
    while (atual) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/**
 * @brief Lista todos os produtos da lista no terminal.
 * @param[in] head Ponteiro para a cabeça da lista.
 */
void listar_produtos(Produto* head) {
    if (!head) {
        printf("Nenhum produto disponível.\n");
        return;
    }
    printf("\n--- Produtos Disponíveis ---\n");
    Produto* temp = head;
    while (temp) {
        printf("ID: %d | Nome: %s | Preço: R$%.2f | Estoque: %d\n",
               temp->id, temp->nome, temp->preco, temp->estoque);
        temp = temp->prox;
    }
}

/**
 * @brief Libera toda a memória alocada para a lista de produtos.
 * @param[in] head Ponteiro para a cabeça da lista a ser liberada.
 */
void liberar_lista_produtos(Produto* head) {
    Produto* temp;
    while (head) {
        temp = head;
        head = head->prox;
        free(temp);
    }
}

/**
 * @brief Salva a lista de produtos em um arquivo no formato CSV.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] filename Nome do arquivo CSV de destino.
 */
void salvar_produtos_csv(Produto* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }
    fprintf(f, "ID,Nome,Preco,Estoque\n");
    Produto* temp = head;
    while (temp) {
        fprintf(f, "%d,%s,%.2f,%d\n", temp->id, temp->nome, temp->preco, temp->estoque);
        temp = temp->prox;
    }
    fclose(f);
}

/**
 * @brief Carrega uma lista de produtos a partir de um arquivo CSV.
 * @param[in] filename Nome do arquivo CSV a ser lido.
 * @return Ponteiro para a cabeça da nova lista de produtos carregada.
 */
Produto* carregar_produtos_csv(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        //fprintf(stderr, "Aviso: Arquivo de produtos '%s' não encontrado. Começando com lista vazia.\n", filename);
        return NULL;
    }
    Produto* head = NULL;
    char linha[256];
    fgets(linha, sizeof(linha), f);
    while (fgets(linha, sizeof(linha), f)) {
        int id, estoque;
        char nome[50];
        float preco;
        if (sscanf(linha, "%d,%49[^,],%f,%d", &id, nome, &preco, &estoque) == 4) {
            head = adicionar_produto(head, id, nome, preco, estoque);
        }
    }
    fclose(f);
    return head;
}