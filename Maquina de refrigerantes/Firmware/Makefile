# Nome do compilador
CC = gcc

# Nome do executável que será gerado
EXEC = maquina_allegro

# Flags do compilador: -Wall (mostra todos os avisos), -g (inclui informações para debug)
# Adicionamos as flags do Allegro usando pkg-config
CFLAGS = -Wall -g $(shell pkg-config --cflags allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5)

# Flags do linker e bibliotecas do Allegro
LDFLAGS =
LIBS = $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5)

# Lista de todos os seus arquivos de código-fonte (.c)
SRCS =  main.c \
		states.c \
		config.c \
		estatisticas.c \
		fila.c \
		lista_produtos.c \
		log_vendas.c \
		pagamento.c \
		pilha.c \
		ui.c

# Gera a lista de arquivos objeto (.o) a partir dos arquivos de código (.c)
OBJS = $(SRCS:.c=.o)

# --- REGRAS ---

# A regra padrão
all: $(EXEC)

# Regra para linkar e criar o executável final
$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

# Regra genérica para compilar um arquivo .c em um arquivo .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados (executada com 'make clean')
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean