#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_vendas.h"

Log* adicionar_log(Log* head, const char* timestamp, int produto_id, float valor_pago, float troco) {
    Log* novo = (Log*)malloc(sizeof(Log));
    if (!novo) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return head;
    }
    strcpy(novo->timestamp, timestamp);
    novo->produto_id = produto_id;
    novo->valor_pago = valor_pago;
    novo->troco = troco;
    novo->prox = head;
    return novo;
}

void salvar_logs(const char* filename, Log* head) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }
    while (head) {
        if (fwrite(head, sizeof(Log), 1, f) != 1) {
            fprintf(stderr, "Erro ao escrever no arquivo %s.\n", filename);
            fclose(f);
            return;
        }
        head = head->prox;
    }
    fclose(f);
}

Log* carregar_logs(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para leitura.\n", filename);
        return NULL;
    }
    Log* head = NULL;
    Log temp;
    while (fread(&temp, sizeof(Log), 1, f)) {
        if (temp.produto_id <= 0 || temp.valor_pago < 0 || temp.troco < 0) {
            fprintf(stderr, "Dados inválidos no arquivo %s. Ignorando registro.\n", filename);
            continue;
        }
        head = adicionar_log(head, temp.timestamp, temp.produto_id, temp.valor_pago, temp.troco);
    }
    if (ferror(f)) {
        fprintf(stderr, "Erro durante a leitura do arquivo %s.\n", filename);
        liberar_logs(head);
        fclose(f);
        return NULL;
    }
    fclose(f);
    return head;
}

void liberar_logs(Log* head) {
    Log* temp;
    while (head) {
        temp = head;
        head = head->prox;
        free(temp);
    }
}

void exibir_logs(Log* head) {
    while (head) {
        printf("[%s] Produto %d | Pago: %.2f | Troco: %.2f\n", head->timestamp, head->produto_id, head->valor_pago, head->troco);
        head = head->prox;
    }
}