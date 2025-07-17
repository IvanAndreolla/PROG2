/**
 * @file ui.h
 * @brief Protótipos para funções de interface de usuário (UI) no terminal.
 * @note Estas funções foram criadas para uma versão de console da aplicação e
 * não são utilizadas pela interface gráfica principal implementada em main.c.
 */

#ifndef UI_H
#define UI_H

#include "fila.h"

/**
 * @brief Limpa a tela do terminal.
 * @details Tenta usar "clear" (Linux/macOS) ou "cls" (Windows).
 */
void limpar_tela();

/**
 * @brief Pausa a execução do programa até que o usuário pressione Enter.
 * @details Útil para permitir que o usuário leia as mensagens antes da tela ser limpa.
 */
void pausar();

/**
 * @brief Exibe a tela de boas-vindas da aplicação.
 * @param[in] propagandas Ponteiro para a fila de propagandas, para exibir a primeira.
 */
void inicializar_interface(Fila *propagandas);

#endif