#include <stdio.h>
#include <stdlib.h>

typedef enum {
    CEREAIS_DERIVADOS,
    VERDURAS_HORTALICAS_DERIVADOS,
    FRUTAS_DERIVADOS,
    PESCADOS_FRUTOS_DO_MAR,
    CARNES_DERIVADOS,
    LEITE_DERIVADOS,
    BEBIDAS,
    OVOS_DERIVADOS,
    ALIMENTOS_PREPARADOS,
    LEGUMINOSAS_DERIVADOS,
} Categoria;

typedef struct Alimento Alimento;

struct Alimento {
    int id;
    char descricao[256];
    int energia;
    float proteina;
    Categoria categoria;
    Alimento *proximo;
};

int main() {
    FILE *file = fopen("dados.bin", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Alimento alimento;

    while (fread(&alimento, sizeof(Alimento), 1, file) == 1) {
        printf("ID: %d", alimento.id);
    }

    fclose(file);
    return 0;
}
