## Proyecto de Administración de Memoria

Este programa implementa una simulación de administración de memoria dinámica en C, utilizando los algoritmos de asignación `Best-fit`, `First-fit` y `Worst-fit`. Se demuestran los problemas de fragmentación y fuga de memoria, gestionando operaciones de asignación, reasignación y liberación de bloques de memoria en un solo bloque de memoria asignado al inicio.

## Descripción del Proyecto

Este proyecto se basa en el curso de Principios de Sistemas Operativos y tiene como objetivo mostrar el funcionamiento básico de la administración de memoria en un sistema operativo. El programa simula operaciones de memoria mediante un archivo de entrada que contiene una secuencia de instrucciones como `ALLOC`, `REALLOC`, `FREE`, y `PRINT`.

## Funcionalidades

- **Asignación Dinámica**: Utiliza `malloc` y `calloc` para asignar bloques de memoria.
- **Reasignación**: Permite redimensionar bloques de memoria utilizando `realloc`.
- **Liberación de Memoria**: Libera bloques de memoria con `free`.
- **Algoritmos de Asignación**:
  - **Best-fit**: Selecciona el bloque de memoria que mejor se ajuste al tamaño solicitado.
  - **First-fit**: Selecciona el primer bloque de memoria que sea suficiente para la solicitud.
  - **Worst-fit**: Selecciona el bloque más grande disponible que satisfaga la solicitud.

## Formato de Instrucciones

- `ALLOC <variable> <tamaño>`: Asigna un bloque de memoria a la variable especificada.
- `REALLOC <variable> <nuevo_tamaño>`: Cambia el tamaño de un bloque de memoria existente.
- `FREE <variable>`: Libera el bloque de memoria asociado a la variable.
- `PRINT`: Muestra el estado actual de la memoria asignada.

## Pasos para ejecutar

1. Construye la imagen Docker:

   ```bash
   docker build -t memory-management .
   ```

2. Ejecuta el contenedor especificando el algoritmo de asignación (usando las iniciales `bf` para Best-fit, `ff` para First-fit, y `wf` para Worst-fit) y el nombre del archivo de entrada:

    ```bash
    docker run memory-management [iniciales del algoritmo: bf - ff - wf] [nombre del archivo txt]
    ```

## Ejemplo

Para ejecutar el algoritmo Best-fit con el archivo fragmentacion_1.txt:

    ```bash
    docker run memory-management bf fragmentacion_1.txt
    ```