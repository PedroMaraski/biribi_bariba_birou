#ifndef MAIN_H
#define MAIN_H

typedef enum {
    ALIMENTOS_PREPARADOS,
    BEBIDAS,
    CARNES_E_DERIVADOS,
    CEREAIS_E_DERIVADOS,
    FRUTAS_E_DERIVADOS,
    GORDURAS_E_OLEOS,
    LEGUMINOSAS_E_DERIVADOS,
    LEITE_E_DERIVADOS,
    MISCELANEAS,
    NOZES_E_SEMENTES,
    OUTROS_ALIMENTOS_INDUSTRIALIZADOS,
    OVOS_E_DERIVADOS,
    PESCADOS_E_FRUTOS_DO_MAR,
    PRODUTOS_ACUCARADOS,
    VERDURAS_HORTALICAS_E_DERIVADOS,

    CATEGORIA_INVALIDA
} TipoCategoria;

typedef struct {
    int numero;
    char descricao[100];
    char categoria[50];
    float energia;
    float proteina;
} Alimento;

typedef struct NodeAlimento {
    Alimento dados;
    struct NodeAlimento *next;
} NodeAlimento;

typedef struct NodeArvore {
    float valor;
    struct NodeArvore *esquerda;
    struct NodeArvore *direita;
    NodeAlimento *alimento;
} NodeArvore;

typedef struct NodeCategoria {
    TipoCategoria tipo;
    NodeAlimento *alimentos;
    NodeArvore *raiz_energia;
    NodeArvore *raiz_proteina;
    struct NodeCategoria *next;
} NodeCategoria;

#endif
