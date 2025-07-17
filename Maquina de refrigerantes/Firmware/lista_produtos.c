/**
 * @file lista_produtos.c
 * @brief Implementação das funções de gerenciamento da lista de produtos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include "lista_produtos.h"

/**
 * @brief Adiciona um novo produto ao início da lista duplamente encadeada.
 */
Produto* adicionar_produto(Produto* head, int id, const char* nome, float preco, int estoque, const char* img_path, float pos_x, float pos_y) {
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

    strncpy(novo->imagem_path, img_path, sizeof(novo->imagem_path) - 1);
    novo->imagem_path[sizeof(novo->imagem_path) - 1] = '\0';
    novo->pos_x_inicial = pos_x;
    novo->pos_y_inicial = pos_y;

    novo->imagem_animacao = al_load_bitmap(img_path);
    if (!novo->imagem_animacao) {
        fprintf(stderr, "Aviso: Falha ao carregar a imagem de animacao: %s\n", img_path);
    }
    
    novo->prox = head;
    novo->ant = NULL;
    if (head) {
        head->ant = novo;
    }
    return novo;
}

/**
 * @brief Remove um produto da lista pelo seu ID.
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

            // Libera a memória da imagem da animação antes de liberar o nó
            if (atual->imagem_animacao) {
                al_destroy_bitmap(atual->imagem_animacao);
            }
            free(atual);
            printf("Produto com ID %d removido.\n", id);
            return head;
        }
        atual = atual->prox;
    }
    printf("Produto com ID %d não encontrado para remoção.\n", id);
    return head;
}

/**
 * @brief Busca um produto na lista pelo seu ID.
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
 */
void listar_produtos(Produto* head) {
    if (!head) {
        printf("Nenhum produto disponível.\n");
        return;
    }
    printf("\n--- Produtos Disponíveis ---\n");
    Produto* temp = head;
    while (temp) {
        printf("ID: %d | Nome: %s | Preço: R$%.2f | Estoque: %d | Img: %s, (%.0f,%.0f)\n",
               temp->id, temp->nome, temp->preco, temp->estoque,
               temp->imagem_path, temp->pos_x_inicial, temp->pos_y_inicial);
        temp = temp->prox;
    }
}

/**
 * @brief Libera toda a memória alocada para a lista de produtos.
 */
void liberar_lista_produtos(Produto* head) {
    Produto* temp;
    while (head) {
        temp = head;
        head = head->prox;
        if (temp->imagem_animacao) {
            al_destroy_bitmap(temp->imagem_animacao);
        }
        free(temp);
    }
}

/**
 * @brief Salva a lista de produtos em um arquivo no formato CSV.
 */
void salvar_produtos_csv(Produto* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }
    fprintf(f, "ID,Nome,Preco,Estoque,ImagemPath,PosX,PosY\n");
    
    Produto* temp = head;
    while (temp) {
        fprintf(f, "%d,%s,%.2f,%d,%s,%.1f,%.1f\n", 
                temp->id, temp->nome, temp->preco, temp->estoque, 
                temp->imagem_path, temp->pos_x_inicial, temp->pos_y_inicial);
        temp = temp->prox;
    }
    fclose(f);
}

/**
 * @brief Carrega uma lista de produtos a partir de um arquivo CSV.
 */
Produto* carregar_produtos_csv(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        return NULL;
    }

    Produto* head = NULL;
    char linha[256];
    
    fgets(linha, sizeof(linha), f); 
    
    while (fgets(linha, sizeof(linha), f)) {
        int id, estoque;
        char nome[50], img_path[100];
        float preco, pos_x, pos_y;

        if (sscanf(linha, "%d,%49[^,],%f,%d,%99[^,],%f,%f", &id, nome, &preco, &estoque, img_path, &pos_x, &pos_y) == 7) {
            head = adicionar_produto(head, id, nome, preco, estoque, img_path, pos_x, pos_y);
        }
    }
    fclose(f);

    Produto* revertida = NULL;
    Produto* atual = head;
    while (atual != NULL) {
        Produto* proximo = atual->prox;
        atual->prox = revertida;
        if (revertida != NULL) {
            revertida->ant = atual;
        }
        revertida = atual;
        atual = proximo;
    }
    if (revertida != NULL) {
        revertida->ant = NULL;
    }

    return revertida;
}