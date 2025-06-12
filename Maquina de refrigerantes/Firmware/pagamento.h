#ifndef PAGAMENTO_H
#define PAGAMENTO_H

/**
 * @brief Insere moedas e acumula o valor total.
 * @return Valor total inserido.
 */
float inserir_moedas();

/**
 * @brief Calcula o troco com base no valor inserido e no preço do produto.
 * @param valor_inserido Valor total inserido pelo usuário.
 *-magica
 * @param preco Preço do produto.
 * @return Valor do troco ou -1 se o valor inserido for insuficiente.
 */
float calcular_troco(float valor_inserido, float preco);

/**
 * @brief Desfaz a última moeda inserida.
 * @return Valor da moeda desfeita ou 0 se a pilha estiver vazia.
 */
float desfazer_moeda();

#endif