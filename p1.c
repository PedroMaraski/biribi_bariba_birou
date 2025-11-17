#include "arquivo.h"
#include <stdio.h>


int main() {
    printf("Convertendo CSV para binário...\n\n");
    csv_to_bin("alimentos.csv", "dados.bin");
    printf("\nConversão concluída.\n");
    return 0;
}
