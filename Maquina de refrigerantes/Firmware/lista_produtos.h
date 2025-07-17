/**
 * @file lista_produtos.h
 * @brief Definições e protótipos para a lista duplamente encadeada de produtos.
 */

#ifndef LISTA_PRODUTOS_H
#define LISTA_PRODUTOS_H

#include <allegro5/allegro.h>

/**
 * @brief Estrutura para representar um produto na máquina de refrigerantes.
 */
typedef struct produto {
    int id;                   /**< Identificador único do produto. */
    char nome[50];            /**< Nome do produto. */
    float preco;                /**< Preço do produto em reais. */
    int estoque;                /**< Quantidade disponível em estoque. */

    // --- CAMPOS ADICIONADOS PARA ANIMAÇÃO ---
    char imagem_path[100];      /**< Caminho do arquivo de imagem para a animação. */
    float pos_x_inicial;        /**< Posição X inicial para a animação de queda. */
    float pos_y_inicial;        /**< Posição Y inicial para a animação de queda. */
    ALLEGRO_BITMAP* imagem_animacao; /**< Ponteiro para o bitmap da animação carregado. */

    struct produto* prox;   /**< Ponteiro para o próximo produto na lista. */
    struct produto* ant;    /**< Ponteiro para o produto anterior na lista. */
} Produto;

/**
 * @brief Adiciona um novo produto ao início da lista duplamente encadeada.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] id ID do novo produto.
 * @param[in] nome Nome do novo produto.
 * @param[in] preco Preço do novo produto.
 * @param[in] estoque Quantidade em estoque do novo produto.
 * @param[in] img_path Caminho do arquivo de imagem para a animação.
 * @param[in] pos_x Posição X inicial da animação.
 * @param[in] pos_y Posição Y inicial da animação.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* adicionar_produto(Produto* head, int id, const char* nome, float preco, int estoque, const char* img_path, float pos_x, float pos_y);

/**
 * @brief Remove um produto da lista pelo seu ID.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] id ID do produto a ser removido.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* remover_produto(Produto* head, int id);

/**
 * @brief Busca um produto na lista pelo seu ID.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] id ID do produto a ser buscado.
 * @return Ponteiro para o nó do produto encontrado, ou NULL se não for encontrado.
 */
Produto* buscar_produto(Produto* head, int id);

/**
 * @brief Lista todos os produtos da lista no terminal.
 * @param[in] head Ponteiro para a cabeça da lista.
 */
void listar_produtos(Produto* head);

/**
 * @brief Libera toda a memória alocada para a lista de produtos.
 * @param[in] head Ponteiro para a cabeça da lista a ser liberada.
 */
void liberar_lista_produtos(Produto* head);

/**
 * @brief Salva a lista de produtos em um arquivo no formato CSV.
 * @param[in] head Ponteiro para a cabeça da lista.
 * @param[in] filename Nome do arquivo CSV de destino.
 */
void salvar_produtos_csv(Produto* head, const char* filename);

/**
 * @brief Carrega uma lista de produtos a partir de um arquivo CSV.
 * @param[in] filename Nome do arquivo CSV a ser lido.
 * @return Ponteiro para a cabeça da nova lista de produtos carregada.
 */
Produto* carregar_produtos_csv(const char* filename);

#endif