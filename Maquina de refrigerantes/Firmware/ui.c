#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "fila.h"

/**
 * @brief Limpa a tela do terminal.
 */
void limpar_tela() {
    system("clear || cls");
}

/**
 * @brief Pausa a execução até que o usuário pressione Enter.
 */
void pausar() {
    printf("\nPressione Enter para continuar...\n");
    while (getchar() != '\n'); // Limpa o buffer
}

/**
 * @brief Inicializa a interface exibindo uma mensagem de boas-vindas e uma propaganda.
 * @param propagandas Ponteiro para a fila de propagandas.
 */
void inicializar_interface(Fila *propagandas) {
    limpar_tela();
    printf("\n=====================================\n");
    printf("  Bem-vindo à Máquina de Refrigerantes!  \n");
    printf("=====================================\n\n");
    if (propagandas) {
        const char* propaganda = frente_fila(propagandas);
        if (propaganda) {
            printf("Propaganda: %s\n\n", propaganda);
        } else {
            printf("Nenhuma propaganda disponível.\n\n");
        }
    } else {
        printf("Fila de propagandas não inicializada.\n\n");
    }
    pausar();
}