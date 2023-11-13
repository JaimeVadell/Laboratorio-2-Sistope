#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>   

void main(int argc, char *argv[]){
    int N = atoi(argv[1]);

    //Obtener arreglo de energía de partículas
    int key = ftok(".", 34);
    double * arregloEnergiaParticulas;
    //Obtener el espacio de memoria compartida con la key creada
    int shmid = shmget(key, sizeof(double) * N, 0666);
    if (shmid == -1) {
        perror("Error al obtener el espacio de memoria compartida");
        exit(EXIT_FAILURE);
    }
    //Adjuntar el espacio de memoria compartida al proceso
    arregloEnergiaParticulas = (double *)shmat(shmid, NULL, 0);
    if (arregloEnergiaParticulas == (double *) -1) {
        perror("Error al adjuntar el espacio de memoria compartida");
        exit(EXIT_FAILURE);
    }
    //Verificar si ya existe el semaforo, si no existe lo crea
    sem_t *sem = sem_open("semaforo", 0);
    if (sem == SEM_FAILED) {
        sem = sem_open("semaforo", O_CREAT, 0666, 1);
        if (sem == SEM_FAILED) {
            perror("Error al crear el semaforo");
            exit(EXIT_FAILURE);
        }
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
        //Zona critica de memoria compartida
        sem_wait(sem);
        sumarEnergiaParticulas(arregloEnergiaParticulas, N, posicionImpactoParticula, energiaParticula, &posicionEnergiaMaximaActual);
        sem_post(sem);
        lineaProcesadas++;
        
    }

    write(STDOUT_FILENO, &lineaProcesadas, sizeof(int));  // Devolver cantidad de lineas procesadas
    
    return;


}