/**
 * @file pagamento.h
 * @brief Protótipos para as funções relacionadas ao processo de pagamento.
 */

#ifndef PAGAMENTO_H
#define PAGAMENTO_H

/**
 * @brief Coleta as moedas inseridas pelo usuário.
 * * A função usa um ponteiro para retornar o valor total acumulado.
 * O valor de retorno da função indica o status da operação.
 * @param[out] valor_acumulado Ponteiro para a variável que armazenará o total inserido.
 * @return 1 se a operação foi concluída com sucesso (usuário digitou 0).
 * @return 0 se a operação foi cancelada (usuário digitou -2).
 */
int inserir_moedas(float *valor_acumulado);

/**
 * @brief Calcula o troco com base no valor inserido e no preço do produto.
 * @param[in] valor_inserido Valor total inserido pelo usuário.
 * @param[in] preco Preço do produto.
 * @return Valor do troco, ou um valor negativo se o pagamento for insuficiente.
 */
float calcular_troco(float valor_inserido, float preco);

/**
 * @brief Desfaz a inserção da última moeda, usando a pilha de moedas.
 * @return O valor da moeda removida, ou 0 se não houver moedas para remover.
 */
float desfazer_moeda();

#endif