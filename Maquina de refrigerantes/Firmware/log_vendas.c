/**
 * @file log_vendas.c
 * @brief Implementação das funções de gerenciamento de logs de vendas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_vendas.h"

/**
 * @brief Adiciona um novo log de venda ao início da lista.
 * @param[in] head Ponteiro para a cabeça da lista de logs.
 * @param[in] timestamp String com a data e hora da venda.
 * @param[in] produto_id ID do produto vendido.
 * @param[in] valor_pago Valor inserido pelo cliente.
 * @param[in] troco Valor do troco.
 * @return Ponteiro para a nova cabeça da lista de logs.
 */
Log* adicionar_log(Log* head, const char* timestamp, int produto_id, float valor_pago, float troco) {
    Log* novo = (Log*)malloc(sizeof(Log));
    if (!novo) {
        fprintf(stderr, "Erro de alocação de memória para novo log.\n");
        return head;
    }
    strncpy(novo->timestamp, timestamp, sizeof(novo->timestamp) -1);
    novo->timestamp[sizeof(novo->timestamp) - 1] = '\0';
    novo->produto_id = produto_id;
    novo->valor_pago = valor_pago;
    novo->troco = troco;
    novo->prox = head;
    return novo;
}

/**
 * @brief Salva a lista de logs em um arquivo binário.
 * @param[in] filename Nome do arquivo de destino.
 * @param[in] head Ponteiro para a cabeça da lista de logs.
 */
void salvar_logs(const char* filename, Log* head) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }
    Log* temp = head;
    while (temp) {
        if (fwrite(temp, sizeof(Log), 1, f) != 1) {
            fprintf(stderr, "Erro ao escrever log no arquivo %s.\n", filename);
            fclose(f);
            return;
        }
        temp = temp->prox;
    }
    fclose(f);
}

/**
 * @brief Carrega a lista de logs a partir de um arquivo binário.
 * @param[in] filename Nome do arquivo a ser lido.
 * @return Ponteiro para a cabeça da nova lista de logs carregada.
 */
Log* carregar_logs(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        //fprintf(stderr, "Aviso: Arquivo de logs '%s' não encontrado. Começando com lista vazia.\n", filename);
        return NULL;
    }
    Log* head = NULL;
    Log temp_log;
    while (fread(&temp_log, sizeof(Log), 1, f) == 1) {
        // Adiciona no final para manter a ordem cronológica na memória (opcional)
        head = adicionar_log(head, temp_log.timestamp, temp_log.produto_id, temp_log.valor_pago, temp_log.troco);
    }
    fclose(f);
    // Como adicionamos no início, a lista fica invertida. Para exibir em ordem, pode-se revertê-la.
    // Para este projeto, a ordem em memória não é crítica.
    return head;
}

/**
 * @brief Libera toda a memória alocada para a lista de logs.
 * @param[in] head Ponteiro para a cabeça da lista a ser liberada.
 */
void liberar_logs(Log* head) {
    Log* temp;
    while (head) {
        temp = head;
        head = head->prox;
        free(temp);
    }
}

/**
 * @brief Exibe todos os logs de vendas no terminal.
 * @param[in] head Ponteiro para a cabeça da lista de logs.
 */
void exibir_logs(Log* head) {
    printf("\n--- Histórico de Vendas ---\n");
    if (!head) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    Log* temp = head;
    while (temp) {
        printf("[%s] Produto %d | Pago: R$%.2f | Troco: R$%.2f\n", 
               temp->timestamp, temp->produto_id, temp->valor_pago, temp->troco);
        temp = temp->prox;
    }
}