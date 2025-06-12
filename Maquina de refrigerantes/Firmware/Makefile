CC = gcc
CFLAGS = -Wall -g

OBJS = main.o states.o ui.o fila.o pilha.o lista_produtos.o log_vendas.o pagamento.o config.o estatisticas.o

all: vending

vending: $(OBJS)
	$(CC) $(CFLAGS) -o vending $(OBJS)

main.o: main.c states.h ui.h
states.o: states.c states.h ui.h
ui.o: ui.c ui.h
fila.o: fila.c fila.h
pilha.o: pilha.c pilha.h
lista_produtos.o: lista_produtos.c lista_produtos.h
log_vendas.o: log_vendas.c log_vendas.h
pagamento.o: pagamento.c pagamento.h
config.o: config.c config.h lista_produtos.h
estatisticas.o: estatisticas.c estatisticas.h log_vendas.h

clean:
	rm -f *.o vending
