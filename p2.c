#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Busca uma categoria na lista
NodeCategoria *buscar_categoria(NodeCategoria *head, const char *nome) {
    NodeCategoria *current = head;
    while (current != NULL) {
        if (strcmp(current->nome, nome) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

NodeArvore* inserir_na_arvore(NodeArvore *raiz, float valor, NodeAlimento *alimento) {
    if (raiz == NULL) {
        NodeArvore* novo_no = (NodeArvore*) malloc(sizeof(NodeArvore));

        if (novo_no == NULL) {
            perror("Erro ao alocar memória para o nó da árvore.");
            exit(1);
        }
        novo_no->valor = valor;
        novo_no->alimento = alimento;
        novo_no->esquerda = NULL;
        novo_no->direita = NULL;

        return novo_no;
    }

    if (valor < raiz->valor) {
        raiz->esquerda = inserir_na_arvore(raiz->esquerda, valor, alimento);
    } else {
        raiz->direita = inserir_na_arvore(raiz->direita, valor, alimento);
    }
    return raiz;
}

// Insere categoria em ordem alfabética
void inserir_categoria_ordenada(NodeCategoria **head, const char *nome) {
    NodeCategoria *nova_categoria =
        (NodeCategoria *)malloc(sizeof(NodeCategoria));
    if (nova_categoria == NULL)
        return;

    strcpy(nova_categoria->nome, nome);
    nova_categoria->alimentos = NULL;
    nova_categoria->raiz_energia = NULL;
    nova_categoria->raiz_proteina = NULL;
    nova_categoria->next = NULL;

    // Lista vazia ou inserir no início
    if (*head == NULL || strcmp(nome, (*head)->nome) < 0) {
        nova_categoria->next = *head;
        *head = nova_categoria;
        return;
    }

    // Procura posição correta em ordem alfabética
    NodeCategoria *current = *head;
    while (current->next != NULL && strcmp(nome, current->next->nome) > 0) {
        current = current->next;
    }

    nova_categoria->next = current->next;
    current->next = nova_categoria;
}

// Busca ou cria categoria
NodeCategoria *buscar_ou_criar_categoria(NodeCategoria **head,
                                         const char *nome) {
    NodeCategoria *cat = buscar_categoria(*head, nome);

    if (cat == NULL) {
        inserir_categoria_ordenada(head, nome);
        cat = buscar_categoria(*head, nome);
    }

    return cat;
}

// Insere alimento em ordem alfabética dentro da categoria
NodeAlimento* inserir_alimento_ordenado(NodeCategoria *categoria, Alimento alimento) {
    NodeAlimento *novo_alimento = (NodeAlimento *)malloc(sizeof(NodeAlimento));
    if (novo_alimento == NULL)
        return NULL;

    novo_alimento->dados = alimento;
    novo_alimento->next = NULL;

    // Lista vazia ou inserir no início
    if (categoria->alimentos == NULL ||
        strcmp(alimento.descricao, categoria->alimentos->dados.descricao) < 0) {
        novo_alimento->next = categoria->alimentos;
        categoria->alimentos = novo_alimento;
        return novo_alimento;
    }

    // Procura posição correta em ordem alfabética
    NodeAlimento *current = categoria->alimentos;
    while (current->next != NULL &&
           strcmp(alimento.descricao, current->next->dados.descricao) > 0) {
        current = current->next;
    }

    novo_alimento->next = current->next;
    current->next = novo_alimento;

    return novo_alimento;
}

// Função principal que lê o arquivo e popula tudo
NodeCategoria *ler_arquivo_e_popular(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    NodeCategoria *lista_categorias = NULL;
    Alimento alimento;

    // Lê cada alimento do arquivo
    while (fread(&alimento, sizeof(Alimento), 1, file) == 1) {
        NodeCategoria *categoria =
            buscar_ou_criar_categoria(&lista_categorias, alimento.categoria);

        if (categoria != NULL) {
            NodeAlimento* novo_no_lista = inserir_alimento_ordenado(categoria, alimento);

            if (novo_no_lista != NULL) {
                categoria->raiz_energia = inserir_na_arvore(
                    categoria->raiz_energia,
                    novo_no_lista->dados.energia,
                    novo_no_lista
                );

                categoria->raiz_proteina = inserir_na_arvore(
                    categoria->raiz_proteina,
                    novo_no_lista->dados.proteina,
                    novo_no_lista
                );
            }
        }
    }

    fclose(file);
    return lista_categorias;
}

// Função para imprimir tudo
void imprimir_tudo(NodeCategoria *head) {
    NodeCategoria *cat = head;

    while (cat != NULL) {
        printf("\n======= Categoria: %s\n", cat->nome);

        NodeAlimento *alim = cat->alimentos;
        while (alim != NULL) {
            printf("  - %s (Energia: %.2f, Proteína: %.2f)\n",
                   alim->dados.descricao, alim->dados.energia,
                   alim->dados.proteina);
            alim = alim->next;
        }

        cat = cat->next;
    }
}

void liberar_arvore(NodeArvore* raiz){
    if (raiz == NULL) return;
    NodeArvore* left_tree = raiz->esquerda;
    NodeArvore* right_tree = raiz->direita;

    liberar_arvore(left_tree);
    liberar_arvore(right_tree);

    free(raiz);
}

// Libera toda a memória
void liberar_tudo(NodeCategoria *head) {
    while (head != NULL) {
        NodeCategoria *next_cat = head->next;

        liberar_arvore(head->raiz_energia);
        liberar_arvore(head->raiz_proteina);

        NodeAlimento *alim = head->alimentos;
        while (alim != NULL) {
            NodeAlimento *next_alim = alim->next;
            free(alim);
            alim = next_alim;
        }

        free(head);
        head = next_cat;
    }
}

int main() {
    NodeCategoria *categorias = ler_arquivo_e_popular("dados.bin");
    imprimir_tudo(categorias);
    liberar_tudo(categorias);

    return 0;
}
