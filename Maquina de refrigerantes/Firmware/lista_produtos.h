#ifndef LISTA_PRODUTOS_H
#define LISTA_PRODUTOS_H

/**
 * @brief Estrutura para representar um produto na máquina de refrigerantes.
 */
typedef struct produto {
    int id;                /**< Identificador único do produto. */
    char nome[50];         /**< Nome do produto. */
    float preco;           /**< Preço do produto em reais. */
    int estoque;           /**< Quantidade disponível em estoque. */
    struct produto* prox;   /**< Ponteiro para o próximo produto. */
    struct produto* ant;   /**< Ponteiro para o produto anterior. */
} Produto;

/**
 * @brief Adiciona um novo produto à lista duplamente encadeada.
 * @param head Ponteiro para o início da lista.
 * @param id ID do produto.
 * @param nome Nome do produto.
 * @param preco Preço do produto.
 * @param estoque Quantidade em estoque.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* adicionar_produto(Produto* head, int id, const char* nome, float preco, int estoque);

/**
 * @brief Remove um produto da lista pelo ID.
 * @param head Ponteiro para o início da lista.
 * @param id ID do produto a ser removido.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* remover_produto(Produto* head, int id);

/**
 * @brief Busca um produto na lista pelo ID.
 * @param head Ponteiro para o início da lista.
 * @param id ID do produto a ser buscado.
 * @return Ponteiro para o produto encontrado ou NULL se não encontrado.
 */
Produto* buscar_produto(Produto* head, int id);

/**
 * @brief Lista todos os produtos disponíveis.
 * @param head Ponteiro para o início da lista.
 */
void listar_produtos(Produto* head);

/**
 * @brief Libera a memória alocada para a lista de produtos.
 * @param head Ponteiro para o início da lista.
 */
void liberar_lista_produtos(Produto* head);

/**
 * @brief Salva a lista de produtos em um arquivo CSV.
 * @param head Ponteiro para o início da lista.
 * @param filename Nome do arquivo CSV.
 */
void salvar_produtos_csv(Produto* head, const char* filename);

/**
 * @brief Carrega a lista de produtos de um arquivo CSV.
 * @param filename Nome do arquivo CSV.
 * @return Ponteiro para a nova cabeça da lista.
 */
Produto* carregar_produtos_csv(const char* filename);

#endif