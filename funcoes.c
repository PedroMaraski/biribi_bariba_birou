#include "funcoes.h"

// Retorna um enumerado com base na string passada como parâmetro
TipoCategoria string_to_enum(const char *nomeString) {
    if (strcmp(nomeString, "Cereais e derivados") == 0)
        return CEREAIS_E_DERIVADOS;
    if (strcmp(nomeString, "Verduras, hortalicas e derivados") == 0)
        return VERDURAS_HORTALICAS_E_DERIVADOS;
    if (strcmp(nomeString, "Frutas e derivados") == 0)
        return FRUTAS_E_DERIVADOS;
    if (strcmp(nomeString, "Gorduras e oleos") == 0)
        return GORDURAS_E_OLEOS;
    if (strcmp(nomeString, "Pescados e frutos do mar") == 0)
        return PESCADOS_E_FRUTOS_DO_MAR;
    if (strcmp(nomeString, "Carnes e derivados") == 0)
        return CARNES_E_DERIVADOS;
    if (strcmp(nomeString, "Leite e derivados") == 0)
        return LEITE_E_DERIVADOS;
    if (strcmp(nomeString, "Bebidas (alcoolicas e não alcoolicas)") == 0)
        return BEBIDAS;
    if (strcmp(nomeString, "Ovos e derivados") == 0)
        return OVOS_E_DERIVADOS;
    if (strcmp(nomeString, "Produtos acucarados") == 0)
        return PRODUTOS_ACUCARADOS;
    if (strcmp(nomeString, "Miscelaneas") == 0)
        return MISCELANEAS;
    if (strcmp(nomeString, "Outros alimentos industrializados") == 0)
        return OUTROS_ALIMENTOS_INDUSTRIALIZADOS;
    if (strcmp(nomeString, "Alimentos preparados") == 0)
        return ALIMENTOS_PREPARADOS;
    if (strcmp(nomeString, "Leguminosas e derivados") == 0)
        return LEGUMINOSAS_E_DERIVADOS;
    if (strcmp(nomeString, "Nozes e sementes") == 0)
        return NOZES_E_SEMENTES;
    return CATEGORIA_INVALIDA;
}

// Retorna uma string com base no enumerado passado como parâmetro
const char *enum_to_string(TipoCategoria tipo) {
    switch (tipo) {
    case CEREAIS_E_DERIVADOS:
        return "Cereais e derivados";
    case VERDURAS_HORTALICAS_E_DERIVADOS:
        return "Verduras, hortalicas e derivados";
    case FRUTAS_E_DERIVADOS:
        return "Frutas e derivados";
    case GORDURAS_E_OLEOS:
        return "Gorduras e oleos";
    case PESCADOS_E_FRUTOS_DO_MAR:
        return "Pescados e frutos do mar";
    case CARNES_E_DERIVADOS:
        return "Carnes e derivados";
    case LEITE_E_DERIVADOS:
        return "Leite e derivados";
    case BEBIDAS:
        return "Bebidas (alcoolicas e não alcoolicas)";
    case OVOS_E_DERIVADOS:
        return "Ovos e derivados";
    case PRODUTOS_ACUCARADOS:
        return "Produtos acucarados";
    case MISCELANEAS:
        return "Miscelaneas";
    case OUTROS_ALIMENTOS_INDUSTRIALIZADOS:
        return "Outros alimentos industrializados";
    case ALIMENTOS_PREPARADOS:
        return "Alimentos preparados";
    case LEGUMINOSAS_E_DERIVADOS:
        return "Leguminosas e derivados";
    case NOZES_E_SEMENTES:
        return "Nozes e sementes";
    default:
        return "Categoria Desconhecida";
    }
}

// Busca uma categoria específica a partir da lista de categorias
// Retorna a categoria caso a encontre, e NULL caso contrário
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

// Insere um alimento e seu respectivo valor em uma árvore
// Retorna o ponteiro para o novo nó
NodeArvore *inserir_na_arvore(NodeArvore *raiz, float valor,
                              NodeAlimento *alimento) {
    if (raiz == NULL) {
        NodeArvore *novo_no = (NodeArvore *)malloc(sizeof(NodeArvore));

        if (novo_no == NULL) {
            perror("Erro ao alocar memoria para o no da árvore.");
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

// Insere a categoria de forma ordenada na lista de categorias
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

    if (*head == NULL || tipo < (*head)->tipo) {
        nova_categoria->next = *head;
        *head = nova_categoria;
        return;
    }

    NodeCategoria *current = *head;
    while (current->next != NULL && tipo > current->next->tipo) {
        current = current->next;
    }

    nova_categoria->next = current->next;
    current->next = nova_categoria;
}

// Busca uma categoria na lista ou a cria se não existir
NodeCategoria *buscar_ou_criar_categoria(NodeCategoria **head,
                                         TipoCategoria tipo) {
    NodeCategoria *cat = buscar_categoria(*head, tipo);

    if (cat == NULL) {
        inserir_categoria_ordenada(head, tipo);
        cat = buscar_categoria(*head, tipo);
    }

    return cat;
}

// Insere um novo alimento em ordem alfabética em sua respectiva categoria
NodeAlimento *inserir_alimento_ordenado(NodeCategoria *categoria,
                                        Alimento alimento) {
    NodeAlimento *novo_alimento = (NodeAlimento *)malloc(sizeof(NodeAlimento));
    if (novo_alimento == NULL)
        return NULL;

    novo_alimento->dados = alimento;
    novo_alimento->next = NULL;

    if (categoria->alimentos == NULL ||
        strcmp(alimento.descricao, categoria->alimentos->dados.descricao) < 0) {
        novo_alimento->next = categoria->alimentos;
        categoria->alimentos = novo_alimento;
        return novo_alimento;
    }

    NodeAlimento *current = categoria->alimentos;
    while (current->next != NULL &&
           strcmp(alimento.descricao, current->next->dados.descricao) > 0) {
        current = current->next;
    }

    novo_alimento->next = current->next;
    current->next = novo_alimento;

    return novo_alimento;
}

// Lê o arquivo binário e popula a lista ligada de categorias de alimentos em
// ordem alfabética
NodeCategoria *ler_arquivo_e_popular(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    NodeCategoria *lista_categorias = NULL;
    Alimento alimento;

    while (fread(&alimento, sizeof(Alimento), 1, file) == 1) {
        TipoCategoria tipo_cat = string_to_enum(alimento.categoria);
        NodeCategoria *categoria =
            buscar_ou_criar_categoria(&lista_categorias, tipo_cat);

        if (categoria != NULL) {
            NodeAlimento *novo_na_lista =
                inserir_alimento_ordenado(categoria, alimento);

            if (novo_na_lista != NULL) {
                categoria->raiz_energia = inserir_na_arvore(
                    categoria->raiz_energia, novo_na_lista->dados.energia,
                    novo_na_lista);

                categoria->raiz_proteina = inserir_na_arvore(
                    categoria->raiz_proteina, novo_na_lista->dados.proteina,
                    novo_na_lista);
            }
        }
    }

    fclose(file);
    return lista_categorias;
}

// Libera a memória destinada à arvore passada como parâmetro
void liberar_arvore(NodeArvore *raiz) {
    if (raiz == NULL)
        return;
    NodeArvore *left_tree = raiz->esquerda;
    NodeArvore *right_tree = raiz->direita;

    liberar_arvore(left_tree);
    liberar_arvore(right_tree);

    free(raiz);
}

// Libera a memória destinada à lista de categorias
// e chama liberar_arvore() para fazer o mesmo com árvores.
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

// Limpa o buffer (usada depois do input do usuário)
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// Lê uma string passada pelo usuário
void lerString(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = 0;
}

// Lista todas as categorias para o usuário
void listarCategorias(NodeCategoria *head) {
    NodeCategoria *cat = head;

    printf("=============Listagem de categorias=============\n");
    while (cat != NULL) {
        printf("%s\n", enum_to_string(cat->tipo));
        cat = cat->next;
    }
}

// Lista todos os alimentos de determinada categoria escolhida pelo usuário
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
                   alim->dados.numero, alim->dados.descricao,
                   alim->dados.energia, alim->dados.proteina);
            alim = alim->next;
        }
    }
}

// Pergunta ao usuário uma categoria e espera um resultado válido
TipoCategoria perguntarCategoriaValida() {
    TipoCategoria tipo = CATEGORIA_INVALIDA;
    do {
        char nome[50];
        printf("Digite um nome válido de categoria: ");
        lerString(nome, 50);

        tipo = string_to_enum(nome);
    } while (tipo == CATEGORIA_INVALIDA);

    return tipo;
}

// Passa por todos os nós da árvore de energia, do valor mais alto ao mais baixo
void percorrerEnergiaDecrescente(NodeArvore *raiz) {
    if (raiz == NULL)
        return;

    percorrerEnergiaDecrescente(raiz->direita);

    printf(" - %s | Energia: %.2f Kcal | Proteína: %.2f g\n",
           raiz->alimento->dados.descricao, raiz->alimento->dados.energia,
           raiz->alimento->dados.proteina);

    percorrerEnergiaDecrescente(raiz->esquerda);
}

// Lista Todos os alimentos em ordem decrescente de nível de energia
void listarEnergiaDecrescente(NodeCategoria *head, TipoCategoria tipo) {
    NodeCategoria *cat = buscar_categoria(head, tipo);

    if (cat == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
        return;
    }

    printf(
        "\n===== Alimentos da categoria %s por ENERGIA (decrescente) =====\n",
        enum_to_string(cat->tipo));

    if (cat->raiz_energia == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    percorrerEnergiaDecrescente(cat->raiz_energia);
}

// Passa por todos os nós da árvore de proteína, do valor mais alto ao mais
// baixo
void percorrerProteinaDecrescente(NodeArvore *raiz) {
    if (raiz == NULL)
        return;

    percorrerProteinaDecrescente(raiz->direita);

    printf(" - %s | Proteína: %.2f g | Energia: %.2f Kcal\n",
           raiz->alimento->dados.descricao, raiz->alimento->dados.proteina,
           raiz->alimento->dados.energia);

    percorrerProteinaDecrescente(raiz->esquerda);
}

// Lista Todos os alimentos em ordem decrescente de nível de proteína
void listarProteinaDecrescente(NodeCategoria *head, TipoCategoria tipo) {
    NodeCategoria *cat = buscar_categoria(head, tipo);

    if (cat == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
        return;
    }

    printf(
        "\n===== Alimentos da categoria %s por PROTEINA (decrescente) =====\n",
        enum_to_string(cat->tipo));

    if (cat->raiz_proteina == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    percorrerProteinaDecrescente(cat->raiz_proteina);
}

// Reconstroi a árvore de energia (usada após remover um alimento)
NodeArvore *reconstruirArvoreEnergia(NodeAlimento *lista) {
    NodeArvore *novaRaiz = NULL;

    NodeAlimento *atual = lista;
    while (atual != NULL) {
        novaRaiz = inserir_na_arvore(novaRaiz, atual->dados.energia, atual);
        atual = atual->next;
    }

    return novaRaiz;
}

// Reconstroi a árvore de proteína (usada após remover um alimento)
NodeArvore *reconstruirArvoreProteina(NodeAlimento *lista) {
    NodeArvore *novaRaiz = NULL;

    NodeAlimento *atual = lista;
    while (atual != NULL) {
        novaRaiz = inserir_na_arvore(novaRaiz, atual->dados.proteina, atual);
        atual = atual->next;
    }

    return novaRaiz;
}

// Remove um alimento da lista de alimentos em que ele se encontra em que ele se
// encontra
NodeAlimento *removerDaLista(NodeAlimento *head, int numero,
                             NodeAlimento **removido) {
    NodeAlimento *atual = head;
    NodeAlimento *anterior = NULL;

    while (atual != NULL && atual->dados.numero != numero) {
        anterior = atual;
        atual = atual->next;
    }

    if (atual == NULL) {
        *removido = NULL;
        return head;
    }

    if (anterior == NULL) {
        head = atual->next;
    } else {
        anterior->next = atual->next;
    }

    *removido = atual;
    return head;
}

// Lista todos os alimentos da árvore especificada que estejam num intervalo
// A árvore utilizada é a de energia e da categoria pedida pelo usuário.
void listarPorEnergia(NodeArvore *raiz, float min, float max) {
    if (raiz == NULL)
        return;

    if (min < raiz->valor) {
        listarPorEnergia(raiz->esquerda, min, max);
    }

    if (raiz->valor >= min && raiz->valor <= max) {

        NodeAlimento *alimento_encontrado = raiz->alimento;

        printf("Numero: %d\n", alimento_encontrado->dados.numero);
        printf("Descricao: %s\n", alimento_encontrado->dados.descricao);
        printf("Energia: %.2f\n", alimento_encontrado->dados.energia);
        printf("Proteina: %.2f\n", alimento_encontrado->dados.proteina);
        printf("-----------------------------\n");
    }

    if (max > raiz->valor) {
        listarPorEnergia(raiz->direita, min, max);
    }
}

// Lista todos os alimentos que se encontram num intervalo de energia e numa
// categoria especificados pelo usuário
void listarEnergiaIntervalo(NodeCategoria *categorias, TipoCategoria nome,
                            float min, float max) {
    NodeCategoria *aux = categorias;

    while (aux != NULL && aux->tipo != nome) {
        aux = aux->next;
    }

    if (aux == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    printf("\nAlimentos da categoria com energia entre %.2f e %.2f:\n\n", min,
           max);

    listarPorEnergia(aux->raiz_energia, min, max);
}

// Lista todos os alimentos da árvore especificada que estejam num intervalo
// A árvore utilizada é a de proteina e da categoria pedida pelo usuário.
void listarPorProteina(NodeArvore *raiz, float min, float max) {
    if (raiz == NULL)
        return;

    if (min <= raiz->valor)
        listarPorProteina(raiz->esquerda, min, max);

    if (raiz->valor >= min && raiz->valor <= max) {
        NodeAlimento *aux = raiz->alimento;
        printf("Numero: %d\n", aux->dados.numero);
        printf("Descricao: %s\n", aux->dados.descricao);
        printf("Energia: %.2f\n", aux->dados.energia);
        printf("Proteina: %.2f\n", aux->dados.proteina);
        printf("-----------------------------\n");
    }

    if (max >= raiz->valor)
        listarPorProteina(raiz->direita, min, max);
}

// Lista todos os alimentos que se encontram num intervalo de proteína e numa
// categoria especificados pelo usuário
void listarProteinaIntervalo(NodeCategoria *categorias, TipoCategoria tipo,
                             float min, float max) {
    NodeCategoria *aux = categorias;

    while (aux != NULL && aux->tipo != tipo) {
        aux = aux->next;
    }

    if (aux == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    printf("\nAlimentos da categoria com proteina entre %.2f e %.2f:\n\n", min,
           max);

    listarPorProteina(aux->raiz_proteina, min, max);
}

// Remove um alimento de uma categoria especificada pelo usuário
// O alimento é passado pelo usuário por índice
void removerAlimento(NodeCategoria *head, TipoCategoria tipo, int numero,
                     bool *houveAlteracoes) {

    NodeCategoria *cat = buscar_categoria(head, tipo);

    if (cat == NULL) {
        printf("\nErro: Categoria nao encontrada.\n");
        return;
    }

    NodeAlimento *removido = NULL;

    cat->alimentos = removerDaLista(cat->alimentos, numero, &removido);

    if (removido == NULL) {
        printf("\nAlimento '%d' nao encontrado.\n", numero);
        return;
    }

    liberar_arvore(cat->raiz_energia);
    cat->raiz_energia = reconstruirArvoreEnergia(cat->alimentos);

    liberar_arvore(cat->raiz_proteina);
    cat->raiz_proteina = reconstruirArvoreProteina(cat->alimentos);

    free(removido);

    *houveAlteracoes = true;

    printf("\nAlimento removido com sucesso!\n");
}

// Salva os dados no arquivo dados.bin caso alguma alteração tenha acontecido
void salvarDadosAtualizados(NodeCategoria *head, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erro ao abrir arquivo para salvar");
        return;
    }

    NodeCategoria *cat_atual = head;

    while (cat_atual != NULL) {
        NodeAlimento *alim_atual = cat_atual->alimentos;

        while (alim_atual != NULL) {
            fwrite(&(alim_atual->dados), sizeof(Alimento), 1, file);
            alim_atual = alim_atual->next;
        }
        cat_atual = cat_atual->next;
    }

    fclose(file);
    printf("\nDados atualizados salvos com sucesso em %s\n", filename);
}

// Menu principal, onde se encontram todas as opções do usuário
void menu() {
    int opcao = 0;
    bool houveAlteracoes = false;

    NodeCategoria *categorias = ler_arquivo_e_popular("dados.bin");

    while (opcao != 9) {

        printf("\n====== MENU ======\n");
        printf("1 - Listar todas as categorias\n");
        printf("2 - Listar alimentos de uma categoria\n");
        printf("3 - Listar alimentos por energia (decrescente)\n");
        printf("4 - Listar alimentos por proteína (decrescente)\n");
        printf("5 - Alimentos com energia dentro de um intervalo\n");
        printf("6 - Alimentos com proteína dentro de um intervalo\n");
        printf("7 - Remover categoria\n");
        printf("8 - Remover alimento\n");
        printf("9 - Sair\n");
        printf("Escolha: ");

        scanf("%1d", &opcao);
        limparEntrada();

        switch (opcao) {
        case 1: {
            puts("Opção escolhida:");
            puts("LISTAR TODAS AS CATEGORIAS");
            listarCategorias(categorias);
        } break;

        case 2: {
            puts("Opção escolhida:");
            puts("LISTAR ALIMENTOS DE UMA CATEGORIA");
            TipoCategoria tipo = perguntarCategoriaValida();
            if (tipo != CATEGORIA_INVALIDA) {
                listarAlimentosDaCategoria(categorias, tipo);
            }
        } break;

        case 3: {
            puts("Opção escolhida:");
            puts("LISTAR ALIMENTOS POR ENERGIA");
            TipoCategoria tipo = perguntarCategoriaValida();
            if (tipo != CATEGORIA_INVALIDA) {
                listarEnergiaDecrescente(categorias, tipo);
            }
        } break;

        case 4: {
            puts("Opção escolhida:");
            puts("LISTAR ALIMENTOS POR PROTEÍNA");
            TipoCategoria tipo = perguntarCategoriaValida();
            if (tipo != CATEGORIA_INVALIDA) {
                listarProteinaDecrescente(categorias, tipo);
            }
        } break;

        case 5: {
            puts("Opção escolhida:");
            puts("ALIMENTOS COM ENERGIA DENTRO DE UM INTERVALO");
            TipoCategoria tipo = perguntarCategoriaValida();
            float min, max;

            printf("Energia mínima: ");
            scanf("%f", &min);
            limparEntrada();

            printf("Energia máxima: ");
            scanf("%f", &max);
            limparEntrada();

            listarEnergiaIntervalo(categorias, tipo, min, max);
        } break;

        case 6: {
            puts("Opção escolhida:");
            puts("ALIMENTOS COM PROTEÍNA DENTRO DE UM INTERVALO");
            TipoCategoria tipo = perguntarCategoriaValida();
            float min, max;

            printf("Proteína mínima: ");
            scanf("%f", &min);
            limparEntrada();

            printf("Proteína máxima: ");
            scanf("%f", &max);
            limparEntrada();

            listarProteinaIntervalo(categorias, tipo, min, max);
        } break;

        case 7: {
            puts("Opção escolhida:");
            puts("REMOVER CATEGORIA");
            TipoCategoria tipo = perguntarCategoriaValida();

            // removerCategoria(&categorias, nome);
        } break;

        case 8: {
            TipoCategoria tipo = perguntarCategoriaValida();
            int numero;

            printf("Número do alimento: ");
            scanf("%d", &numero);
            limparEntrada();

            removerAlimento(categorias, tipo, numero, &houveAlteracoes);
        } break;

        case 9:
            break;

        default:
            break;
        }
    }

    if (houveAlteracoes == true) {
        salvarDadosAtualizados(categorias, "dados.bin");
    }

    liberar_tudo(categorias);
}
