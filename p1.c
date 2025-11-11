#include "definicoes.h"
#include <stdio.h>

#define MAX_LINE_LENGTH 1024

void csv_to_bin(const char* csv_file, const char* bin_file){
    char linha[MAX_LINE_LENGTH];
    char separador = ";";
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

    // Cabeçalho
    if (fgets(linha, MAX_LINE_LENGTH, f_csv) == NULL) {
        printf("Arquivo CSV vazio ou erro de leitura.\n");
        fclose(f_csv);
        fclose(f_bin);
        return;
    }

    while(fgets(linha, MAX_LINE_LENGTH, f_csv) != NULL){
        size_t len = strlen(linha);
        fwrite(linha, sizeof(char), len, f_bin);
    }

    fclose(f_csv);
    fclose(f_bin);

}

int main(){
    csv_to_bin();
    return 0;
}

