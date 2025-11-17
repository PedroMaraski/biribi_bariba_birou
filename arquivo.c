#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Coloca os dados do arquivo .csv em um arquivo em binário
void csv_to_bin(const char *csv_file, const char *bin_file) {
    char linha[MAX_LINE_LENGTH];
    FILE *f_csv, *f_bin;

    f_csv = fopen(csv_file, "r");
    if (f_csv == NULL) {
        perror("Erro ao abrir o arquivo CSV.");
        return;
    }
    f_bin = fopen(bin_file, "wb");
    if (f_bin == NULL) {
        perror("Erro ao abrir o arquivo binário.");
        return;
    }

    // Pula cabeçalho
    if (fgets(linha, MAX_LINE_LENGTH, f_csv) == NULL) {
        printf("Arquivo CSV vazio ou erro de leitura.\n");
        fclose(f_csv);
        fclose(f_bin);
        return;
    }

    int contador = 0;

    while (fgets(linha, MAX_LINE_LENGTH, f_csv) != NULL) {
        Alimento alimento;

        // Limpa a struct
        memset(&alimento, 0, sizeof(Alimento));

        // Limpa o \n no final da linha se existir
        linha[strcspn(linha, "\n")] = 0;

        // Parse da linha csv
        char *token;
        int campo = 0;

        token = strtok(linha, ";");
        while (token != NULL && campo < 5) {
            switch (campo) {
            case 0:
                alimento.numero = atoi(token);
                break;
            case 1:
                strncpy(alimento.descricao, token,
                        sizeof(alimento.descricao) - 1);
                break;
            case 2:
                alimento.energia = atof(token);
                break;
            case 3:
                alimento.proteina = atof(token);
                break;
            case 4:
                strncpy(alimento.categoria, token,
                        sizeof(alimento.categoria) - 1);
                break;
            }
            campo++;
            token = strtok(NULL, ";");
        }
        if (campo == 5) {
            fwrite(&alimento, sizeof(Alimento), 1, f_bin);
            contador++;
            printf("Gravado: %d | %s | %.2f | %.2f | %s\n", alimento.numero,
                   alimento.descricao, alimento.energia, alimento.proteina,
                   alimento.categoria);
        }
    }

    printf("\nTotal de alimentos gravados: %d\n", contador);
    printf("Tamanho de cada registro: %zu bytes\n", sizeof(Alimento));

    fclose(f_csv);
    fclose(f_bin);
}
