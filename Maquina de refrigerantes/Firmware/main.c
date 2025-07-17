/**
 * @file main.c
 * @brief Ponto de entrada principal e gerenciador da interface gráfica com Allegro 5.
 * @details Este arquivo inicializa a biblioteca Allegro, cria a janela, gerencia
 * o laço de eventos principal e controla a máquina de estados finitos que rege a
 * aplicação. Toda a lógica de renderização das telas e de interação do usuário
 * (teclado e mouse) está contida aqui.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "config.h"
#include "states.h"
#include "lista_produtos.h"
#include "log_vendas.h"
#include "pagamento.h"
#include "estatisticas.h"
#include "fila.h"

// Definições da Janela e UI
#define LARGURA_JANELA 1280
#define ALTURA_JANELA 720
#define FPS 60.0

// Coordenadas da "tela" da máquina
#define TELA_X 607
#define TELA_Y 130
#define TELA_LARGURA 159
#define TELA_ALTURA 190

// Coordenadas da área de propaganda
#define PROPAGANDA_X 335
#define PROPAGANDA_Y 110
#define PROPAGANDA_LARGURA 220
#define PROPAGANDA_ALTURA 60

// Coordenadas da porta de entrega
#define PORTA_X 400
#define PORTA_Y 560

/**
 * @struct AnimacaoQueda
 * @brief Armazena os dados para a animação de queda de um produto.
 */
typedef struct {
    ALLEGRO_BITMAP* bitmap; ///< Bitmap do produto que está caindo.
    float x, y, angulo;     ///< Posição (x, y) e ângulo de rotação atuais.
    float velocidade_y;     ///< Velocidade de queda no eixo Y.
    float velocidade_angulo;///< Velocidade de rotação.
    float y_final;          ///< Posição Y onde a animação termina.
    bool ativa;             ///< Flag que indica se a animação está em andamento.
} AnimacaoQueda;

/**
 * @struct AnimacaoEntrega
 * @brief Armazena os dados para a animação de entrega do produto ao cliente.
 */
typedef struct {
    ALLEGRO_BITMAP* bitmap; ///< Bitmap do produto sendo entregue.
    float x, y, escala, alpha; ///< Posição, escala e transparência (alpha) atuais.
    float velocidade_x, velocidade_escala; ///< Velocidades de movimento e de alteração da escala.
    float escala_final;     ///< Escala final para a animação.
    bool ativa;             ///< Flag que indica se a animação está em andamento.
} AnimacaoEntrega;

/**
 * @brief Desenha os produtos na vitrine da máquina.
 * @param lista_produtos Ponteiro para a lista de produtos.
 * @param animacao_atual Ponteiro para a animação de queda, para não desenhar o produto que está caindo.
 */
void desenhar_produtos_estaticos(Produto* lista_produtos, AnimacaoQueda* animacao_atual) {
    float largura_na_vitrine = 50.0f;
    float altura_na_vitrine = 50.0f;
    
    Produto* p = lista_produtos;
    while (p) {
        if (p->imagem_animacao) {
            if (p->estoque > 0 && (!animacao_atual->ativa || p->imagem_animacao != animacao_atual->bitmap)) {
                
                float sw = al_get_bitmap_width(p->imagem_animacao);
                float sh = al_get_bitmap_height(p->imagem_animacao);

                al_draw_scaled_bitmap(p->imagem_animacao, 
                                    0, 0, sw, sh,
                                    p->pos_x_inicial, p->pos_y_inicial,
                                    largura_na_vitrine, altura_na_vitrine,
                                    0);
            }
        }
        p = p->prox;
    }
}

/**
 * @brief Desenha o menu principal na tela da máquina.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param indice_selecionado Índice da opção de menu atualmente selecionada.
 */
void desenhar_menu_principal(ALLEGRO_FONT *fonte, int indice_selecionado) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    const char* opcoes[] = {"COMPRAR", "Configurações", "Estatísticas", "Sair"};

    float btn_largura = TELA_LARGURA * 0.9f;
    float main_btn_altura = 45.0f;
    float other_btn_altura = 15.0f;
    float espaco_entre_btns = 10.0f;

    float altura_total_elementos = main_btn_altura + (3 * other_btn_altura) + (3 * espaco_entre_btns);
    float y_pos = TELA_Y + (TELA_ALTURA - altura_total_elementos) / 2;

    float btn_x = centro_x - btn_largura / 2;
    ALLEGRO_COLOR cor_fundo, cor_texto;

    if (indice_selecionado == 0) {
        al_draw_filled_rounded_rectangle(btn_x - 2, y_pos - 2, btn_x + btn_largura + 2, y_pos + main_btn_altura + 2, 9, 9, al_map_rgb(255, 255, 0));
        cor_fundo = al_map_rgb(0, 180, 140);
        cor_texto = al_map_rgb(255, 255, 255);
    } else {
        cor_fundo = al_map_rgb(50, 50, 70);
        cor_texto = al_map_rgb(200, 200, 200);
    }
    al_draw_filled_rounded_rectangle(btn_x, y_pos, btn_x + btn_largura, y_pos + main_btn_altura, 6, 6, cor_fundo);
    al_draw_text(fonte, cor_texto, centro_x, y_pos + (main_btn_altura / 2) - (al_get_font_line_height(fonte) / 2), ALLEGRO_ALIGN_CENTER, opcoes[0]);

    y_pos += main_btn_altura + espaco_entre_btns;

    for (int i = 1; i < 4; i++) {
        if (indice_selecionado == i) {
            al_draw_filled_rounded_rectangle(btn_x - 2, y_pos - 2, btn_x + btn_largura + 2, y_pos + other_btn_altura + 2, 9, 9, al_map_rgb(255, 255, 0));
            cor_fundo = al_map_rgb(40, 120, 200);
            cor_texto = al_map_rgb(255, 255, 255);
        } else {
            cor_fundo = al_map_rgb(50, 50, 70);
            cor_texto = al_map_rgb(200, 200, 200);
        }
        al_draw_filled_rounded_rectangle(btn_x, y_pos, btn_x + btn_largura, y_pos + other_btn_altura, 6, 6, cor_fundo);
        al_draw_text(fonte, cor_texto, centro_x, y_pos + (other_btn_altura / 2) - (al_get_font_line_height(fonte) / 2), ALLEGRO_ALIGN_CENTER, opcoes[i]);

        y_pos += other_btn_altura + espaco_entre_btns;
    }
    
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 13, ALLEGRO_ALIGN_CENTER, "Use as setas e Enter");
}

/**
 * @brief Desenha a mensagem de propaganda na área designada.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param propaganda String contendo a propaganda a ser exibida.
 */
void desenhar_tela_propaganda(ALLEGRO_FONT* fonte, const char* propaganda) {
    if (!propaganda) return;
    int centro_x = PROPAGANDA_X + PROPAGANDA_LARGURA / 2;
    int centro_y = PROPAGANDA_Y + PROPAGANDA_ALTURA / 2;
    al_draw_text(fonte, al_map_rgb(255, 215, 0), centro_x, centro_y - (al_get_font_line_height(fonte) / 2), ALLEGRO_ALIGN_CENTER, propaganda);
}

/**
 * @brief Desenha a tela de seleção de produtos.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param lista_produtos Ponteiro para a lista de produtos a ser exibida.
 * @param indice_selecionado Índice do produto atualmente selecionado.
 */
void desenhar_tela_produtos(ALLEGRO_FONT *fonte, Produto *lista_produtos, int indice_selecionado) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    int y_pos = TELA_Y + 15;
    al_draw_text(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "--- Produtos ---");
    
    y_pos += 30;
    int i = 0;
    Produto* atual = lista_produtos;
    while(atual != NULL) {
        ALLEGRO_COLOR cor_texto = al_map_rgb(255, 255, 255);
        if (i == indice_selecionado) {
            float rect_x1 = TELA_X + 5;
            float rect_x2 = TELA_X + TELA_LARGURA - 5;
            al_draw_filled_rectangle(rect_x1, y_pos - 2, rect_x2, y_pos + 18, al_map_rgb(70, 70, 90));
            cor_texto = al_map_rgb(255, 255, 0);
        }
        al_draw_textf(fonte, cor_texto, TELA_X + 10, y_pos, ALLEGRO_ALIGN_LEFT, "%s-R$%.2f (%d)", atual->nome, atual->preco, atual->estoque);
        y_pos += 20;
        i++;
        atual = atual->prox;
    }

    if (indice_selecionado != -1) {
        al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "Setas e Enter");
    } else {
        al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "ESC para voltar");
    }
}

/**
 * @brief Desenha a tela de pagamento para um produto selecionado.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param produto_selecionado Ponteiro para o produto que está sendo comprado.
 * @param valor_inserido Valor monetário já inserido pelo usuário.
 * @param botao_pressionado_id ID do botão sendo pressionado (para feedback visual).
 */
void desenhar_tela_pagamento(ALLEGRO_FONT *fonte, Produto *produto_selecionado, float valor_inserido, int botao_pressionado_id) {
    if (!produto_selecionado) return;
    int centro_x = TELA_X + TELA_LARGURA / 2;
    int y_pos = TELA_Y + 15;

    al_draw_text(fonte, al_map_rgb(255, 255, 0), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "--- PAGAMENTO ---");
    y_pos += 25;
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "%s", produto_selecionado->nome);
    y_pos += 20;
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Preço: R$ %.2f", produto_selecionado->preco);
    
    int y_botoes = TELA_Y + 80;
    float padding = 5;
    float button_width = (TELA_LARGURA - 4 * padding) / 3.0;
    
    float x_b1 = TELA_X + padding;
    float x_b2 = x_b1 + button_width + padding;
    float x_b3 = x_b2 + button_width + padding;

    ALLEGRO_COLOR cor_b1 = (botao_pressionado_id == 1) ? al_map_rgb(80, 200, 80) : al_map_rgb(0, 100, 0);
    al_draw_filled_rectangle(x_b1, y_botoes, x_b1 + button_width, y_botoes + 25, cor_b1);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), x_b1 + button_width / 2, y_botoes + 3, ALLEGRO_ALIGN_CENTER, "1.00");

    ALLEGRO_COLOR cor_b2 = (botao_pressionado_id == 2) ? al_map_rgb(80, 200, 80) : al_map_rgb(0, 100, 0);
    al_draw_filled_rectangle(x_b2, y_botoes, x_b2 + button_width, y_botoes + 25, cor_b2);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), x_b2 + button_width / 2, y_botoes + 3, ALLEGRO_ALIGN_CENTER, "0.50");
    
    ALLEGRO_COLOR cor_b3 = (botao_pressionado_id == 3) ? al_map_rgb(80, 200, 80) : al_map_rgb(0, 100, 0);
    al_draw_filled_rectangle(x_b3, y_botoes, x_b3 + button_width, y_botoes + 25, cor_b3);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), x_b3 + button_width / 2, y_botoes + 3, ALLEGRO_ALIGN_CENTER, "0.25");

    int y_pagar = y_botoes + 35;
    float pagar_width = button_width * 2 + padding;
    float x_pagar_start = centro_x - pagar_width / 2;
    
    ALLEGRO_COLOR cor_pagar = (botao_pressionado_id == 4) ? al_map_rgb(80, 255, 80) : al_map_rgb(0, 150, 0);
    al_draw_filled_rectangle(x_pagar_start, y_pagar, x_pagar_start + pagar_width, y_pagar + 25, cor_pagar);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), centro_x, y_pagar + 3, ALLEGRO_ALIGN_CENTER, "PAGAR");

    al_draw_textf(fonte, al_map_rgb(173, 216, 230), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "Inserido: R$ %.2f", valor_inserido);
}

/**
 * @brief Desenha a tela de estatísticas de vendas.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param ctx Ponteiro para o contexto da aplicação, contendo os logs de vendas.
 */
void desenhar_tela_estatisticas(ALLEGRO_FONT *fonte, Contexto *ctx) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    int y_pos = TELA_Y + 15;
    al_draw_text(fonte, al_map_rgb(255, 255, 0), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "--- ESTATÍSTICAS ---");
    
    EstatisticasData dados;
    calcular_estatisticas(ctx->logs_vendas, &dados);
    
    y_pos += 30;
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Vendas: %d", dados.total_vendas);
    y_pos += 25;
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Total: R$ %.2f", dados.valor_arrecadado);
    y_pos += 25;
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Média: R$ %.2f", dados.valor_medio);
    y_pos += 25;

    if (dados.total_vendas > 0) {
        Produto* mais_vendido = buscar_produto(ctx->lista_produtos, dados.id_mais_vendido);
        if (mais_vendido) {
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Mais Vendido: %s", mais_vendido->nome);
        } else {
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Mais Vendido: ID %d (Removido)", dados.id_mais_vendido);
        }
    } else {
        al_draw_text(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "Mais Vendido: N/A");
    }
    
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "Pressione ESC para voltar");
}

/**
 * @brief Desenha a tela de inserção de senha para acesso à configuração.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param senha_digitada String com a senha atualmente digitada pelo usuário.
 * @param cursor_visivel Flag para controlar o piscar do cursor.
 */
void desenhar_tela_senha(ALLEGRO_FONT* fonte, const char* senha_digitada, bool cursor_visivel) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    int y_pos = TELA_Y + TELA_ALTURA / 2 - 40;
    al_draw_text(fonte, al_map_rgb(255, 255, 0), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "ACESSO RESTRITO");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos + 20, ALLEGRO_ALIGN_CENTER, "Digite a senha");
    
    float box_width = TELA_LARGURA * 0.9;
    float box_x1 = centro_x - box_width / 2;
    float box_x2 = centro_x + box_width / 2;
    float box_y = y_pos + 50;
    float text_x = box_x1 + 10;
    
    al_draw_rectangle(box_x1, box_y, box_x2, box_y + 30, al_map_rgb(255, 255, 255), 2.0);
    
    char senha_visual[51] = {0};
    for (size_t i = 0; i < strlen(senha_digitada); i++) {
        senha_visual[i] = '*';
    }
    al_draw_text(fonte, al_map_rgb(255, 255, 255), text_x, box_y + 5, ALLEGRO_ALIGN_LEFT, senha_visual);
    
    if (cursor_visivel) {
        int pos_cursor_x = text_x + al_get_text_width(fonte, senha_visual);
        al_draw_line(pos_cursor_x, box_y + 2, pos_cursor_x, box_y + 28, al_map_rgb(255, 255, 255), 2.0);
    }
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "Enter: Confirma | ESC: Volta");
}

/**
 * @brief Desenha o menu principal de configurações.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param indice_selecionado Índice da opção de configuração selecionada.
 */
void desenhar_menu_config(ALLEGRO_FONT* fonte, int indice_selecionado) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    al_draw_text(fonte, al_map_rgb(0, 255, 0), centro_x, TELA_Y + 15, ALLEGRO_ALIGN_CENTER, "--- CONFIGURAÇÃO ---");
    
    const char* opcoes[] = {"Adicionar Produto", "Remover Produto", "Listar Produtos", "Atualizar Estoque"};
    
    float highlight_width = TELA_LARGURA * 0.95;
    float rect_x1 = centro_x - highlight_width / 2;
    float rect_x2 = centro_x + highlight_width / 2;
    
    for (int i = 0; i < 4; i++) {
        int y_pos = TELA_Y + 50 + i * 30;
        ALLEGRO_COLOR cor_texto = al_map_rgb(255, 255, 255);
        if (i == indice_selecionado) {
            al_draw_filled_rectangle(rect_x1, y_pos - 5, rect_x2, y_pos + 25, al_map_rgb(70, 70, 90));
            cor_texto = al_map_rgb(255, 255, 0);
        }
        al_draw_text(fonte, cor_texto, centro_x, y_pos, ALLEGRO_ALIGN_CENTER, opcoes[i]);
    }
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "Setas, Enter e ESC ");
}

/**
 * @brief Desenha o formulário para adicionar um novo produto.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param id String para o campo ID.
 * @param nome String para o campo Nome.
 * @param preco String para o campo Preço.
 * @param estoque String para o campo Estoque.
 * @param img_path String para o caminho da imagem.
 * @param pos_x String para a posição X.
 * @param pos_y String para a posição Y.
 * @param campo_ativo Índice do campo do formulário atualmente ativo.
 * @param cursor_visivel Flag para controlar o piscar do cursor.
 */
void desenhar_tela_add_produto(ALLEGRO_FONT* fonte, char* id, char* nome, char* preco, char* estoque, char* img_path, char* pos_x, char* pos_y, int campo_ativo, bool cursor_visivel) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    al_draw_text(fonte, al_map_rgb(0, 255, 0), centro_x, TELA_Y + 9, ALLEGRO_ALIGN_CENTER, "--- ADICIONAR PRODUTO ---");
    
    char* labels[] = {"ID:", "Nome:", "Preço:", "Estq:", "Img Path:", "Pos X:", "Pos Y:"};
    char* valores[] = {id, nome, preco, estoque, img_path, pos_x, pos_y};
    
    float padding = 5;
    float label_x = TELA_X + padding;
    float input_x = TELA_X + TELA_LARGURA * 0.45;
    float input_w = TELA_LARGURA * 0.55 - padding * 2;

    for (int i = 0; i < 7; i++) {
        int y_pos = TELA_Y + 25 + i * 23;
        al_draw_text(fonte, al_map_rgb(255, 255, 255), label_x, y_pos + 5, ALLEGRO_ALIGN_LEFT, labels[i]);
        
        al_draw_rectangle(input_x, y_pos, input_x + input_w, y_pos + 20, al_map_rgb(255, 255, 255), 1.0);
        if (i == campo_ativo) {
            al_draw_rectangle(input_x - 1, y_pos - 1, input_x + input_w + 1, y_pos + 21, al_map_rgb(255, 255, 0), 2.0);
        }
        
        al_draw_text(fonte, al_map_rgb(255, 255, 255), input_x + 3, y_pos + 3, ALLEGRO_ALIGN_LEFT, valores[i]);
        if (i == campo_ativo && cursor_visivel) {
            int pos_cursor_x = input_x + 3 + al_get_text_width(fonte, valores[i]);
            al_draw_line(pos_cursor_x, y_pos + 2, pos_cursor_x, y_pos + 18, al_map_rgb(255, 255, 255), 2.0);
        }
    }
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 193, ALLEGRO_ALIGN_CENTER, "TAB:Navega | Enter:Salva");
}

/**
 * @brief Desenha a tela para remover um produto.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param id_str String para o campo ID do produto a ser removido.
 * @param cursor_visivel Flag para controlar o piscar do cursor.
 */
void desenhar_tela_remove_produto(ALLEGRO_FONT* fonte, char* id_str, bool cursor_visivel) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    int y_pos = TELA_Y + TELA_ALTURA / 2 - 40;
    
    al_draw_text(fonte, al_map_rgb(255, 100, 100), centro_x, y_pos, ALLEGRO_ALIGN_CENTER, "--- REMOVER PRODUTO ---");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), centro_x, y_pos + 30, ALLEGRO_ALIGN_CENTER, "ID do Produto a Remover:");

    float box_width = TELA_LARGURA * 0.5;
    float box_x = centro_x - box_width / 2;
    float box_y = y_pos + 60;
    
    al_draw_rectangle(box_x, box_y, box_x + box_width, box_y + 30, al_map_rgb(255, 255, 255), 2.0);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), centro_x, box_y + 5, ALLEGRO_ALIGN_CENTER, id_str);
    
    if (cursor_visivel) {
        int text_w = al_get_text_width(fonte, id_str);
        int pos_cursor_x = centro_x + text_w / 2 + 2;
        al_draw_line(pos_cursor_x, box_y + 2, pos_cursor_x, box_y + 28, al_map_rgb(255, 255, 255), 2.0);
    }
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "Enter: Confirma | ESC: Volta");
}

/**
 * @brief Desenha a tela para atualizar o estoque de um produto.
 * @param fonte Ponteiro para a fonte a ser usada.
 * @param id_str String para o campo ID.
 * @param estoque_str String para o novo valor de estoque.
 * @param campo_ativo Índice do campo do formulário atualmente ativo.
 * @param cursor_visivel Flag para controlar o piscar do cursor.
 */
void desenhar_tela_atualizar_estoque(ALLEGRO_FONT* fonte, char* id_str, char* estoque_str, int campo_ativo, bool cursor_visivel) {
    int centro_x = TELA_X + TELA_LARGURA / 2;
    al_draw_text(fonte, al_map_rgb(100, 180, 255), centro_x, TELA_Y + 20, ALLEGRO_ALIGN_CENTER, "--- ATUALIZAR ESTOQUE ---");

    float padding = 10;
    float label_x = TELA_X + padding;
    float input_x = TELA_X + TELA_LARGURA * 0.60;
    float input_w = TELA_LARGURA * 0.40 - padding * 2;
    
    char* labels[] = {"ID do Produto:", "Nova Qtd.:"};
    char* valores[] = {id_str, estoque_str};

    for (int i = 0; i < 2; i++) {
        int y_pos = TELA_Y + 70 + i * 45;
        al_draw_text(fonte, al_map_rgb(255, 255, 255), label_x, y_pos + 5, ALLEGRO_ALIGN_LEFT, labels[i]);
        
        al_draw_rectangle(input_x, y_pos, input_x + input_w, y_pos + 25, al_map_rgb(255, 255, 255), 1.0);
        if (i == campo_ativo) {
            al_draw_rectangle(input_x - 1, y_pos - 1, input_x + input_w + 1, y_pos + 26, al_map_rgb(255, 255, 0), 2.0);
        }
        
        al_draw_text(fonte, al_map_rgb(255, 255, 255), input_x + 5, y_pos + 5, ALLEGRO_ALIGN_LEFT, valores[i]);
        if (i == campo_ativo && cursor_visivel) {
            int pos_cursor_x = input_x + 5 + al_get_text_width(fonte, valores[i]);
            al_draw_line(pos_cursor_x, y_pos + 2, pos_cursor_x, y_pos + 23, al_map_rgb(255, 255, 255), 2.0);
        }
    }
    al_draw_text(fonte, al_map_rgb(200, 200, 200), centro_x, TELA_Y + TELA_ALTURA - 25, ALLEGRO_ALIGN_CENTER, "TAB: Navega | Enter: Salva");
}


/**
 * @brief Função principal da aplicação.
 * @details Inicializa o Allegro, carrega recursos, gerencia o laço de eventos
 * (entradas do usuário, timer, redesenho da tela) e, ao final, salva os dados
 * e libera os recursos alocados.
 * @return 0 em caso de sucesso, -1 em caso de erro de inicialização.
 */
int main() {
    al_init(); al_init_font_addon(); al_init_ttf_addon(); al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard(); al_install_mouse();
    
    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    ALLEGRO_FONT *fonte_geral = al_load_font("fonte.ttf", 11, 0);
    ALLEGRO_FONT *fonte_propaganda = al_load_font("fonte2.ttf", 16, 0);
    ALLEGRO_BITMAP *imagem_fundo = al_load_bitmap("maquina.png");
    ALLEGRO_BITMAP *porta_semi_aberta = al_load_bitmap("porta_semi_aberta.png");
    ALLEGRO_BITMAP *porta_aberta = al_load_bitmap("porta_aberta.png");

    al_set_window_title(janela, "Máquina de Refrigerantes");
    if (!janela || !timer || !fila_eventos || !fonte_geral || !fonte_propaganda) {
        fprintf(stderr, "Falha ao inicializar recursos básicos do Allegro.\n"); return -1;
    }
    if (!imagem_fundo) {
        fprintf(stderr, "Aviso: 'maquina.png' não encontrada. Usando fundo sólido.\n");
    }
    if (!porta_semi_aberta || !porta_aberta) {
        fprintf(stderr, "Aviso: Imagens da porta nao encontradas.\n");
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    Contexto* ctx = (Contexto*)malloc(sizeof(Contexto));
    ctx->lista_produtos = carregar_produtos_csv("produtos.csv");
    ctx->produto_selecionado = NULL;
    ctx->logs_vendas = carregar_logs("vendas.bin");
    
    if (!ctx->lista_produtos) {
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 1, "Pureza", 1.50, 10, "lata_pureza.png", 350, 250);
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 2, "Laranjinha", 1.50, 10, "lata_laranjinha.png", 400, 250);
        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, 3, "Guaraná", 2.00, 5, "lata_guarana.png", 450, 250);
        
    }
    
    Fila propagandas;
    inicializar_fila(&propagandas);
    enfileirar(&propagandas, "Pureza: O sabor daqui.");
    enfileirar(&propagandas, "Laranjinha: Sabor que faz sorrir.");
    enfileirar(&propagandas, "Guaraná: A energia do Brasil.");
    enfileirar(&propagandas, "Coca-Cola: A pausa que refresca.");
    enfileirar(&propagandas, "Curta o seu momento Lelé.");
    enfileirar(&propagandas, "Guaraná Jesus: O sonho cor-de-rosa.");

    
    const char* propaganda_atual = frente_fila(&propagandas);
    double tempo_proxima_propaganda = al_get_time() + 5.0;

    bool rodando = true;
    bool redesenhar = true;
    State estado_atual = MENU_PRINCIPAL;
    int indice_selecionado = 0;
    int menu_principal_indice_selecionado = 0;
    int config_indice_selecionado = 0;
    float valor_inserido_pagamento = 0.0f;

    char mensagem_tela[100] = "";
    double tempo_mensagem = 0;
    ALLEGRO_COLOR cor_mensagem;
    
    char senha_digitada[51] = "";
    int frames_cursor = 0;
    bool cursor_visivel = true;
    double tempo_mudanca_estado = 0;

    int campo_ativo_form = 0;
    char form_id_str[11] = "", form_nome_str[51] = "", form_preco_str[11] = "", form_estoque_str[11] = "";
    char form_img_path_str[100] = "", form_pos_x_str[10] = "", form_pos_y_str[10] = "";
    
    AnimacaoQueda animacao_atual = { .ativa = false };
    AnimacaoEntrega animacao_entrega = { .ativa = false };
    Produto* produto_em_animacao = NULL;
    double tempo_inicio_porta = 0;

    int botao_pagamento_pressionado = 0;
    double tempo_feedback_botao = 0;
    const double DURACAO_FEEDBACK_BOTAO = 0.15;

    al_start_timer(timer);

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;

            if (botao_pagamento_pressionado != 0 && al_get_time() > tempo_feedback_botao) {
                botao_pagamento_pressionado = 0;
            }
            
            if (estado_atual == ANIMACAO_QUEDA && animacao_atual.ativa) {
                animacao_atual.y += animacao_atual.velocidade_y;
                animacao_atual.angulo += animacao_atual.velocidade_angulo;
                if (animacao_atual.y >= animacao_atual.y_final) {
                    animacao_atual.ativa = false;
                    estado_atual = ANIMACAO_PORTA;
                    tempo_inicio_porta = al_get_time();
                }
            } 
            else if (estado_atual == ANIMACAO_PORTA) {
                if (al_get_time() - tempo_inicio_porta > 0.7) {
                    estado_atual = ANIMACAO_ENTREGA;
                    animacao_entrega.ativa = true;
                    animacao_entrega.bitmap = produto_em_animacao->imagem_animacao;
                    animacao_entrega.x = PORTA_X + 40;
                    animacao_entrega.y = PORTA_Y + 10;
                    animacao_entrega.escala = 0.5f;
                    animacao_entrega.alpha = 3.0f;
                    animacao_entrega.velocidade_x = 0.5f;
                    animacao_entrega.velocidade_escala = 0.05f;
                    animacao_entrega.escala_final = 6.8f;
                }
            }
            else if (estado_atual == ANIMACAO_ENTREGA && animacao_entrega.ativa) {
                animacao_entrega.x += animacao_entrega.velocidade_x;
                animacao_entrega.escala += animacao_entrega.velocidade_escala;
                animacao_entrega.alpha -= 0.015f; 

                if (animacao_entrega.alpha < 0) { 
                    animacao_entrega.alpha = 0;
                    animacao_entrega.ativa = false;
                    produto_em_animacao = NULL;
                    estado_atual = MENU_PRINCIPAL;
                }
            }
            
            frames_cursor = (frames_cursor + 1) % (int)FPS;
            if (frames_cursor == 0 || frames_cursor == FPS / 2) { cursor_visivel = !cursor_visivel; }
            if (al_get_time() > tempo_proxima_propaganda) {
                const char* proxima = desenfileirar(&propagandas);
                if (proxima) { enfileirar(&propagandas, proxima); propaganda_atual = frente_fila(&propagandas); }
                tempo_proxima_propaganda = al_get_time() + 5.0;
            }
        } else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            State estado_anterior = estado_atual;
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                if (estado_atual == MENU_PRINCIPAL) rodando = false;
                else if (estado_atual == TELA_SENHA) estado_atual = MENU_PRINCIPAL;
                else if (estado_atual == CONFIGURACAO_MENU) estado_atual = MENU_PRINCIPAL;
                else if (estado_atual == CONFIG_LISTAR_PRODUTOS) estado_atual = CONFIGURACAO_MENU;
                else if (estado_atual == CONFIG_ADICIONAR_PRODUTO) estado_atual = CONFIGURACAO_MENU;
                else if (estado_atual == CONFIG_REMOVER_PRODUTO) estado_atual = CONFIGURACAO_MENU;
                else if (estado_atual == CONFIG_ATUALIZAR_ESTOQUE) estado_atual = CONFIGURACAO_MENU;
                else if (estado_atual == ESTATISTICAS) estado_atual = MENU_PRINCIPAL;
                else if (estado_atual == SELECAO_PRODUTO) estado_atual = MENU_PRINCIPAL;
                else if (estado_atual == PAGAMENTO) {
                    if (valor_inserido_pagamento > 0) {
                        sprintf(mensagem_tela, "Cancelado. R$%.2f devolvido.", valor_inserido_pagamento);
                        cor_mensagem = al_map_rgb(255, 165, 0);
                        tempo_mensagem = al_get_time() + 3.0;
                    }
                    ctx->produto_selecionado = NULL; estado_atual = SELECAO_PRODUTO;
                }
            }
            
            if (estado_atual == MENU_PRINCIPAL) {
                switch (evento.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        menu_principal_indice_selecionado = (menu_principal_indice_selecionado + 3) % 4;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        menu_principal_indice_selecionado = (menu_principal_indice_selecionado + 1) % 4;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        switch (menu_principal_indice_selecionado) {
                            case 0: 
                                estado_atual = SELECAO_PRODUTO; 
                                indice_selecionado = 0; 
                                break;
                            case 1:
                                estado_atual = TELA_SENHA; 
                                strcpy(senha_digitada, ""); 
                                break;
                            case 2: 
                                estado_atual = ESTATISTICAS; 
                                break;
                            case 3:
                                rodando = false; 
                                break;
                        }
                        break;
                }
            } else if (estado_atual == SELECAO_PRODUTO) {
                int num_produtos = 0; Produto* temp = ctx->lista_produtos; while(temp) { num_produtos++; temp = temp->prox; }
                switch (evento.keyboard.keycode) {
                    case ALLEGRO_KEY_UP: indice_selecionado--; if (indice_selecionado < 0) indice_selecionado = 0; break;
                    case ALLEGRO_KEY_DOWN: indice_selecionado++; if (indice_selecionado >= num_produtos) indice_selecionado = num_produtos - 1; break;
                    case ALLEGRO_KEY_ENTER:
                        temp = ctx->lista_produtos; for(int i=0; i < indice_selecionado && temp != NULL; i++) { temp = temp->prox; }
                        if (temp != NULL && temp->estoque > 0) {
                            ctx->produto_selecionado = temp; valor_inserido_pagamento = 0.0f; estado_atual = PAGAMENTO;
                        } else {
                            strcpy(mensagem_tela, "Produto fora de estoque!");
                            cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                        }
                        break;
                }
            } else if (estado_atual == TELA_SENHA) {
                 if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (strcmp(senha_digitada, SENHA_PADRAO) == 0) {
                        estado_atual = CONFIGURACAO_MENU;
                    } else {
                        strcpy(mensagem_tela, "Senha Incorreta!"); cor_mensagem = al_map_rgb(255, 50, 50);
                        tempo_mensagem = al_get_time() + 2.0; strcpy(senha_digitada, "");
                    }
                } else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    if (strlen(senha_digitada) > 0) { senha_digitada[strlen(senha_digitada) - 1] = '\0'; }
                }
            } else if (estado_atual == CONFIGURACAO_MENU) {
                switch (evento.keyboard.keycode) {
                    case ALLEGRO_KEY_UP: config_indice_selecionado = (config_indice_selecionado + 3) % 4; break;
                    case ALLEGRO_KEY_DOWN: config_indice_selecionado = (config_indice_selecionado + 1) % 4; break;
                    case ALLEGRO_KEY_ENTER:
                        if (config_indice_selecionado == 0) {
                            estado_atual = CONFIG_ADICIONAR_PRODUTO; campo_ativo_form = 0;
                            strcpy(form_id_str, ""); strcpy(form_nome_str, ""); strcpy(form_preco_str, ""); strcpy(form_estoque_str, "");
                            strcpy(form_img_path_str, ""); strcpy(form_pos_x_str, ""); strcpy(form_pos_y_str, "");
                        } else if (config_indice_selecionado == 1) {
                            estado_atual = CONFIG_REMOVER_PRODUTO; strcpy(form_id_str, "");
                        } else if (config_indice_selecionado == 2) {
                            estado_atual = CONFIG_LISTAR_PRODUTOS;
                        } else if (config_indice_selecionado == 3) {
                            estado_atual = CONFIG_ATUALIZAR_ESTOQUE; campo_ativo_form = 0;
                            strcpy(form_id_str, ""); strcpy(form_estoque_str, "");
                        }
                        break;
                }
            } else if (estado_atual == CONFIG_ADICIONAR_PRODUTO) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_TAB) {
                    campo_ativo_form = (campo_ativo_form + 1) % 7;
                } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    int id = atoi(form_id_str); float preco = atof(form_preco_str); int estoque = atoi(form_estoque_str);
                    float pos_x = atof(form_pos_x_str); float pos_y = atof(form_pos_y_str);
                    if (id <= 0 || strlen(form_nome_str) == 0 || preco <= 0 || estoque < 0 || strlen(form_img_path_str) == 0) {
                        strcpy(mensagem_tela, "ERRO: Campos inválidos!");
                        cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                    } else if (buscar_produto(ctx->lista_produtos, id) != NULL) {
                        strcpy(mensagem_tela, "ERRO: ID já existe!");
                        cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                    } else {
                        ctx->lista_produtos = adicionar_produto(ctx->lista_produtos, id, form_nome_str, preco, estoque, form_img_path_str, pos_x, pos_y);
                        strcpy(mensagem_tela, "Produto adicionado!");
                        cor_mensagem = al_map_rgb(50, 255, 50); tempo_mensagem = al_get_time() + 3.0;
                        estado_atual = CONFIGURACAO_MENU;
                    }
                } else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    char* target_str = NULL;
                    if (campo_ativo_form == 0) target_str = form_id_str;
                    else if (campo_ativo_form == 1) target_str = form_nome_str;
                    else if (campo_ativo_form == 2) target_str = form_preco_str;
                    else if (campo_ativo_form == 3) target_str = form_estoque_str;
                    else if (campo_ativo_form == 4) target_str = form_img_path_str;
                    else if (campo_ativo_form == 5) target_str = form_pos_x_str;
                    else if (campo_ativo_form == 6) target_str = form_pos_y_str;
                    if (target_str && strlen(target_str) > 0) { target_str[strlen(target_str) - 1] = '\0'; }
                }
            } else if (estado_atual == CONFIG_REMOVER_PRODUTO) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    int id = atoi(form_id_str);
                    if (id <= 0 || buscar_produto(ctx->lista_produtos, id) == NULL) {
                        strcpy(mensagem_tela, "ERRO: Produto não encontrado!");
                        cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                    } else {
                        ctx->lista_produtos = remover_produto(ctx->lista_produtos, id);
                        strcpy(mensagem_tela, "Produto removido!");
                        cor_mensagem = al_map_rgb(50, 255, 50); tempo_mensagem = al_get_time() + 3.0;
                        estado_atual = CONFIGURACAO_MENU;
                    }
                } else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    if (strlen(form_id_str) > 0) { form_id_str[strlen(form_id_str) - 1] = '\0'; }
                }
            } else if (estado_atual == CONFIG_ATUALIZAR_ESTOQUE) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_TAB) {
                    campo_ativo_form = (campo_ativo_form + 1) % 2;
                } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    int id = atoi(form_id_str); int novo_estoque = atoi(form_estoque_str);
                    Produto* produto_alvo = buscar_produto(ctx->lista_produtos, id);
                    if (produto_alvo == NULL) {
                        strcpy(mensagem_tela, "ERRO: Produto não encontrado!");
                        cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                    } else if (novo_estoque < 0) {
                        strcpy(mensagem_tela, "ERRO: Estoque negativo!");
                        cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                    } else {
                        produto_alvo->estoque = novo_estoque;
                        strcpy(mensagem_tela, "Estoque atualizado!");
                        cor_mensagem = al_map_rgb(50, 255, 50); tempo_mensagem = al_get_time() + 3.0;
                        estado_atual = CONFIGURACAO_MENU;
                    }
                } else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    char* target_str = (campo_ativo_form == 0) ? form_id_str : form_estoque_str;
                    if (strlen(target_str) > 0) { target_str[strlen(target_str) - 1] = '\0'; }
                }
            }
            if (estado_atual != estado_anterior) { tempo_mudanca_estado = al_get_time(); }
        } else if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (al_get_time() - tempo_mudanca_estado < 0.05) continue;

            if (estado_atual == TELA_SENHA) {
                if (evento.keyboard.unichar >= 32 && evento.keyboard.unichar <= 126) {
                    if (strlen(senha_digitada) < 50) {
                        char temp_char[] = {evento.keyboard.unichar, '\0'}; strcat(senha_digitada, temp_char);
                    }
                }
            } else if (estado_atual == CONFIG_ADICIONAR_PRODUTO) {
                char* target_str = NULL; int max_len = 0;
                if (campo_ativo_form == 0) { target_str = form_id_str; max_len = 10; }
                else if (campo_ativo_form == 1) { target_str = form_nome_str; max_len = 50; }
                else if (campo_ativo_form == 2) { target_str = form_preco_str; max_len = 10; }
                else if (campo_ativo_form == 3) { target_str = form_estoque_str; max_len = 10; }
                else if (campo_ativo_form == 4) { target_str = form_img_path_str; max_len = 99; }
                else if (campo_ativo_form == 5) { target_str = form_pos_x_str; max_len = 9; }
                else if (campo_ativo_form == 6) { target_str = form_pos_y_str; max_len = 9; }

                if (target_str && evento.keyboard.unichar >= 32) {
                    if (strlen(target_str) < max_len) {
                        char temp_char[] = {evento.keyboard.unichar, '\0'}; strcat(target_str, temp_char);
                    }
                }
            } else if (estado_atual == CONFIG_REMOVER_PRODUTO || estado_atual == CONFIG_ATUALIZAR_ESTOQUE) {
                char* target_str = (estado_atual == CONFIG_REMOVER_PRODUTO || campo_ativo_form == 0) ? form_id_str : form_estoque_str;
                if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9') {
                    if (strlen(target_str) < 10) {
                        char temp_char[] = {evento.keyboard.unichar, '\0'}; strcat(target_str, temp_char);
                    }
                }
            }
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (estado_atual == PAGAMENTO) {
                int mx = evento.mouse.x; int my = evento.mouse.y;

                int centro_x = TELA_X + TELA_LARGURA / 2;
                int y_botoes = TELA_Y + 80;
                float padding = 5;
                float button_width = (TELA_LARGURA - 4 * padding) / 3.0;
                
                float x_b1 = TELA_X + padding;
                float x_b2 = x_b1 + button_width + padding;
                float x_b3 = x_b2 + button_width + padding;

                if (my >= y_botoes && my <= y_botoes + 25) {
                    if (mx >= x_b1 && mx <= x_b1 + button_width) {
                        valor_inserido_pagamento += 1.00;
                        botao_pagamento_pressionado = 1;
                        tempo_feedback_botao = al_get_time() + DURACAO_FEEDBACK_BOTAO;
                    }
                    if (mx >= x_b2 && mx <= x_b2 + button_width) {
                        valor_inserido_pagamento += 0.50;
                        botao_pagamento_pressionado = 2;
                        tempo_feedback_botao = al_get_time() + DURACAO_FEEDBACK_BOTAO;
                    }
                    if (mx >= x_b3 && mx <= x_b3 + button_width) {
                        valor_inserido_pagamento += 0.25;
                        botao_pagamento_pressionado = 3;
                        tempo_feedback_botao = al_get_time() + DURACAO_FEEDBACK_BOTAO;
                    }
                }

                int y_pagar = y_botoes + 35;
                float pagar_width = button_width * 2 + padding;
                float x_pagar_start = centro_x - pagar_width / 2;

                if (mx >= x_pagar_start && mx <= x_pagar_start + pagar_width && my >= y_pagar && my <= y_pagar + 25) {
                    botao_pagamento_pressionado = 4;
                    tempo_feedback_botao = al_get_time() + DURACAO_FEEDBACK_BOTAO;
                    float troco = calcular_troco(valor_inserido_pagamento, ctx->produto_selecionado->preco);
                    if (troco >= 0) {
                        ctx->produto_selecionado->estoque--;
                        char timestamp[20]; time_t now = time(NULL); strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
                        ctx->logs_vendas = adicionar_log(ctx->logs_vendas, timestamp, ctx->produto_selecionado->id, valor_inserido_pagamento, troco);
                        
                        produto_em_animacao = ctx->produto_selecionado;
                        animacao_atual.bitmap = produto_em_animacao->imagem_animacao;

                        if (animacao_atual.bitmap) {
                            float largura_na_vitrine = 30.0f;
                            float altura_na_vitrine = 55.0f;

                            animacao_atual.x = produto_em_animacao->pos_x_inicial + (largura_na_vitrine / 2.0f);
                            animacao_atual.y = produto_em_animacao->pos_y_inicial + (altura_na_vitrine / 2.0f);
                            animacao_atual.angulo = (rand() % 2 == 0 ? -0.2f : 0.2f);
                            animacao_atual.velocidade_y = 4.0f;
                            animacao_atual.velocidade_angulo = (rand() % 2 == 0 ? -0.03f : 0.03f);
                            animacao_atual.y_final = 480;
                            animacao_atual.ativa = true;
                            estado_atual = ANIMACAO_QUEDA;
                        } else {
                            estado_atual = MENU_PRINCIPAL;
                        }

                        sprintf(mensagem_tela, "Sucesso! Troco: R$%.2f", troco);
                        cor_mensagem = al_map_rgb(50, 255, 50);
                        tempo_mensagem = al_get_time() + 3.0;
                        
                    } else {
                        strcpy(mensagem_tela, "Valor insuficiente!"); cor_mensagem = al_map_rgb(255, 50, 50); tempo_mensagem = al_get_time() + 3.0;
                    }
                }
            }
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;
            
            if (imagem_fundo) { al_draw_bitmap(imagem_fundo, 0, 0, 0); }
            else { al_clear_to_color(al_map_rgb(20, 20, 40)); }

            desenhar_produtos_estaticos(ctx->lista_produtos, &animacao_atual);
            
            al_draw_filled_rectangle(TELA_X, TELA_Y, TELA_X + TELA_LARGURA, TELA_Y + TELA_ALTURA, al_map_rgb(15, 15, 25));
            al_draw_filled_rectangle(PROPAGANDA_X, PROPAGANDA_Y, PROPAGANDA_X + PROPAGANDA_LARGURA, PROPAGANDA_Y + PROPAGANDA_ALTURA, al_map_rgb(15, 15, 25));
            desenhar_tela_propaganda(fonte_propaganda, propaganda_atual);

            if (estado_atual == ANIMACAO_QUEDA && animacao_atual.ativa) {
                float largura_desejada_queda = 50.0f;
                float altura_desejada_queda = 50.0f;
                float sw = al_get_bitmap_width(animacao_atual.bitmap);
                float sh = al_get_bitmap_height(animacao_atual.bitmap);
                float escala_x = largura_desejada_queda / sw;
                float escala_y = altura_desejada_queda / sh;
                float centro_bmp_x = sw / 2.0;
                float centro_bmp_y = sh / 2.0;
                al_draw_scaled_rotated_bitmap(animacao_atual.bitmap, centro_bmp_x, centro_bmp_y, animacao_atual.x, animacao_atual.y, escala_x, escala_y, animacao_atual.angulo, 0);
            }
            else if (estado_atual == ANIMACAO_PORTA && porta_semi_aberta && porta_aberta) {
                double tempo_decorrido = al_get_time() - tempo_inicio_porta;
                if (tempo_decorrido < 0.35) {
                    al_draw_bitmap(porta_semi_aberta, PORTA_X - 77.0f, PORTA_Y - 45.0f, 0);
                } else {
                    al_draw_bitmap(porta_aberta, PORTA_X - 77.0f, PORTA_Y - 45.0f, 0);
                }
            }
            else if (estado_atual == ANIMACAO_ENTREGA && animacao_entrega.ativa) {
                if(porta_aberta) al_draw_bitmap(porta_aberta, PORTA_X - 77.0f, PORTA_Y - 45.0f, 0);
                
                float largura_base_entrega = 50.0f;
                float altura_base_entrega = 50.0f;
                float sw = al_get_bitmap_width(animacao_entrega.bitmap);
                float sh = al_get_bitmap_height(animacao_entrega.bitmap);
                float largura_final = largura_base_entrega * animacao_entrega.escala;
                float altura_final = altura_base_entrega * animacao_entrega.escala;
                ALLEGRO_COLOR cor_tinta = al_map_rgba_f(1.0, 1.0, 1.0, animacao_entrega.alpha);
                al_draw_tinted_scaled_bitmap(animacao_entrega.bitmap, cor_tinta, 0, 0, sw, sh, animacao_entrega.x, animacao_entrega.y, largura_final, altura_final, 0);
            }

            al_set_clipping_rectangle(TELA_X, TELA_Y, TELA_LARGURA, TELA_ALTURA);
            
            if (estado_atual < ANIMACAO_QUEDA) {
                if (estado_atual == MENU_PRINCIPAL) {
                    desenhar_menu_principal(fonte_geral, menu_principal_indice_selecionado);
                } else if (estado_atual == SELECAO_PRODUTO) {
                    desenhar_tela_produtos(fonte_geral, ctx->lista_produtos, indice_selecionado);
                } else if (estado_atual == PAGAMENTO) {
                    desenhar_tela_pagamento(fonte_geral, ctx->produto_selecionado, valor_inserido_pagamento, botao_pagamento_pressionado);
                } else if (estado_atual == ESTATISTICAS) {
                    desenhar_tela_estatisticas(fonte_geral, ctx);
                } else if (estado_atual == TELA_SENHA) {
                    desenhar_tela_senha(fonte_geral, senha_digitada, cursor_visivel);
                } else if (estado_atual == CONFIGURACAO_MENU) {
                    desenhar_menu_config(fonte_geral, config_indice_selecionado);
                } else if (estado_atual == CONFIG_LISTAR_PRODUTOS) {
                    desenhar_tela_produtos(fonte_geral, ctx->lista_produtos, -1);
                } else if (estado_atual == CONFIG_ADICIONAR_PRODUTO) {
                    desenhar_tela_add_produto(fonte_geral, form_id_str, form_nome_str, form_preco_str, form_estoque_str, form_img_path_str, form_pos_x_str, form_pos_y_str, campo_ativo_form, cursor_visivel);
                } else if (estado_atual == CONFIG_REMOVER_PRODUTO) {
                    desenhar_tela_remove_produto(fonte_geral, form_id_str, cursor_visivel);
                } else if (estado_atual == CONFIG_ATUALIZAR_ESTOQUE) {
                    desenhar_tela_atualizar_estoque(fonte_geral, form_id_str, form_estoque_str, campo_ativo_form, cursor_visivel);
                }
            }
            
            al_set_clipping_rectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA);

            if (al_get_time() < tempo_mensagem) {
                int msg_box_w = TELA_LARGURA * 0.9; 
                int msg_box_h = 50;
                int msg_x = TELA_X + TELA_LARGURA / 2;
                int msg_y = TELA_Y + TELA_ALTURA / 2;

                al_draw_filled_rounded_rectangle(msg_x - msg_box_w/2, msg_y - msg_box_h/2, msg_x + msg_box_w/2, msg_y + msg_box_h/2, 6, 6, al_map_rgba(0,0,0,220));
                al_draw_text(fonte_geral, cor_mensagem, msg_x, msg_y - (al_get_font_line_height(fonte_geral)/2), ALLEGRO_ALIGN_CENTER, mensagem_tela);
            }

            al_flip_display();
        }
    }

    salvar_produtos_csv(ctx->lista_produtos, "produtos.csv");
    salvar_estatisticas_csv(ctx->logs_vendas, "estatisticas.csv");
    salvar_logs("vendas.bin", ctx->logs_vendas);
    liberar_lista_produtos(ctx->lista_produtos);
    liberar_logs(ctx->logs_vendas);
    free(ctx);
    al_destroy_bitmap(imagem_fundo);
    if(porta_semi_aberta) al_destroy_bitmap(porta_semi_aberta);
    if(porta_aberta) al_destroy_bitmap(porta_aberta);
    al_destroy_font(fonte_geral);
    al_destroy_font(fonte_propaganda);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}