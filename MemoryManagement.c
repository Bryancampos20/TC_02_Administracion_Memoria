#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MEMORY_SIZE 1024

#define COLOR_YELLOW "\033[1;33m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_RED "\033[1;31m" 

typedef struct {
    char* name;   
    size_t size;
    int occupied;
} MemoryBlock;

MemoryBlock memory[MAX_BLOCKS];
size_t total_memory_used = 0;

void initialize_memory() {
    memory[0].size = MEMORY_SIZE;
    memory[0].name = (char*)malloc(sizeof(char) * 20);
    strcpy(memory[0].name, "Free");
    memory[0].occupied = 0;
    for (int i = 1; i < MAX_BLOCKS; i++) {
        memory[i].size = 0;
        memory[i].occupied = 0;
        memory[i].name = NULL; 
    }
}

int best_fit(size_t size, const char* var_name) {
    int best_index = -1;
    size_t min_size = MEMORY_SIZE + 1;

    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!memory[i].occupied && memory[i].size >= size) {
            if (memory[i].size < min_size) {
                min_size = memory[i].size;
                best_index = i;
            }
        }
    }

    if (best_index != -1) {
        if (memory[best_index].size > size) {
            size_t remaining_size = memory[best_index].size - size;
            memory[best_index].size = size;
            memory[best_index].name = (char*)malloc(sizeof(char) * (strlen(var_name) + 1)); 
            strcpy(memory[best_index].name, var_name);
            memory[best_index].occupied = 1;

            for (int j = best_index + 1; j < MAX_BLOCKS; j++) {
                if (memory[j].size == 0) {
                    memory[j].size = remaining_size;
                    memory[j].name = (char*)malloc(sizeof(char) * 20); 
                    strcpy(memory[j].name, "Free");
                    memory[j].occupied = 0;
                    break;
                }
            }
        } else {
            memory[best_index].name = (char*)malloc(sizeof(char) * (strlen(var_name) + 1)); 
            strcpy(memory[best_index].name, var_name);
            memory[best_index].occupied = 1;
        }
        total_memory_used += size;
        return 1;
    }

    return 0;
}

int first_fit(size_t size, const char* var_name) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!memory[i].occupied && memory[i].size >= size) {
            if (memory[i].size > size) {
                size_t remaining_size = memory[i].size - size;
                memory[i].size = size;
                memory[i].name = (char*)malloc(sizeof(char) * (strlen(var_name) + 1));
                strcpy(memory[i].name, var_name);
                memory[i].occupied = 1;

                for (int j = i + 1; j < MAX_BLOCKS; j++) {
                    if (memory[j].size == 0) {
                        memory[j].size = remaining_size;
                        memory[j].name = (char*)malloc(sizeof(char) * 20);
                        strcpy(memory[j].name, "Free");
                        memory[j].occupied = 0;
                        break;
                    }
                }
            } else {
                memory[i].name = (char*)malloc(sizeof(char) * (strlen(var_name) + 1));
                strcpy(memory[i].name, var_name);
                memory[i].occupied = 1;
            }
            total_memory_used += size;
            return 1;
        }
    }
    return 0;
}

int worst_fit(size_t size, const char* var_name) {
    int worst_index = -1;
    size_t max_size = 0;

    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!memory[i].occupied && memory[i].size >= size) {
            if (memory[i].size > max_size) {
                max_size = memory[i].size;
                worst_index = i;
            }
        }
    }

    if (worst_index != -1) {
        if (memory[worst_index].size > size) {
            size_t remaining_size = memory[worst_index].size - size;
            memory[worst_index].size = size;
            memory[worst_index].name = (char*)malloc(sizeof(char) * (strlen(var_name) + 1));
            strcpy(memory[worst_index].name, var_name);
            memory[worst_index].occupied = 1;

            for (int j = worst_index + 1; j < MAX_BLOCKS; j++) {
                if (memory[j].size == 0) {
                    memory[j].size = remaining_size;
                    memory[j].name = (char*)malloc(sizeof(char) * 20);
                    strcpy(memory[j].name, "Free");
                    memory[j].occupied = 0;
                    break;
                }
            }
        } else {
            memory[worst_index].name = (char*)malloc(sizeof(char) * (strlen(var_name) + 1));
            strcpy(memory[worst_index].name, var_name);
            memory[worst_index].occupied = 1;
        }
        total_memory_used += size;
        return 1;
    }
    return 0;
}

int realloc_memory(const char* var_name, size_t new_size) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (memory[i].occupied && strcmp(memory[i].name, var_name) == 0) {
            if (new_size == memory[i].size) {
                // Si el tamaño es el mismo, no se necesita reasignación.
                printf("No se requiere reasignación, el tamaño es el mismo.\n");
                return 1;
            } else if (new_size < memory[i].size) {
                // Si el nuevo tamaño es menor, reducimos el bloque actual
                size_t remaining_size = memory[i].size - new_size;
                memory[i].size = new_size;

                // Creamos un nuevo bloque con el tamaño restante
                for (int j = i + 1; j < MAX_BLOCKS; j++) {
                    if (memory[j].size == 0) {
                        memory[j].size = remaining_size;
                        memory[j].name = (char*)malloc(sizeof(char) * 20);
                        strcpy(memory[j].name, "Free");
                        memory[j].occupied = 0;
                        break;
                    }
                }
                printf("Bloque %s reducido a tamaño %zu bytes.\n", var_name, new_size);
                total_memory_used -= remaining_size;
                return 1;
            } else if (new_size > memory[i].size) {
                // Si el nuevo tamaño es mayor, intentamos expandir el bloque
                if (i + 1 < MAX_BLOCKS && !memory[i + 1].occupied && memory[i + 1].size >= (new_size - memory[i].size)) {
                    // Expansión posible en el siguiente bloque libre
                    size_t additional_size = new_size - memory[i].size;
                    memory[i].size = new_size;
                    memory[i + 1].size -= additional_size;

                    if (memory[i + 1].size == 0) {
                        memory[i + 1].occupied = 0;
                        free(memory[i + 1].name);
                        memory[i + 1].name = NULL;
                    }
                    total_memory_used += additional_size;
                    printf("Bloque %s expandido a tamaño %zu bytes.\n", var_name, new_size);
                    return 1;
                } else {
                    // Si no podemos expandir, liberamos el bloque actual y reasignamos uno nuevo
                    free_memory(var_name); // Liberamos el bloque actual
                    if (best_fit(new_size, var_name) || first_fit(new_size, var_name) || worst_fit(new_size, var_name)) {
                        printf("Bloque %s reasignado a nuevo tamaño %zu bytes.\n", var_name, new_size);
                        return 1;
                    } else {
                        printf(COLOR_RED "Error: No se pudo reasignar %s a tamaño %zu\n" COLOR_RESET, var_name, new_size);
                        return 0;
                    }
                }
            }
        }
    }
    printf(COLOR_RED "Error: No se encontró la variable %s\n" COLOR_RESET, var_name); 
    return 0; 
}


void free_memory(const char* var_name) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (memory[i].occupied && strcmp(memory[i].name, var_name) == 0) {
            total_memory_used -= memory[i].size;
            memory[i].occupied = 0;
            free(memory[i].name); 
            memory[i].name = (char*)malloc(sizeof(char) * 20); // Asigna "Free" después de liberar
            strcpy(memory[i].name, "Free");  // Asegúrate de asignar "Free"
            printf(COLOR_GREEN "Memoria liberada: %s\n" COLOR_RESET, var_name);
            return;
        }
    }
    printf(COLOR_RED "Error: No se encontró la variable %s\n" COLOR_RESET, var_name);
}


void print_memory() {
    printf("◆◆ Estado actual de la memoria:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (memory[i].size > 0) {
            if (memory[i].occupied) {
                printf(COLOR_PURPLE "Bloque %d: %zu bytes (Ocupado - %s)\n" COLOR_RESET, i, memory[i].size, memory[i].name);
            } else {
                printf(COLOR_PURPLE "Bloque %d: %zu bytes (Libre - %s)\n" COLOR_RESET, i, memory[i].size, memory[i].name);
            }
        }
    }
    printf("◆◆ Memoria usada: %zu bytes de %d bytes.\n", total_memory_used, MEMORY_SIZE);
}

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
            print_memory();
        }
    }

    fclose(file);
    return 0;
}
