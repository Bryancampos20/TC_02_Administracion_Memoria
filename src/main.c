#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_management.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf(COLOR_RED "Uso: %s <algoritmo> <archivo_entrada>\n" COLOR_RESET, argv[0]);
        return 1;
    }
    const char* algorithm = argv[1];
    FILE* file = fopen(argv[2], "r");
    if (!file) {
        perror(COLOR_RED "Error al abrir el archivo\n" COLOR_RESET);
        return 1;
    }

    initialize_memory();

    char command[20], var_name[20];
    size_t size;

    while (fscanf(file, "%s", command) != EOF) {
        if (command[0] == '#') {
            fscanf(file, "%*[^\n]"); 
            continue;
        }
        if (strcmp(command, "ALLOC") == 0) {
            fscanf(file, "%s %zu", var_name, &size);
            if (strcmp(algorithm, "bf") == 0) {
                if (best_fit(size, var_name)) {
                    printf(COLOR_YELLOW "Asignado: %s de tamaño %zu\n" COLOR_RESET, var_name, size);
                } else {
                    printf(COLOR_RED "Error: No se pudo asignar %s de tamaño %zu\n" COLOR_RESET, var_name, size);
                }
            } else if (strcmp(algorithm, "ff") == 0) {
                if (first_fit(size, var_name)) {
                    printf(COLOR_YELLOW "Asignado: %s de tamaño %zu\n" COLOR_RESET, var_name, size);
                } else {
                    printf(COLOR_RED "Error: No se pudo asignar %s de tamaño %zu\n" COLOR_RESET, var_name, size);
                }
            } else if (strcmp(algorithm, "wf") == 0) {
                if (worst_fit(size, var_name)) {
                    printf(COLOR_YELLOW "Asignado: %s de tamaño %zu\n" COLOR_RESET, var_name, size);
                } else {
                    printf(COLOR_RED "Error: No se pudo asignar %s de tamaño %zu\n" COLOR_RESET, var_name, size);
                }
            } else {
                printf(COLOR_RED "Error: Algoritmo desconocido\n" COLOR_RESET);
            }
        } else if (strcmp(command, "REALLOC") == 0) {
            fscanf(file, "%s %zu", var_name, &size);
            if (realloc_memory(var_name, size)) {
                printf("Reasignado: %s a tamaño %zu\n", var_name, size);
            }
        } else if (strcmp(command, "FREE") == 0) {
            fscanf(file, "%s", var_name);
            free_memory(var_name);
        } else if (strcmp(command, "PRINT") == 0) {
            print_memory_status();
        }
    }

    fclose(file);
    return 0;
}
