/**
 * @file config.h
 * @brief Definições e protótipos para o módulo de configuração da máquina.
 *
 * Este arquivo contém a estrutura de contexto da aplicação, a senha de
 * administrador e os protótipos das funções relacionadas ao menu de
 * configuração.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "lista_produtos.h"
#include "log_vendas.h"

/**
 * @brief Senha padrão para acesso ao menu de configuração.
 */
#define SENHA_PADRAO "admin123"

/**
 * @brief Estrutura de contexto que armazena o estado global da aplicação.
 *
 * Agrupa os ponteiros para as principais estruturas de dados utilizadas
 * pela máquina de refrigerantes.
 */
typedef struct {
    Produto* lista_produtos;     /**< Ponteiro para a cabeça da lista de produtos. */
    Log* logs_vendas;           /**< Ponteiro para a cabeça da lista de logs de vendas. */
    Produto* produto_selecionado; /**< Ponteiro para o produto atualmente selecionado para compra. */
} Contexto;

/**
 * @brief Solicita e verifica a senha do administrador.
 * * Permite até 3 tentativas antes de bloquear o acesso.
 * @return 1 se a senha estiver correta, 0 caso contrário.
 */
int verificar_senha();

/**
 * @brief Exibe e gerencia o menu de configuração.
 *
 * Permite que o administrador adicione, remova e liste produtos. O acesso
 * a este menu é protegido por senha.
 * @param[in,out] ctx Ponteiro para o contexto da aplicação, que será modificado.
 */
void menu_configuracao(Contexto *ctx);

#endif