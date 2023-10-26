#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"

void main(int argc, char *argv[]){
    int N = atoi(argv[1]);
    double* arregloEnergiaParticulas = CrearArregloEnergiaParticulas(N);
    for (int i = 0; i < N; i++){
        arregloEnergiaParticulas[i] = 0; // Inicializar el arreglo en 0
    }

    int posicionImpactoParticula = 0;
    int energiaParticula = 0;
    int posicionEnergiaMaximaActual = 0;
    int lineaProcesadas = 0;
    char buffer[1024];
    while(1){

        // Leer una línea desde el pipe (stdin_fileno)
        int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)); // Leer desde el pipe
        if (bytes_read == -1) {
            perror("Error al leer desde el pipe");
            exit(EXIT_FAILURE);
        }

        // Eliminar el carácter de nueva línea si está presente
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Comprobar si la línea es igual a "FIN"
        if (strcmp(buffer, "FIN") == 0) {
            break; // Salir del bucle si la línea es "FIN"
        }

        // Obtener y separar el chunk obtenido
        sscanf(buffer, "%d %d", &posicionImpactoParticula, &energiaParticula);
        sumarEnergiaParticulas(arregloEnergiaParticulas, N, posicionImpactoParticula, energiaParticula, &posicionEnergiaMaximaActual);
        lineaProcesadas++;
        
    }

    write(STDOUT_FILENO, arregloEnergiaParticulas, sizeof(double)*N); // Devolver arreglo
    write(STDOUT_FILENO, &lineaProcesadas, sizeof(int));  // Devolver cantidad de lineas procesadas
    
    return;


}