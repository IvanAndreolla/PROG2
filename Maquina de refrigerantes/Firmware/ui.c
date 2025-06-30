/**
 * @file ui.c
 * @brief Implementação das funções de interface de usuário.
 */

#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "fila.h"

/**
 * @brief Limpa a tela do terminal.
 * * Tenta usar "clear" (Linux/macOS) ou "cls" (Windows).
 */
void limpar_tela() {
    system("clear || cls");
}

/**
 * @brief Pausa a execução do programa até que o usuário pressione Enter.
 * * Esta versão corrigida consome quaisquer caracteres residuais no buffer de
 * entrada e espera por um único Enter para continuar.
 */
void pausar() {
    printf("\nPressione Enter para continuar...\n");
    int c;
    // Loop para consumir (limpar) quaisquer caracteres que possam ter
    // sobrado no buffer de entrada (stdin) de uma leitura anterior.
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


/**
 * @brief Exibe a tela de boas-vindas da aplicação.
 * @param[in] propagandas Ponteiro para a fila de propagandas, para exibir a primeira.
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
    // A chamada inicial para pausar também usará a nova função corrigida.
    pausar();
}