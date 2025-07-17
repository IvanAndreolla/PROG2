/**
 * @file config.c
 * @brief Implementação das funcionalidades do menu de configuração.
 * @note Este módulo foi projetado para uma versão de console da aplicação.
 * Na versão gráfica final (main.c), toda a interface e lógica de configuração
 * foram reimplementadas com Allegro, e as funções deste arquivo (exceto
 * `verificar_senha`, cuja lógica foi adaptada) não são utilizadas.
 */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "lista_produtos.h"
#include "ui.h" 

/**
 * @brief Lê um número inteiro da entrada padrão com validação.
 * @details Continua solicitando a entrada até que um número inteiro válido seja fornecido.
 * @return O número inteiro lido.
 */
static int ler_inteiro() {
    char buffer[256];
    int valor;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura. Tente novamente: ");
            continue;
        }
        if (sscanf(buffer, "%d", &valor) == 1) {
            return valor;
        }
        printf("Entrada inválida. Digite um número: ");
    }
}

/**
 * @brief Lê um número de ponto flutuante da entrada padrão com validação.
 * @details Continua solicitando a entrada até que um número válido seja fornecido.
 * @return O número float lido.
 */
static float ler_float() {
    char buffer[256];
    float valor;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura. Tente novamente: ");
            continue;
        }
        if (sscanf(buffer, "%f", &valor) == 1) {
            return valor;
        }
        printf("Entrada inválida. Digite um número: ");
    }
}

/**
 * @brief Solicita e verifica a senha do administrador.
 * @details Permite até 3 tentativas antes de bloquear o acesso.
 * @return 1 se a senha estiver correta, 0 caso contrário.
 */
int verificar_senha() {
    char senha[50];
    int tentativas = 3;
    while (tentativas > 0) {
        printf("Digite a senha (%d tentativas restantes): ", tentativas);
        if (fgets(senha, sizeof(senha), stdin) == NULL) {
            continue;
        }
        senha[strcspn(senha, "\n")] = '\0';
        if (strcmp(senha, SENHA_PADRAO) == 0) {
            return 1;
        }
        printf("Senha incorreta.\n");
        --tentativas;
    }
    printf("Muitas tentativas falhas. Acesso bloqueado.\n");
    return 0;
}

/**
 * @brief Exibe e gerencia o menu de configuração.
 *
 * Permite que o administrador adicione, remova e liste produtos. O acesso
 * a este menu é protegido por senha.
 * @param[in,out] ctx Ponteiro para o contexto da aplicação, que será modificado.
 */
void menu_configuracao(Contexto *ctx) {
    if (!verificar_senha()) {
        pausar();
        return;
    }

    int op;
    do {
        limpar_tela();
        printf("\n--- CONFIGURAÇÃO ---\n");
        printf("1. Adicionar produto\n");
        printf("2. Remover produto\n");
        printf("3. Listar produtos\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        op = ler_inteiro();

        if (op == 1) {
            int id, estoque;
            char nome[50];
            float preco;
            
            char img_path[100];
            float pos_x, pos_y;

            printf("ID: ");
            id = ler_inteiro();

            if (buscar_produto(ctx->lista_produtos, id) != NULL) {
                printf("\nERRO: O ID %d já está em uso. O produto não foi adicionado.\n", id);
            } else {
                printf("Nome: ");
                if (fgets(nome, sizeof(nome), stdin) != NULL) {
                    nome[strcspn(nome, "\n")] = '\0';
                }
                printf("Preço: ");
                preco = ler_float();
                printf("Estoque: ");
                estoque = ler_inteiro();

                printf("Caminho da imagem (ex: lata.png): ");
                if (fgets(img_path, sizeof(img_path), stdin) != NULL) {
                    img_path[strcspn(img_path, "\n")] = '\0';
                }
                printf("Posição X inicial da lata: ");
                pos_x = ler_float();
                printf("Posição Y inicial da lata: ");
                pos_y = ler_float();

                ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, id, nome, preco, estoque, img_path, pos_x, pos_y);
                printf("Produto '%s' adicionado com sucesso!\n", nome);
            }
        } else if (op == 2) {
            int id;
            printf("ID do produto a remover: ");
            id = ler_inteiro();
            ctx->lista_produtos = remover_produto(ctx->lista_produtos, id);
        } else if (op == 3) {
            listar_produtos(ctx->lista_produtos);
        }
        
        if (op != 0) {
            pausar();
        }

    } while (op != 0);
}