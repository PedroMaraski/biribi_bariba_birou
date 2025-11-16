#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <locale.h>
#endif

void configurar_console(void) {
    #ifdef _WIN32
        // Configura UTF-8 para o console Windows
        printf("windows\n");
        SetConsoleOutputCP(CP_UTF8);
    #else
        // Para sistemas Unix/Linux
        setlocale(LC_ALL, "pt_BR.UTF-8");
    #endif
}

TipoCategoria string_to_enum(const char *nomeString){
    if (strcmp(nomeString, "Cereais e derivados") == 0) return CEREAIS_E_DERIVADOS;
    if (strcmp(nomeString, "Verduras, hortaliças e derivados") == 0) return VERDURAS_HORTALICAS_E_DERIVADOS;
    if (strcmp(nomeString, "Frutas e derivados") == 0) return FRUTAS_E_DERIVADOS;
    if (strcmp(nomeString, "Gorduras e óleos") == 0) return GORDURAS_E_OLEOS;
    if (strcmp(nomeString, "Pescados e frutos do mar") == 0) return PESCADOS_E_FRUTOS_DO_MAR;
    if (strcmp(nomeString, "Carnes e derivados") == 0) return CARNES_E_DERIVADOS;
    if (strcmp(nomeString, "Leite e derivados") == 0) return LEITE_E_DERIVADOS;
    if (strcmp(nomeString, "Bebidas (alcóolicas e não alcoólicas)") == 0) return BEBIDAS;
    if (strcmp(nomeString, "Ovos e derivados") == 0) return OVOS_E_DERIVADOS;
    if (strcmp(nomeString, "Produtos açucarados") == 0) return PRODUTOS_ACUCARADOS;
    if (strcmp(nomeString, "Miscelâneas") == 0) return MISCELANEAS;
    if (strcmp(nomeString, "Outros alimentos industrializados") == 0) return OUTROS_ALIMENTOS_INDUSTRIALIZADOS;
    if (strcmp(nomeString, "Alimentos preparados") == 0) return ALIMENTOS_PREPARADOS;
    if (strcmp(nomeString, "Leguminosas e derivados") == 0) return LEGUMINOSAS_E_DERIVADOS;
    if (strcmp(nomeString, "Nozes e sementes") == 0) return NOZES_E_SEMENTES;
    return CATEGORIA_INVALIDA;
}

const char* enum_to_string(TipoCategoria tipo) {
    switch (tipo) {
        case CEREAIS_E_DERIVADOS: return "Cereais e derivados";
        case VERDURAS_HORTALICAS_E_DERIVADOS: return "Verduras, hortaliças e derivados";
        case FRUTAS_E_DERIVADOS: return "Frutas e derivados";
        case GORDURAS_E_OLEOS: return "Gorduras e óleos";
        case PESCADOS_E_FRUTOS_DO_MAR: return "Pescados e frutos do mar";
        case CARNES_E_DERIVADOS: return "Carnes e derivados";
        case LEITE_E_DERIVADOS: return "Leite e derivados";
        case BEBIDAS: return "Bebidas (alcóolicas e não alcoólicas)";
        case OVOS_E_DERIVADOS: return "Ovos e derivados";
        case PRODUTOS_ACUCARADOS: return "Produtos açucarados";
        case MISCELANEAS: return "Miscelâneas";
        case OUTROS_ALIMENTOS_INDUSTRIALIZADOS: return "Outros alimentos industrializados";
        case ALIMENTOS_PREPARADOS: return "Alimentos preparados";
        case LEGUMINOSAS_E_DERIVADOS: return "Leguminosas e derivados";
        case NOZES_E_SEMENTES: return "Nozes e sementes";
        default: return "Categoria Desconhecida";
    }
}


// Busca uma categoria na lista
NodeCategoria *buscar_categoria(NodeCategoria *head, TipoCategoria tipo) {
    NodeCategoria *current = head;
    while (current != NULL) {
        if (current->tipo == tipo) {
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
void inserir_categoria_ordenada(NodeCategoria **head, TipoCategoria tipo) {
    NodeCategoria *nova_categoria =
        (NodeCategoria *)malloc(sizeof(NodeCategoria));
    if (nova_categoria == NULL)
        return;

    nova_categoria->tipo = tipo;
    nova_categoria->alimentos = NULL;
    nova_categoria->raiz_energia = NULL;
    nova_categoria->raiz_proteina = NULL;
    nova_categoria->next = NULL;

    // Lista vazia ou inserir no início
    if (*head == NULL || tipo < (*head)->tipo) {
        nova_categoria->next = *head;
        *head = nova_categoria;
        return;
    }

    // Procura posição correta em ordem alfabética
    NodeCategoria *current = *head;
    while (current->next != NULL && tipo > current->next->tipo) {
        current = current->next;
    }

    nova_categoria->next = current->next;
    current->next = nova_categoria;
}

// Busca ou cria categoria
NodeCategoria *buscar_ou_criar_categoria(NodeCategoria **head,
                                         TipoCategoria tipo) {
    NodeCategoria *cat = buscar_categoria(*head, tipo);

    if (cat == NULL) {
        inserir_categoria_ordenada(head, tipo);
        cat = buscar_categoria(*head, tipo);
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
        TipoCategoria tipo_cat = string_to_enum(alimento.categoria);
        NodeCategoria *categoria =
            buscar_ou_criar_categoria(&lista_categorias, tipo_cat);

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
        printf("\n======= Categoria: %s\n", enum_to_string(cat->tipo));

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

// alterações Pedro:

void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


void lerString(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = 0;
}

// alterações Marcos das alterações Pedro:

void listarCategorias(NodeCategoria *head){
    NodeCategoria *cat = head;

    printf("=============Listagem de categorias=============\n");
    while (cat != NULL) {
        printf("%s\n", enum_to_string(cat->tipo));
        cat = cat->next;
    }
}

void listarAlimentosDaCategoria(NodeCategoria *head, TipoCategoria tipo) {
    NodeCategoria *categoria_encontrada = buscar_categoria(head, tipo);

    if (categoria_encontrada == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
    } else {

        printf("\n======= Alimentos da Categoria: %s =======\n",
               enum_to_string(categoria_encontrada->tipo));

        NodeAlimento *alim = categoria_encontrada->alimentos;

        if (alim == NULL) {
            printf("Nenhum alimento cadastrado nesta categoria.\n");
        }

        while (alim != NULL) {
            printf("%d - %s (Energia: %.2f Kcal, Proteina: %.2f g)\n",
                   alim->dados.numero,
                   alim->dados.descricao,
                   alim->dados.energia,
                   alim->dados.proteina);
            alim = alim->next;
        }
    }
}      

TipoCategoria perguntarCategoriaValida() {
    TipoCategoria tipo = CATEGORIA_INVALIDA;
    char nome[50];
    
    while (tipo == CATEGORIA_INVALIDA) {
        printf("Digite o nome da categoria: ");
        lerString(nome, 50);
        tipo = string_to_enum(nome);
        
        if (tipo == CATEGORIA_INVALIDA) {
            printf("Erro: Categoria '%s' nao existe. Tente novamente.\n", nome);
        }
    }
    
    return tipo;
}


// função 3, serve para todos os alimentos de certa categoria em ordem decresente com base na kcal
//essa é uma função auxiliar que auxilia (auxiliar q auxilia é foda kkkk) a ler a arvore de forma decrecente se baseando na energia 
void percorrerEnergiaDecrescente(NodeArvore *raiz) {
    if (raiz == NULL)
        return;

    // Maior energia primeiro
    percorrerEnergiaDecrescente(raiz->direita);

    // Exibe o alimento do nó
    printf(" - %s | Energia: %.2f Kcal | Proteína: %.2f g\n",
           raiz->alimento->dados.descricao,
           raiz->alimento->dados.energia,
           raiz->alimento->dados.proteina);

    // Valores menores depois
    percorrerEnergiaDecrescente(raiz->esquerda);
}

//essa é a parte q a função main chama, que é responsavel por listar em ordem decrecente esses alimentos ultilizando a função auxiliar 
void listarEnergiaDecrescente(NodeCategoria *head, TipoCategoria tipo) {
    NodeCategoria *cat = buscar_categoria(head, tipo);

    if (cat == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
        return;
    }

    printf("\n===== Alimentos da categoria %s por ENERGIA (decrescente) =====\n",
           enum_to_string(cat->tipo));

    if (cat->raiz_energia == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    percorrerEnergiaDecrescente(cat->raiz_energia);//é a ligação de uma função na outra 
}

//função 4: lista uma categoria q o usuario escolher de forma decrecente só que agora se baseando na proteina
//essa é a função auxiliar que ajuda a percorrer a arvore de forma decrecente usando a proteina para isso 
void percorrerProteinaDecrescente(NodeArvore *raiz) {
    if (raiz == NULL)
        return;

    //Maior proteína primeiro
    percorrerProteinaDecrescente(raiz->direita);

    // Imprime o alimento do nó atual
    printf(" - %s | Proteína: %.2f g | Energia: %.2f Kcal\n",
           raiz->alimento->dados.descricao,
           raiz->alimento->dados.proteina,
           raiz->alimento->dados.energia);

    // 3. Depois os menores
    percorrerProteinaDecrescente(raiz->esquerda);
}

//a função ligada ao main(), faz literalmente oq a função 3 faz só q para proteinas agora
void listarProteinaDecrescente(NodeCategoria *head, TipoCategoria tipo) {
    NodeCategoria *cat = buscar_categoria(head, tipo);

    if (cat == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
        return;
    }

    printf("\n===== Alimentos da categoria %s por PROTEINA (decrescente) =====\n",
           enum_to_string(cat->tipo));

    if (cat->raiz_proteina == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    // Conexão com a função auxiliar
    percorrerProteinaDecrescente(cat->raiz_proteina);
}

//Função 5(essa foi chata kkkkk): Responsavel por excluir um alimento 

//reconstroe a arvore de energia 
NodeArvore* reconstruirArvoreEnergia(NodeAlimento *lista) {
    NodeArvore *novaRaiz = NULL;

    NodeAlimento *atual = lista;
    while (atual != NULL) {
        novaRaiz = inserir_na_arvore(novaRaiz, atual->dados.energia, atual);
        atual = atual->next;
    }

    return novaRaiz;
}

//reconstroe a arvore de Proteinas
NodeArvore* reconstruirArvoreProteina(NodeAlimento *lista) {
    NodeArvore *novaRaiz = NULL;

    NodeAlimento *atual = lista;
    while (atual != NULL) {
        novaRaiz = inserir_na_arvore(novaRaiz, atual->dados.proteina, atual);
        atual = atual->next;
    }

    return novaRaiz;
}


// essa função auxiliar que é responsavel por procurar um alimento e retiralo da lista 
NodeAlimento* removerDaLista(NodeAlimento *head, int numero, NodeAlimento **removido) {
    NodeAlimento *atual = head;
    NodeAlimento *anterior = NULL;

    while (atual != NULL && atual->dados.numero != numero) {
        anterior = atual;
        atual = atual->next;
    }

    if (atual == NULL) {
        *removido = NULL;
        return head; // não achou
    }

    // Achou
    if (anterior == NULL) {  
        // primeiro da lista
        head = atual->next;
    } else {
        anterior->next = atual->next;
    }

    *removido = atual;
    return head;
}
//essas funções auxiliares seram chamadas na ordem correta na função 5

//função 5: oque sera chamado pela main() e elimina o alimento escolido pelo usuario 
void removerAlimento(NodeCategoria *head, TipoCategoria tipo, int numero, bool *houveAlteracoes) {

    NodeCategoria *cat = buscar_categoria(head, tipo);

    if (cat == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
        return;
    }

    NodeAlimento *removido = NULL;

    //Remove da lista ligada
    cat->alimentos = removerDaLista(cat->alimentos, numero, &removido);

    if (removido == NULL) {
        printf("\nAlimento '%d' nao encontrado.\n", numero);
        return;
    }

    //Reconstroi arvore de energia
    liberar_arvore(cat->raiz_energia);
    cat->raiz_energia = reconstruirArvoreEnergia(cat->alimentos);

    //Reconstroi arvore de proteina
    liberar_arvore(cat->raiz_proteina);
    cat->raiz_proteina = reconstruirArvoreProteina(cat->alimentos);

    //Libera o alimento
    free(removido);

    //Marca modificação
    *houveAlteracoes = true;

    printf("\nAlimento removido com sucesso!\n");
}

int main() {
    configurar_console();
    int opcao = 0;
    bool houveAlteracoes = false; // verifica se o usuario escolheu alguma opção que faz alterações, se sim ele muda para 1, e entra no if ao final do codigo

    NodeCategoria *categorias = ler_arquivo_e_popular("dados.bin"); // já estava ai, estão deixei kkkkk, mas ele é responsavel por retornar toda a lista completa já montada 

    while (opcao != 9) {

        printf("\n====== MENU ======\n");
        printf("1 - Listar todas as categorias\n");
        printf("2 - Listar alimentos de uma categoria\n");
        printf("3 - Listar alimentos por energia (decrescente)\n");
        printf("4 - Listar alimentos por proteína (decrescente)\n");
        printf("5 - Alimentos com energia entre valores\n");
        printf("6 - Alimentos com proteína entre valores\n");
        printf("7 - Remover categoria\n");
        printf("8 - Remover alimento\n");
        printf("9 - Sair\n");
        printf("Escolha: ");

        scanf("%1d", &opcao);
        limparEntrada();


        switch (opcao) {
        case 1: {
            listarCategorias(categorias);
        }
            break;

        case 2: {
            TipoCategoria tipo = perguntarCategoriaValida();
            if (tipo != CATEGORIA_INVALIDA) {
                listarAlimentosDaCategoria(categorias, tipo);
            }
            }
            break;

        case 3: {
            TipoCategoria tipo = perguntarCategoriaValida();
            if (tipo != CATEGORIA_INVALIDA) {
                listarEnergiaDecrescente(categorias, tipo);
            }
            }
            break;

        case 4: {
            TipoCategoria tipo = perguntarCategoriaValida();
            if (tipo != CATEGORIA_INVALIDA) {
                listarProteinaDecrescente(categorias, tipo); 
            }
            }
            break; 
        
        case 5: {
            char nome[50];
            float min, max;

            printf("Categoria: ");
            lerString(nome, 50);

            printf("Energia mínima: ");
            scanf("%f", &min);
            limparEntrada();

            printf("Energia máxima: ");
            scanf("%f", &max);
            limparEntrada();

            //listarEnergiaIntervalo(categorias, nome, min, max);
            }
            break;

        case 6: {
            char nome[50];
            float min, max;

            printf("Categoria: ");
            lerString(nome, 50);

            printf("Proteína mínima: ");
            scanf("%f", &min);
            limparEntrada();

            printf("Proteína máxima: ");
            scanf("%f", &max);
            limparEntrada();

            //listarProteinaIntervalo(categorias, nome, min, max);
            }
            break;

        case 7: {
            char nome[50];
            printf("Categoria a remover: ");
            lerString(nome, 50);

            //removerCategoria(&categorias, nome);
            }
            break;

        case 8: {
            TipoCategoria tipo = perguntarCategoriaValida();
            int numero;

            printf("Número do alimento: ");
            scanf("%d", &numero);
            limparEntrada();

            removerAlimento(categorias, tipo, numero, &houveAlteracoes);
            }
            break;

        case 9:
        break;

        default:
            break;
        }
    }

    // é a parte que eu comentei la em cima, se o usuario escolheu alguma opção de alterar a lista, ele vai cair nesse if 
    if (houveAlteracoes == true){
        //salvarDadosAtualizados(categorias, "dados.bin");
    }

    //libera tudo, meio auto explicativo né 
    liberar_tudo(categorias);

    return 0; 

}
