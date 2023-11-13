# Laboratorio 2 Sistope // Jaime Vadell
 Brokers,workers, fork(), dup2(), execv(), shmget(), shmat(), semaforos

- Para crear el ejectuble es necesario abrir una terminal donde se encuentra el archivo "Makefile" y ejecutar por consola el comando "make". En la carpeta output estara disponible el ejecutable el cual se llamara "lab2".

- El resto de los comandos y nombres de archivos sigue el establecido en el documento del laboratorio.

- Los archivos de salida generador por el programa se encontraran en la carpeta "testsOuput"

- El programa lee los archivos de prueba automaticamente desde la carpeta "test". Por lo tanto es necesario colocar aqui cualquier archivo de prueba. Para su posterior ejecucion se debe proseguir con los comando definidos en el enunciado. Por ejemplo para correr el programa con test1_35.txt(el cual se encuentra en "tests") es necesario ejecutar por ejemplo:
"./lab2 -N 35 -P 4 -i test1_35.txt -o salida1.txt -c 2 -D"
y el programa internamente buscara test1_35.txt en la carpeta "tests"

- El proyecto cuenta con la implementacion de memoria compartida a traves del uso de shmget() y shmat() ademas de la sincronizacion por el uso de semaforos

- No pude de encontrar una pareja para realizar el laboratorio por lo cual este fue hecho unicamente por mi. 

- GitHub del proyecto: https://github.com/JaimeVadell/Laboratorio-2-Sistope
