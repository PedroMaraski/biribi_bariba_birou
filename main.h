#define MAX_LINE_LENGTH 1024

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
    char nome[50];
    NodeAlimento *alimentos;
    NodeArvore *raiz_energia;
    NodeArvore *raiz_proteina;
    struct NodeCategoria *next;
} NodeCategoria;

