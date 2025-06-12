#ifndef CONFIG_H
#define CONFIG_H

#include "lista_produtos.h"
#include "log_vendas.h"

#define SENHA_PADRAO "admin123"

typedef struct {
    Produto* lista_produtos;
    Log* logs_vendas;
    Produto* produto_selecionado;
} Contexto;

int verificar_senha();
void menu_configuracao(Contexto *ctx);

#endif