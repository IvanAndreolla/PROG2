#include <stdio.h>
#include <string.h>
#include "config.h"
#include "lista_produtos.h"

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

void menu_configuracao(Contexto *ctx) {
    if (!verificar_senha()) {
        return;
    }

    int op;
    do {
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
            printf("ID: ");
            id = ler_inteiro();
            printf("Nome: ");
            if (fgets(nome, sizeof(nome), stdin) != NULL) {
                nome[strcspn(nome, "\n")] = '\0';
            }
            printf("Preço: ");
            preco = ler_float();
            printf("Estoque: ");
            estoque = ler_inteiro();
            ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, id, nome, preco, estoque);
        } else if (op == 2) {
            int id;
            printf("ID do produto a remover: ");
            id = ler_inteiro();
            ctx->lista_produtos = remover_produto(ctx->lista_produtos, id);
        } else if (op == 3) {
            listar_produtos(ctx->lista_produtos);
        }
    } while (op != 0);
}