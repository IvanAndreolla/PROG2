#include <stdio.h>
#include "pagamento.h"
#include "pilha.h"

static Pilha moedas;

/**
 * @brief Inicializa a pilha de moedas.
 */
static void inicializar_moedas() {
    inicializar_pilha(&moedas);
}

/**
 * @brief Insere moedas e acumula o valor total, com opção de desfazer.
 * @return Valor total inserido.
 */
float inserir_moedas() {
    inicializar_moedas();
    float total = 0.0f;
    float valor;
    printf("Insira moedas (0.25, 0.5, 1.0). Digite 0 para concluir, -1 para desfazer:\n");

    while (1) {
        printf("Moeda: ");
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura. Tente novamente.\n");
            continue;
        }
        if (sscanf(buffer, "%f", &valor) != 1) {
            printf("Entrada inválida. Digite um número.\n");
            continue;
        }
        if (valor == 0) break;
        if (valor == -1) {
            float desfeito = desfazer_moeda();
            if (desfeito > 0) {
                total -= desfeito;
                printf("Desfeita moeda de R$%.2f. Total: R$%.2f\n", desfeito, total);
            } else {
                printf("Nada para desfazer.\n");
            }
            continue;
        }
        if (valor == 0.25f || valor == 0.5f || valor == 1.0f) {
            total += valor;
            empilhar(&moedas, valor);
            printf("Total inserido: R$%.2f\n", total);
        } else {
            printf("Moeda inválida! Devolvendo...\n");
        }
    }
    return total;
}

/**
 * @brief Calcula o troco com base no valor inserido e no preço.
 * @param valor_inserido Valor total inserido.
 * @param preco Preço do produto.
 * @return Valor do troco ou -1 se insuficiente.
 */
float calcular_troco(float valor_inserido, float preco) {
    return valor_inserido >= preco ? valor_inserido - preco : -1;
}

/**
 * @brief Desfaz a última moeda inserida.
 * @return Valor da moeda desfeita ou 0 se a pilha estiver vazia.
 */
float desfazer_moeda() {
    if (!pilha_vazia(&moedas)) {
        return desempilhar(&moedas);
    }
    return 0;
}