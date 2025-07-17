/**
 * @file pagamento.c
 * @brief Implementação das funções de pagamento.
 * @note Este módulo contém implementações originalmente pensadas para uma versão de
 * console. Na versão gráfica final (main.c), a função `inserir_moedas` é substituída
 * por uma lógica de interface gráfica. A função `calcular_troco` continua sendo utilizada.
 */

#include <stdio.h>
#include "pagamento.h"
#include "pilha.h"

/**
 * @brief Pilha estática para armazenar as moedas inseridas em uma transação.
 * @details Usada pela funcionalidade de "desfazer" na versão console.
 */
static Pilha moedas;

/**
 * @brief Inicializa a pilha de moedas, preparando para uma nova transação.
 */
static void inicializar_moedas() {
    inicializar_pilha(&moedas);
}

/**
 * @brief Coleta as moedas inseridas pelo usuário (versão console).
 * @details A função usa um ponteiro para retornar o valor total acumulado.
 * O valor de retorno da função indica o status da operação.
 * @param[out] valor_acumulado Ponteiro para a variável que armazenará o total inserido.
 * @return 1 se a operação foi concluída com sucesso (usuário digitou 0).
 * @return 0 se a operação foi cancelada (usuário digitou -2).
 */
int inserir_moedas(float *valor_acumulado) {
    inicializar_moedas();
    float total = 0.0f;
    float valor_lido;
    
    printf("Insira moedas (0.25, 0.5, 1.0). Digite 0 para concluir, -1 para desfazer, -2 para cancelar:\n");

    while (1) {
        printf("Moeda: ");
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura. Tente novamente.\n");
            continue;
        }
        if (sscanf(buffer, "%f", &valor_lido) != 1) {
            printf("Entrada inválida. Digite um número.\n");
            continue;
        }

        if (valor_lido == 0) {
            *valor_acumulado = total;
            return 1; // Sucesso
        }

        if (valor_lido == -2) {
            *valor_acumulado = total;
            return 0; // Cancelado
        }

        if (valor_lido == -1) {
            float desfeito = desfazer_moeda();
            if (desfeito > 0) {
                total -= desfeito;
                printf("Desfeita moeda de R$%.2f. Total: R$%.2f\n", desfeito, total);
            } else {
                printf("Nada para desfazer.\n");
            }
            continue;
        }

        if (valor_lido == 0.25f || valor_lido == 0.5f || valor_lido == 1.0f) {
            total += valor_lido;
            empilhar(&moedas, valor_lido);
            printf("Total inserido: R$%.2f\n", total);
        } else {
            printf("Moeda inválida! Devolvendo...\n");
        }
    }
}

/**
 * @brief Calcula o troco com base no valor inserido e no preço do produto.
 * @param[in] valor_inserido Valor total inserido pelo usuário.
 * @param[in] preco Preço do produto.
 * @return Valor do troco, ou um valor negativo se o pagamento for insuficiente.
 */
float calcular_troco(float valor_inserido, float preco) {
    return valor_inserido >= preco ? valor_inserido - preco : -1;
}

/**
 * @brief Desfaz a inserção da última moeda, usando a pilha de moedas.
 * @return O valor da moeda removida, ou 0 se não houver moedas para remover.
 */
float desfazer_moeda() {
    if (!pilha_vazia(&moedas)) {
        return desempilhar(&moedas);
    }
    return 0;
}