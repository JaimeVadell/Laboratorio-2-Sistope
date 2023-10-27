#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Variables que representan los argumentos de entrada al programa
    int N = 0;
    int P = 0;
    char *archivo_entrada = NULL;
    char *archivo_salida = NULL;
    int mostrar_celdas = 0;
    int num_chunk = 0;

    int opt;
    while ((opt = getopt(argc, argv, "N:P:i:o:c:D")) != -1) {
        switch (opt) {
            case 'N':
                N = atoi(optarg);
                break;
            case 'P':
                P = atoi(optarg);
                break;
            case 'i':
                archivo_entrada = optarg;
                break;
            case 'o':
                archivo_salida = optarg;
                break;
            case 'c':
                num_chunk = atoi(optarg);
                break;
            case 'D':
                mostrar_celdas = 1;
                break;
            default:
                fprintf(stderr, "Uso: %s -N N -P P -i archivo_entrada -o archivo_salida -c num_chunk -D\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verificar que los argumentos requeridos se proporcionaron
    if(N <=0 || P<= 0 || archivo_entrada==NULL || archivo_salida==NULL || num_chunk <= 0){
        fprintf(stderr, "Faltan argumentos requeridos o existe un error en los datos\n");
        fprintf(stderr, "Uso: %s -N N -P P -i archivo_entrada -o archivo_salida -c num_chunk -D\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Crear Broker
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Error al crear un proceso hijo");
        exit(EXIT_FAILURE);
    }
    
    //Proceso hijo
    if(child_pid == 0){
        char arg_N[10];
        snprintf(arg_N, 10, "%d", N); // Convertir N en string
        char arg_P[10];
        snprintf(arg_P, 10, "%d", P); // Convertir P en string
        char arg_num_chunk[10];
        snprintf(arg_num_chunk, 10, "%d", num_chunk); // Convertir num_chunk en string
        char arg_mostrar_celdas[10];
        snprintf(arg_mostrar_celdas, 10, "%d", mostrar_celdas); // Convertir mostrar_celdas en string
        
        // Definir rutas para archivo de entrada y salida
        char ruta_entrada[100]; 
        char ruta_salida[100];
        
        // Concatenar rutas con nombres de archivo
        snprintf(ruta_entrada, 100, "../tests/%s", archivo_entrada); // Subir un nivel al directorio padre
        snprintf(ruta_salida, 100, "../testsOutput/%s", archivo_salida); // Subir un nivel al directorio padre
        
        char *args[] = {"./broker", arg_N, arg_P, ruta_entrada, ruta_salida, arg_num_chunk, arg_mostrar_celdas, NULL};
        execv(args[0], args);
    }
    return 0;
}