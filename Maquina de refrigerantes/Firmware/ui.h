/**
 * @file ui.h
 * @brief Protótipos para funções de interface de usuário (UI) no terminal.
 */

#ifndef UI_H
#define UI_H

#include "fila.h" // Incluído para o parâmetro de inicializar_interface

/**
 * @brief Limpa a tela do terminal.
 * * Tenta usar "clear" (Linux/macOS) ou "cls" (Windows).
 */
void limpar_tela();

/**
 * @brief Pausa a execução do programa até que o usuário pressione Enter.
 * * Útil para permitir que o usuário leia as mensagens antes da tela ser limpa.
 */
void pausar();

/**
 * @brief Exibe a tela de boas-vindas da aplicação.
 * @param[in] propagandas Ponteiro para a fila de propagandas, para exibir a primeira.
 */
void inicializar_interface(Fila *propagandas);

#endif