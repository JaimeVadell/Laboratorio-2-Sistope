#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "funciones.h"
#include <string.h>
#define READ_END 0
#define WRITE_END 1


struct DataArrays {
    double *doubleArray;
    int *intArray;
};

void enviarChunks(int write_pipe[][2], int num_children, const char *nombreArchivo, int num_chunk);
void finalizarComunicacionProcesosHijos(int write_pipe[][2], int num_children);
void cerrarPipes(int read_pipe[][2], int write_pipe[][2], int num_children);
struct DataArrays *retornoHijos(int N, int read_pipe[][2], int num_children);
void liberarMemoria(struct DataArrays *dataArrays);

int main(int argc, char *argv[]) {
    //Obtener argumentos de entrada
    int N = atoi(argv[1]);
    int num_children = atoi(argv[2]); 
    const char *nombreArchivo = argv[3];
    const char *nombreArchivoSalida = argv[4];
    int num_chunk = atoi(argv[5]); 
    int mostrar_celdas = atoi(argv[6]);

    int read_pipe[num_children][2]; // Pipes para leer desde los hijos
    int write_pipe[num_children][2]; // Pipes para escribir a los hijos

    // Crear pipes para la comunicación bidireccional
    for (int i = 0; i < num_children; i++) {
        if (pipe(read_pipe[i]) == -1 || pipe(write_pipe[i]) == -1) {
            perror("Error al crear el pipe");
            exit(EXIT_FAILURE);
        }
    }
    //Crear procesos hijos
    for (int i = 0; i < num_children; i++) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Error al crear un proceso hijo");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Este es el proceso hijo       // Inicializar la semilla para generar números aleatorios extremo de lectura del pipe de lectura
            close(write_pipe[i][WRITE_END]); // Cerramos el extremo de escritura del pipe de escritura

            // Redirigir la entrada y salida estándar a los pipes
            dup2(read_pipe[i][WRITE_END], STDOUT_FILENO); // Redirigir la salida estándar al pipe de lectura
            close(read_pipe[i][WRITE_END]);

            dup2(write_pipe[i][READ_END], STDIN_FILENO); // Redirigir la entrada estándar desde el pipe de escritura
            close(write_pipe[i][READ_END]);

            // Ejecutar programa hijo(worker)
            char arg_N[10];
            snprintf(arg_N, 10, "%d", N); // Convertir N en string

            char *args[] = {"./worker", arg_N, NULL}; // Pass the string as an argument
            execv(args[0], args);
            // En caso de que execv falle

            perror("Error al ejecutar execv");
            exit(EXIT_FAILURE);
        }
    }

    // Este es el proceso padre
    // Cerrar los pipes que no se usan
    for(int i = 0; i < num_children; i++){
    close(read_pipe[i][WRITE_END]); // Cerramos el extremo de escritura del pipe de lectura
    close(write_pipe[i][READ_END]); // Cerramos el extremo de lectura del pipe de escritura
    }

    // Enviar los chunks a los hijos
    srand((time(NULL))); // Inicializar la semilla para generar números aleatorios
    enviarChunks(write_pipe, num_children, nombreArchivo, num_chunk);
    // Finalizar la comunicación con los hijos y esperar a que terminen
    finalizarComunicacionProcesosHijos(write_pipe, num_children);
    // Recibir los resultados de los hijos
    struct DataArrays *dataArrays = retornoHijos(N, read_pipe, num_children);
    // Cerrar los pipes
    cerrarPipes(read_pipe, write_pipe, num_children);


    //Obtener posicion energia maxima del arreglo
    int posicionEnergiaMaximaActual = obtenerPosicionMaximaEnergia(dataArrays->doubleArray, N);
    if(mostrar_celdas){
        imprimirNormalizado(dataArrays->doubleArray, posicionEnergiaMaximaActual, N, dataArrays->intArray, num_children);
    }
    // Generar archivo de salida
    imprimirEnOrden(dataArrays->doubleArray, posicionEnergiaMaximaActual, N, nombreArchivoSalida);
    
    // Liberar la memoria
    liberarMemoria(dataArrays);
    return 0;
}



/*Entrada: Estructura con arreglo de energia de particulas total y arreglo de lineas procesadas por cada hijo
Salida: N/A
Descripcion: Libera la memoria de la estructura
*/
void liberarMemoria(struct DataArrays *dataArrays){
    free(dataArrays->doubleArray);
    free(dataArrays->intArray);
    free(dataArrays);
}


/*Entrada: Arreglo de pipes de lectura, arreglo de pipes de escritura, numero de hijos
Salida: N/A
Descripcion: Cierra los pipes de lectura y escritura de los hijos
*/
void cerrarPipes(int read_pipe[][2], int write_pipe[][2], int num_children){
    //Cerrar los pipes
    for (int i = 0; i < num_children; i++) {
        close(read_pipe[i][READ_END]);
        close(write_pipe[i][WRITE_END]);
    }
    return;
}
/*Entrada: numero total de celdas, arreglo de pipes de lectura, numero de hijos
Salida: Estructura con arreglo de energia de particulas total y arreglo de lineas procesadas por cada hijo
Descripcion: Recibe los resultados de los hijos y los guarda en un arreglo de energia de particulas total y un arreglo de lineas procesadas por cada hijo
*/
struct DataArrays *retornoHijos(int N, int read_pipe[][2], int num_children){
    struct DataArrays *dataArrays = malloc(sizeof(struct DataArrays));
        // Variable de retorno de funcion
    double *arregloEnergiaParticulasTotal = CrearArregloEnergiaParticulas(N);
    int *lineasProcesadasHijo = (int *)malloc(num_children * sizeof(int));
    // Leer los resultados de los hijos
    for (int i = 0; i < num_children; i++) {
        double arregloEnergiaParticulasHijos[N];
        int lineasProcesadas;

        // Leer el arreglo de energía de partículas
        if (read(read_pipe[i][READ_END], arregloEnergiaParticulasHijos, sizeof(double) * N) == -1) {
            perror("Error al leer el arreglo de energía de partículas");
            exit(EXIT_FAILURE);
        }

        // Leer la cantidad de líneas procesadas
        if (read(read_pipe[i][READ_END], &lineasProcesadas, sizeof(int)) == -1) {
            perror("Error al leer la cantidad de líneas procesadas");
            exit(EXIT_FAILURE);
        }

        //Sumar energia de arreglo a arregloTotal
        for (int j = 0; j < N; j++) {
            arregloEnergiaParticulasTotal[j] += arregloEnergiaParticulasHijos[j];
    
        }

        //Sumar lineas procesadas
        lineasProcesadasHijo[i] = lineasProcesadas;
    }

    dataArrays->doubleArray = arregloEnergiaParticulasTotal;
    dataArrays->intArray = lineasProcesadasHijo;
    
    return dataArrays;

}
/*Entrada: Arreglo de pipes de escritura, numero de hijos
Salida: N/A
Descripcion: Finaliza la comunicacion con los hijos y espera a que terminen
*/
void finalizarComunicacionProcesosHijos(int write_pipe[][2], int num_children){
    //Finalizar la comunicacion con los hijos
    // Enviar "FIN" a todos los hijos y cerrar los extremos de escritura de los pipes
    for (int i = 0; i < num_children; i++) {
        const char *fin_message = "FIN";
        write(write_pipe[i][WRITE_END], fin_message, strlen(fin_message) + 1); // +1 para incluir el carácter nulo
        close(write_pipe[i][WRITE_END]);
    }

    //Esperar a que todos los procesos terminen
    int status = 0;
    for (int i = 0; i < num_children; i++) {
        wait(&status);
    }
}

/*Entrada: Arreglo de pipes de escritura, numero de hijos, nombre del archivo
Salida: N/A
Descripcion: Lee el archivo y envia los chunks a los hijos
*/
void enviarChunks(int write_pipe[][2], int num_children, const char *nombreArchivo, int num_chunk) {
    // Leer el archivo y enviar los chunks a los hijos
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    int numLines;
    fscanf(archivo, "%d", &numLines); // Lee la primera línea con el número de líneas

    //Variables necesarias para el envio de chunks y otros
    int hijoRandom;
    char linea[1024]; // Tamaño adecuado para lineas
    fgets(linea, sizeof(linea), archivo); // Ignorar la primera línea
    // Lee el archivo línea por línea y ejecuta los calculos
    for (int i = 0; i < numLines; i++) {
        hijoRandom = generarNumeroAleatorio(0, num_children - 1); // Generar un número aleatorio entre 0 y num_children - 1
        //Entregar cierto numero de lineas a cada hijo de acuerdo a num_chunk
        for(int chunkProcesadoHijo = 0; chunkProcesadoHijo < num_chunk; chunkProcesadoHijo++){
            if (fgets(linea, sizeof(linea), archivo) != NULL) {
                // Escribir la línea en el pipe del hijo seleccionado
                write(write_pipe[hijoRandom][WRITE_END], linea, sizeof(linea));
            }
        }
    }
    fclose(archivo);
}