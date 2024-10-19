#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdlib.h>

#define COLOR_YELLOW "\033[1;33m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_RED "\033[1;31m" 

#define MAX_BLOCKS 50
#define MEMORY_SIZE 1000
#define MAX_NAME_LENGTH 10

typedef struct {
    char name[MAX_NAME_LENGTH];
    size_t size;
    int occupied;
} MemoryBlock;

extern MemoryBlock* memory;
extern size_t total_memory_used;

void initialize_memory();
int best_fit(size_t size, const char* var_name);
int first_fit(size_t size, const char* var_name);
int worst_fit(size_t size, const char* var_name);
int realloc_memory(const char* var_name, size_t new_size);
void free_memory(const char* var_name);
void print_memory_status();

#endif // MEMORY_MANAGEMENT_H
