/*  Desenvolvido por: Northon Duarte*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Definições globais
#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

// Estrutura do Item (Nível Novato + Prioridade do Mestre)
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // 1 a 5
} Item;

// Variáveis Globais de Controle
Item mochila[MAX_ITENS];
int totalItens = 0;
bool ordenadoPorNome = false; // Flag para validar busca binária

// --- Funções Auxiliares ---

// Limpa o buffer do teclado para evitar erros no scanf/fgets
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Funcionalidades Nível Novato ---

void adicionarItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\n[!] A mochila está cheia! Descarte algo primeiro.\n");
        return;
    }

    Item novoItem;
    
    printf("\n--- Adicionando Item ---\n");
    printf("Nome: ");
    scanf(" %29[^\n]", novoItem.nome); // Lê string com espaços
    
    printf("Tipo (ex: Arma, Cura, Municao): ");
    scanf(" %19[^\n]", novoItem.tipo);
    
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    do {
        printf("Prioridade (1-Baixa a 5-Alta): ");
        scanf("%d", &novoItem.prioridade);
    } while (novoItem.prioridade < 1 || novoItem.prioridade > 5);

    mochila[totalItens] = novoItem;
    totalItens++;
    ordenadoPorNome = false; // Ao adicionar, a lista pode perder a ordenação
    printf("[+] Item '%s' adicionado com sucesso!\n", novoItem.nome);
}

void listarItens() {
    if (totalItens == 0) {
        printf("\n[!] A mochila está vazia.\n");
        return;
    }

    printf("\n--- Conteúdo da Mochila ---\n");
    printf("%-20s | %-15s | %-3s | %-3s\n", "Nome", "Tipo", "Qtd", "Prio");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%-20s | %-15s | %03d | %d\n", 
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}

void removerItem() {
    char nomeAlvo[MAX_NOME];
    printf("\nNome do item para remover: ");
    scanf(" %29[^\n]", nomeAlvo);

    int encontrado = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeAlvo) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Move os itens subsequentes para trás para preencher o buraco
        for (int i = encontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        printf("[-] Item '%s' removido.\n", nomeAlvo);
    } else {
        printf("[!] Item não encontrado.\n");
    }
}

// --- Funcionalidades Nível Aventureiro ---

void buscaSequencial() {
    char nomeBusca[MAX_NOME];
    printf("\n[Busca Sequencial] Digite o nome: ");
    scanf(" %29[^\n]", nomeBusca);

    bool achou = false;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n=== Item Encontrado ===\n");
            printf("Nome: %s\nTipo: %s\nQtd: %d\nPrioridade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
            achou = true;
            break; // Para assim que achar
        }
    }

    if (!achou) {
        printf("[!] Item '%s' não está na mochila.\n", nomeBusca);
    }
}

// --- Funcionalidades Nível Mestre ---

// Algoritmo Insertion Sort
void ordenarItens() {
    if (totalItens < 2) {
        printf("\n[!] Itens insuficientes para ordenar.\n");
        return;
    }

    int opcao;
    printf("\nOrdenar por: [1] Nome  [2] Prioridade: ");
    scanf("%d", &opcao);

    int comparacoes = 0;
    int i, j;
    Item pivo;

    for (i = 1; i < totalItens; i++) {
        pivo = mochila[i];
        j = i - 1;

        // Lógica do Insertion Sort
        bool condicao = false;
        while (j >= 0) {
            comparacoes++; // Contando comparações
            
            if (opcao == 1) {
                // Ordenação alfabética (strcmp > 0 significa que pivo é "menor/anterior")
                if (strcmp(mochila[j].nome, pivo.nome) > 0) condicao = true;
                else condicao = false;
            } else {
                // Ordenação por prioridade (maior prioridade primeiro)
                if (mochila[j].prioridade < pivo.prioridade) condicao = true;
                else condicao = false;
            }

            if (condicao) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break;
            }
        }
        mochila[j + 1] = pivo;
    }

    printf("\n[v] Mochila ordenada! Comparações realizadas: %d\n", comparacoes);
    
    if (opcao == 1) {
        ordenadoPorNome = true;
    } else {
        ordenadoPorNome = false; // Se ordenou por prioridade, bagunçou os nomes
    }
    
    listarItens();
}

void buscaBinaria() {
    // Validação de Pré-requisito
    if (!ordenadoPorNome) {
        printf("\n[Erro] A lista PRECISA estar ordenada por Nome para usar a Busca Binária.\n");
        printf("Vá na opção de Ordenar e escolha 'Por Nome' primeiro.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n[Busca Binária] Digite o nome exato: ");
    scanf(" %29[^\n]", nomeBusca);

    int inicio = 0;
    int fim = totalItens - 1;
    int meio;
    bool achou = false;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        int comparacao = strcmp(mochila[meio].nome, nomeBusca);

        if (comparacao == 0) {
            printf("\n=== Item Encontrado (Binária) ===\n");
            printf("Posição no vetor: %d\n", meio);
            printf("Nome: %s | Qtd: %d | Prioridade: %d\n", 
                   mochila[meio].nome, mochila[meio].quantidade, mochila[meio].prioridade);
            achou = true;
            break;
        } else if (comparacao < 0) {
            // Se o nome do meio é "menor" que o buscado, busca na direita
            inicio = meio + 1;
        } else {
            // Se o nome do meio é "maior", busca na esquerda
            fim = meio - 1;
        }
    }

    if (!achou) {
        printf("[!] Item não encontrado via Busca Binária.\n");
    }
}

// --- Menu Principal ---
int main() {
    int opcao;

    do {
        printf("\n=== MOCHILA FREE FIRE (Nível Mestre) ===\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item (Sequencial)\n");
        printf("5. Ordenar Mochila (Insertion Sort)\n");
        printf("6. Buscar Item (Binária)\n");
        printf("0. Sair da Ilha\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o enter do buffer

        switch(opcao) {
            case 1: adicionarItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: buscaSequencial(); break;
            case 5: ordenarItens(); break;
            case 6: buscaBinaria(); break;
            case 0: printf("Saindo... Boa sorte na ilha!\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
