#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_produtos.h"

Produto* adicionar_produto(Produto* head, int id, const char* nome, float preco, int estoque) {
    Produto* novo = (Produto*)malloc(sizeof(Produto));
    if (!novo) {
        fprintf(stderr, "Erro de alocação de memória.\n");
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
            free(atual);
            return head;
        }
        atual = atual->prox;
    }
    printf("Produto com ID %d não encontrado.\n", id);
    return head;
}

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

void listar_produtos(Produto* head) {
    if (!head) {
        printf("Nenhum produto disponível.\n");
        return;
    }
    printf("\n--- Produtos Disponíveis ---\n");
    while (head) {
        printf("ID: %d | Nome: %s | Preço: R$%.2f | Estoque: %d\n",
               head->id, head->nome, head->preco, head->estoque);
        head = head->prox;
    }
}

void liberar_lista_produtos(Produto* head) {
    Produto* temp;
    while (head) {
        temp = head;
        head = head->prox;
        free(temp);
    }
}

void salvar_produtos_csv(Produto* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }
    fprintf(f, "ID,Nome,Preco,Estoque\n");
    while (head) {
        fprintf(f, "%d,%s,%.2f,%d\n", head->id, head->nome, head->preco, head->estoque);
        head = head->prox;
    }
    fclose(f);
}

Produto* carregar_produtos_csv(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para leitura.\n", filename);
        return NULL;
    }
    Produto* head = NULL;
    char linha[256];
    fgets(linha, sizeof(linha), f); // Ignora cabeçalho
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