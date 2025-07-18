# PROG2
Página para adicionar projetos e atividades realizadas na matéria de programação 2

# 🤖 Máquina de Refrigerantes - Simulação em C

Simulação de uma máquina de vendas automática desenvolvida em C, com foco em arquitetura de software, máquinas de estado e uso de estruturas de dados.

| Característica | Detalhes |
| :--- | :--- |
| **Linguagem** | C |
| **Interface** | Linha de Comando (CLI) |
| **Dependências** | Nenhuma! Apenas `gcc` e `make` |
| **Persistência** | Arquivos `.csv` e `.bin` |
| **Senha Admin** | `admin123` |

## 🖼️ Demonstração Visual

Veja abaixo como a aplicação se parece em execução.

<p align="center">
  <em>Tela Principal e Menu de Produtos</em><br><br>
  <img src="Maquina de refrigerantes/Firmware/images/menu_principal.png" alt="Menu Principal" width="45%"/>
  <img src="Maquina de refrigerantes/Firmware/images/selecao_produto.png" alt="Seleção de Produto" width="45%"/>
</p>
<br>
<p align="center">
<em>Tela de Pagamento</em><br><br>
<p align="center">
  <img src="Maquina de refrigerantes/Firmware/images/pagamento.png" alt="Tela de Pagamento" width="60%"/>
</p>
<br>
<p align="center">
<em>Menu Administrativo</em><br><br>
<p align="center">
  <img src="Maquina de refrigerantes/Firmware/images/menu_admin.png" alt="Menu Administrativo" width="60%"/>
</p>


## ⚙️ Guia de Uso da Máquina

A máquina foi projetada para ser intuitiva. Abaixo, um passo a passo detalhado de como realizar uma compra.

### **Passo 1: Acessando a Lista de Produtos** 🛒

Ao iniciar a aplicação, você verá o menu principal. Para começar uma compra, você deve entrar na tela de seleção de produtos.

1.  No menu principal, **digite `1`** e pressione **Enter**.

    > ```
    > 1. Selecionar produto
    > 2. Configurações
    > 3. Ver estatísticas
    > 4. Sair
    > Escolha: 1
    > ```

2.  A máquina irá limpar a tela e exibir todos os produtos disponíveis, com seus respectivos códigos (ID), nome, preço e quantidade em estoque.

    > ```
    > --- LISTA DE PRODUTOS ---
    > ID: 3 | Nome: Coca          | Preço: R$2.00 | Estoque: 10
    > ID: 2 | Nome: Guaraná Jesus | Preço: R$1.50 | Estoque: 9
    > ID: 1 | Nome: Pureza        | Preço: R$1.50 | Estoque: 10
    > -------------------------
    > ID do produto (0 para voltar):
    > ```

### **Passo 2: Escolhendo o Produto** 🥤

Com a lista de produtos em tela, escolha o que você deseja.

1.  Digite o **ID do produto** desejado e pressione **Enter**. Para este exemplo, vamos escolher "Guaraná Jesus" (`ID: 2`).

    > ```
    > ID do produto (0 para voltar): 2
    > ```

2.  O sistema confirmará sua seleção e o levará para a tela de pagamento.

### **Passo 3: Realizando o Pagamento** 💰

Esta tela é onde você insere o dinheiro. A máquina aceita moedas específicas e oferece opções de controle.

-   **Moedas aceitas:** `0.25`, `0.50`, `1.00`.
-   **Comandos Especiais:**
    -   Digite `-1` para **desfazer** a última moeda inserida.
    -   Digite `-2` para **cancelar** a compra e receber seu dinheiro de volta.
    -   Digite `0` quando tiver inserido o valor suficiente para **finalizar** o pagamento.

1.  Insira as moedas uma a uma, pressionando **Enter** após cada uma. O total inserido será atualizado a cada inserção.

    > ```
    > --- PAGAMENTO ---
    > Produto: Guaraná Jesus | Preço: R$1.50
    > Insira moedas (0.25, 0.5, 1.0). Digite 0 para concluir, -1 para desfazer, -2 para cancelar:
    > Moeda: 1.00
    > Total inserido: R$1.00
    > Moeda: 0.25
    > Total inserido: R$1.25
    > Moeda: -1
    > Desfeita moeda de R$0.25. Total: R$1.00
    > Moeda: 0.50
    > Total inserido: R$1.50
    > ```

2.  Quando o valor for igual ou superior ao preço do produto, **digite `0`** e pressione **Enter** para confirmar.

### **Passo 4: Finalizando a Compra** 🎉

Após confirmar o pagamento, a máquina processará a transação.

1.  O sistema exibirá uma mensagem de sucesso e o valor do seu troco.

    > ```
    > Compra realizada! Troco: R$0.00
    >
    > Pressione Enter para continuar...
    > ```

2.  Após pressionar **Enter**, você será redirecionado para o menu principal, pronto para uma nova operação. O estoque do produto comprado será atualizado.

## 🛠️ Como Compilar e Executar

Este projeto não tem dependências externas. Tudo que você precisa é um compilador C e da ferramenta `make`.

### Requisitos

1.  **Compilador GCC**: Para instalar em sistemas baseados em Debian/Ubuntu:
    ```bash
    sudo apt update
    sudo apt install build-essential
    ```
2.  **Make**: Geralmente instalado junto com o `build-essential`.

### Passo a Passo

1.  **Clone o repositório:**
    ```bash
    git clone <URL_DO_SEU_REPOSITORIO>
    cd <NOME_DA_PASTA_DO_PROJETO>
    ```

2.  **Compile o projeto:**
    O `Makefile` automatiza todo o processo.
    ```bash
    make
    ```

3.  **Execute a simulação:**
    ```bash
    ./vending
    ```

4.  **Limpeza (Opcional):**
    Remove os arquivos de objeto (`.o`) e o executável.
    ```bash
    make clean
    ```

## 📂 Estrutura do Projeto

O código é modularizado para facilitar a manutenção e o entendimento.

| Módulo | Responsabilidade |
| :--- | :--- |
| **`main.c`** | Ponto de entrada, gerencia a máquina de estados. |
| **`states / .c e .h`** | Define os diferentes estados da aplicação (MENU, PAGAMENTO, etc.). |
| **`ui / .c e .h`** | Funções de controle da interface no terminal. |
| **`lista_produtos / .c e .h`** | Gerencia a lista duplamente encadeada de produtos. |
| **`pagamento/ .c e .h`** | Controla a lógica de pagamento e troco. |
| **`pilha / .c e .h`** | Implementações das estruturas de dados de pilha. |
| **`config / .c e .h`** | Lógica do menu administrativo (adicionar/remover produtos). |
| **`estatisticas / .c e .h`** | Geração e exibição de estatísticas de vendas. |
| **`log_vendas / .c e .h`** | Gravação e leitura do histórico de vendas. |
| **`fila / .c e .h`** | Implementações das filas para uso de propagandas. |
