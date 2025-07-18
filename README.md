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
  <img src="images/menu_principal.png" alt="Menu Principal" width="45%"/>
  <img src="images/selecao_produto.png" alt="Seleção de Produto" width="45%"/>
</p>
<br>
<p align="center">
  <em>Tela de Pagamento e Menu Administrativo</em><br><br>
  <img src="images/pagamento.png" alt="Tela de Pagamento" width="45%"/>
  <img src="images/menu_admin.png" alt="Menu Administrativo" width="45%"/>
</p>


## ⚙️ Guia de Uso da Máquina

A máquina foi projetada para ser intuitiva. Siga os passos abaixo para comprar um produto.

| Passo | Instruções | Screenshot Sugerido |
|:---|:---|:---|
| **1** | Na tela inicial, pressione a tecla correspondente para **"Comprar Produto"**. Você verá a lista de produtos disponíveis, seus preços e o estoque. | <img src="images/passo1_selecao.png" alt="Selecionando a opção de compra" width="250"/> |
| **2** | Digite o **código do produto** que deseja comprar e pressione Enter. | <img src="images/passo2_codigo.png" alt="Digitando o código do produto" width="250"/> |
| **3** | A tela de pagamento será exibida. **Insira as moedas** digitando os valores (ex: 1.00, 0.50, 0.25) e pressionando Enter. O valor inserido será acumulado. | <img src="images/passo3_moedas.png" alt="Inserindo moedas" width="250"/> |
| **4** | Quando o valor for suficiente, a máquina informará o troco e a **compra será finalizada com sucesso**. Você será redirecionado ao menu principal. | <img src="images/passo4_sucesso.png" alt="Compra finalizada" width="250"/> |

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
