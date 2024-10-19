#include "memory_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MemoryBlock* memory;  
size_t total_memory_used = 0;

void initialize_memory() {
    memory = malloc(MAX_BLOCKS * sizeof(MemoryBlock));   
    if (!memory) {
        perror(COLOR_RED"Error al asignar memoria"COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    memory[0].size = MEMORY_SIZE;
    strcpy(memory[0].name, "Free");
    memory[0].occupied = 0;
    for (int i = 1; i < MAX_BLOCKS; i++) {
        memory[i].size = 0;
        memory[i].occupied = 0;
        strcpy(memory[i].name, "Free");
    }
}

/* 
Algoritmo best-fit: Que consiste en  asignar los bloques de memoria que mejor ajuste tengan.
Para que sea más eficiente la asignación. 
*/
int best_fit(size_t size, const char* var_name) {
    //Inicializamos el índice del mejor bloque y el tamaño mínimo
    int best_index = -1;
    size_t min_size = MEMORY_SIZE + 1;

    /*Buscamos los bloques libres y si tienen suficiente tamaño.
    Si el bloque que estamos analizando es elegible, se selecciona el mejor bloque
    */
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!memory[i].occupied && memory[i].size >= size) {
            if (memory[i].size < min_size) {
                min_size = memory[i].size;
                best_index = i;
            }
        }
    }

    /*
    Asignamo memoria si encontramos un bloque adecuado. Y si queda con espacio
    sobrante, se crea un nuevo bloque con dicho espacio sobrante y se le asigna como 
    free. Pero si no hay espacio sobrante, solo se asigna normalmente 
    */
    if (best_index != -1) {
        if (memory[best_index].size > size) {
            size_t remaining_size = memory[best_index].size - size;
            memory[best_index].size = size;
            strncpy(memory[best_index].name, var_name, MAX_NAME_LENGTH - 1);
            memory[best_index].name[MAX_NAME_LENGTH - 1] = '\0'; 
            memory[best_index].occupied = 1;

            for (int j = best_index + 1; j < MAX_BLOCKS; j++) {
                if (memory[j].size == 0) {
                    memory[j].size = remaining_size;
                    strcpy(memory[j].name, "Free");
                    memory[j].occupied = 0;
                    break;
                }
            }
        } else {
            strncpy(memory[best_index].name, var_name, MAX_NAME_LENGTH - 1);
            memory[best_index].name[MAX_NAME_LENGTH - 1] = '\0'; 
            memory[best_index].occupied = 1;
        }
        //Actualizamos el estado de la memoria utilizada
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
                strncpy(memory[i].name, var_name, MAX_NAME_LENGTH - 1);
                memory[i].name[MAX_NAME_LENGTH - 1] = '\0'; 
                memory[i].occupied = 1;

                for (int j = i + 1; j < MAX_BLOCKS; j++) {
                    if (memory[j].size == 0) {
                        memory[j].size = remaining_size;
                        strcpy(memory[j].name, "Free");
                        memory[j].occupied = 0;
                        break;
                    }
                }
            } else {
                strncpy(memory[i].name, var_name, MAX_NAME_LENGTH - 1);
                memory[i].name[MAX_NAME_LENGTH - 1] = '\0'; 
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
            strncpy(memory[worst_index].name, var_name, MAX_NAME_LENGTH - 1);
            memory[worst_index].name[MAX_NAME_LENGTH - 1] = '\0'; 
            memory[worst_index].occupied = 1;

            for (int j = worst_index + 1; j < MAX_BLOCKS; j++) {
                if (memory[j].size == 0) {
                    memory[j].size = remaining_size;
                    strcpy(memory[j].name, "Free");
                    memory[j].occupied = 0;
                    break;
                }
            }
        } else {
            strncpy(memory[worst_index].name, var_name, MAX_NAME_LENGTH - 1);
            memory[worst_index].name[MAX_NAME_LENGTH - 1] = '\0'; 
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
                        strcpy(memory[i + 1].name, "Free");
                    }
                    total_memory_used += additional_size;
                    printf("Bloque %s expandido a tamaño %zu bytes.\n", var_name, new_size);
                    return 1;
                } else {
                    // Si no podemos expandir, liberamos el bloque actual y reasignamos uno nuevo
                    free_memory(var_name);
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

/*
Consiste en liberar un bloque de memoria que ya estaba asignado anteriormente a una variable.
Busca el bloque que necesitamos liberar, cuando lo encuentra lo marcamos como libre, disminuimos la memoria
utilizada y cambiamos su nombre a free
*/
void free_memory(const char* var_name) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (memory[i].occupied && strcmp(memory[i].name, var_name) == 0) {
            total_memory_used -= memory[i].size;
            memory[i].occupied = 0;
            strcpy(memory[i].name, "Free");
            printf(COLOR_GREEN "Memoria liberada: %s\n" COLOR_RESET, var_name);
            return;
        }
    }
    printf(COLOR_RED "Error: No se encontró la variable %s\n" COLOR_RESET, var_name);
}

/*
Recorre los bloques para imprimir el estado y tamaño en el que se encuentran
*/
void print_memory_status() {
    printf("◆◆ Estado de la memoria:\n");
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

