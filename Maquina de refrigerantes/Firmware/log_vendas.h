#ifndef LOG_VENDAS_H
#define LOG_VENDAS_H

typedef struct Log {
    char timestamp[20];
    int produto_id;
    float valor_pago;
    float troco;
    struct Log* prox;
} Log;

Log* adicionar_log(Log* head, const char* timestamp, int produto_id, float valor_pago, float troco);
void salvar_logs(const char* filename, Log* head);
Log* carregar_logs(const char* filename);
void liberar_logs(Log* head);
void exibir_logs(Log* head);

#endif
