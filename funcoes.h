#ifndef FUNCOES_H
#define FUNCOES_H

#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TipoCategoria string_to_enum(const char *nomeString);
const char *enum_to_string(TipoCategoria tipo);
NodeCategoria *buscar_categoria(NodeCategoria *head, TipoCategoria tipo);
NodeArvore *inserir_na_arvore(NodeArvore *raiz, float valor,
                              NodeAlimento *alimento);
void inserir_categoria_ordenada(NodeCategoria **head, TipoCategoria tipo);
NodeCategoria *buscar_ou_criar_categoria(NodeCategoria **head,
                                         TipoCategoria tipo);
NodeAlimento *inserir_alimento_ordenado(NodeCategoria *categoria,
                                        Alimento alimento);
NodeCategoria *ler_arquivo_e_popular(const char *filename);
void liberar_arvore(NodeArvore *raiz);
void liberar_tudo(NodeCategoria *head);
void limparEntrada();
void lerString(char *destino, int tamanho);
void listarCategorias(NodeCategoria *head);
void listarAlimentosDaCategoria(NodeCategoria *head, TipoCategoria tipo);
TipoCategoria perguntarCategoriaValida();
void percorrerEnergiaDecrescente(NodeArvore *raiz);
void listarEnergiaDecrescente(NodeCategoria *head, TipoCategoria tipo);
void percorrerProteinaDecrescente(NodeArvore *raiz);
void listarProteinaDecrescente(NodeCategoria *head, TipoCategoria tipo);
NodeArvore *reconstruirArvoreEnergia(NodeAlimento *lista);
NodeArvore *reconstruirArvoreProteina(NodeAlimento *lista);
NodeAlimento *removerDaLista(NodeAlimento *head, int numero,
                             NodeAlimento **removido);
void removerAlimento(NodeCategoria *head, TipoCategoria tipo, int numero,
                     bool *houveAlteracoes);
void listarPorEnergia(NodeArvore *raiz, float min, float max);
void listarEnergiaIntervalo(NodeCategoria *categorias, TipoCategoria nome,
                            float min, float max);
void listarPorProteina(NodeArvore *raiz, float min, float max);
void listarProteinaIntervalo(NodeCategoria *categorias, TipoCategoria tipo,
                             float min, float max);
void salvarDadosAtualizados(NodeCategoria *head, const char *filename);

void menu();

#endif
