# 🤖 Máquina de Refrigerantes - Simulação Gráfica em C com Allegro 5

Simulação de uma máquina de vendas automática desenvolvida em C, com uma interface gráfica completa utilizando a biblioteca Allegro 5. O projeto demonstra conceitos de arquitetura de software, máquinas de estado, estruturas de dados e gerenciamento de eventos em um ambiente gráfico.

| Característica | Detalhes |
| :--- | :--- |
| **Linguagem** | C |
| **Interface** | Gráfica (GUI) com **Allegro 5** |
| **Dependências** | `gcc`, `make` e a biblioteca `Allegro 5` com seus addons |
| **Persistência** | Arquivos `.csv` e `.bin` |
| **Senha Admin** | `admin123` |

## 🖼️ Demonstração Visual

Veja abaixo a interface gráfica da aplicação em ação. A interação é feita via teclado e mouse, com animações que tornam a experiência mais imersiva.

<p align="center">
  <em>Tela Principal e Animação de Anúncios</em><br><br>
  <img src="Maquina de refrigerantes/Firmware/images/menu_principal.png" alt="Menu Principal" width="100%"/>
</p>
<br>
<p align="center">
  <em>Seleção de Produtos e Tela de Pagamento Interativa</em><br><br>
  <img src="Maquina de refrigerantes/Firmware/images/selecao_produto.png" alt="Seleção de Produto" width="100%"/>
  <img src="Maquina de refrigerantes/Firmware/images/pagamento.png" alt="Tela de Pagamento" width="100%"/>
</p>
<br>
<p align="center">
<em>Animação de Compra: Queda e Entrega do Produto</em><br><br>
<p align="center">
  <img src="Maquina de refrigerantes/Firmware/images/funcionamento.gif" alt="Animação da Compra" width="100%"/>
</p>
<br>
<p align="center">
<em>Menu Administrativo para Gerenciamento de Produtos</em><br><br>
<p align="center">
  <img src="Maquina de refrigerantes/Firmware/images/menu_conf.png" alt="Menu Administrativo" width="100%"/>
</p>


## ⚙️ Guia de Uso da Máquina

A interface foi projetada para ser intuitiva. As interações principais são feitas com as **setas do teclado**, **Enter**, **Esc** e o **mouse**.

### **Passo 1: Navegando no Menu Principal** 🖱️

Ao iniciar a aplicação, você verá o menu principal na tela da máquina.
1.  Use as teclas **↑ (seta para cima)** e **↓ (seta para baixo)** para navegar entre as opções ("COMPRAR", "Configurações", "Estatísticas", "Sair").
2.  Pressione **Enter** na opção "COMPRAR" para iniciar uma compra.

### **Passo 2: Escolhendo o Produto** 🥤

A tela exibirá a lista de produtos disponíveis.
1.  Use as **setas do teclado** para selecionar o produto desejado. A seleção ficará destacada.
2.  Com o produto desejado destacado, pressione **Enter**. Se o produto estiver em estoque, você avançará para a tela de pagamento. Caso contrário, uma mensagem de "Fora de estoque" será exibida.

### **Passo 3: Realizando o Pagamento** 💰

Nesta tela, você insere o dinheiro usando o mouse.
- **Moedas:** Clique nos botões **"1.00"**, **"0.50"** ou **"0.25"** para adicionar dinheiro. O "Total Inserido" será atualizado.
- **Confirmar:** Quando o valor inserido for suficiente, clique no botão **"PAGAR"**.
- **Cancelar:** Para cancelar a compra, pressione a tecla **Esc**. O valor inserido será devolvido.

### **Passo 4: Animação e Finalização da Compra** 🎉

Após confirmar o pagamento:
1.  Uma **animação** será iniciada: o produto selecionado cairá da prateleira, a porta de entrega se abrirá e o item será "entregue" ao usuário, deslizando para fora da tela.
2.  Uma mensagem de sucesso com o valor do troco será exibida.
3.  A aplicação retornará ao menu principal, e o estoque do produto comprado será atualizado.

## 🛠️ Como Compilar e Executar

O projeto depende da biblioteca Allegro 5 e seus addons.

### Requisitos

1.  **Compilador GCC e Make**:
    ```bash
    # Para sistemas Debian/Ubuntu
    sudo apt update
    sudo apt install build-essential
    ```
2.  **Biblioteca Allegro 5**:
    ```bash
    # Para sistemas Debian/Ubuntu, instale a biblioteca principal e os addons
    sudo apt install liballegro5-dev liballegro-image-5-dev liballegro-primitives-5-dev liballegro-font5-dev liballegro-ttf5-dev
    ```

### Passo a Passo

1.  **Clone o repositório:**
    ```bash
    git clone <URL_DO_SEU_REPOSITORIO>
    cd <NOME_DA_PASTA_DO_PROJETO>
    ```

2.  **Compile o projeto:**
    O `Makefile` fornecido automatiza todo o processo.
    ```bash
    make
    ```

3.  **Execute a simulação:**
    O nome do executável é `maquina_allegro`.
    ```bash
    ./maquina_allegro
    ```

4.  **Limpeza (Opcional):**
    Remove os arquivos de objeto (`.o`) e o executável.
    ```bash
    make clean
    ```

## 📂 Estrutura do Projeto

O código é modularizado para facilitar a manutenção e o entendimento.

| Módulo | Arquivos | Responsabilidade |
| :--- | :--- | :--- |
| **Ponto de Entrada** | `main.c` | Ponto de entrada, gerencia a FSM, o laço de eventos e toda a renderização da GUI com Allegro. |
| **Estados** | `states.c`, `states.h` | Define os diferentes estados da aplicação (MENU, PAGAMENTO, ANIMAÇÃO, etc.). |
| **Lista de Produtos**| `lista_produtos.c`, `lista_produtos.h` | Gerencia a lista duplamente encadeada de produtos e o carregamento dos bitmaps (`ALLEGRO_BITMAP`). |
| **Pagamento** | `pagamento.c`, `pagamento.h` | Contém a lógica de `calcular_troco`. A função `inserir_moedas` é um legado da versão console. |
| **Configuração** | `config.c`, `config.h`| Define a senha e a estrutura `Contexto`. A interface de configuração foi reimplementada em `main.c`. |
| **Estatísticas** | `estatisticas.c`, `estatisticas.h` | Geração e salvamento de estatísticas de vendas em `.csv`. |
| **Log de Vendas** | `log_vendas.c`, `log_vendas.h` | Gravação e leitura do histórico de vendas em um arquivo `.bin`. |
| **Fila** | `fila.c`, `fila.h`| Implementação da fila circular usada para exibir as propagandas rotativas. |
| **Pilha** | `pilha.c`, `pilha.h`| Estrutura de dados de pilha, usada na versão console para a função "desfazer". |
| **UI (Legado)** | `ui.c`, `ui.h`| **(Legado)** Funções da interface da versão console. **Não é utilizado** na versão Allegro. |
| **Makefile** | `Makefile` | Script de compilação que lida com as dependências e bibliotecas do Allegro. |
