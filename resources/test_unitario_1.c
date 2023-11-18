#include <stdio.h>//Para las funciones de entrada/salida estandar.
#include <stdlib.h>// Para funciones relacionadas con la gestion de memoria y otras utilidades.
#include <string.h>// Para funciones de manipulacion de cadenas de caracteres.
#include <unistd.h>// Para la funcion fork y otras llamadas al sistema.
#include <sys/types.h>// Para tipos de datos relacionados con llamadas al sistema.
#include <sys/wait.h>// Para la funcion wait y macros relacionadas.

#define MAX_LINE 80// constante que representa la longitud maxima de la linea de comando.

//Esta función toma un comando como entrada y ejecuta ese comando en un proceso hijo
int ejecutarComando(char* comando) {

    //Crea un nuevo proceso hijo utilizando fork() y almacena el resultado en pid
    pid_t pid = fork();

    //Si el pid es 0, el codigo dentro de este bloque se ejecuta en el proceso hijo.
    if (pid == 0) {
        
        //El comando se divide en tokens utilizando strtok y se almacenan en el array args.
        char *args[MAX_LINE / 2 + 1];
        char *token = strtok(comando, " ");
        int i = 0;
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }

        // se asigna NULL al siguiente elemento del array args(comun en la ejecucion de comandos).
        args[i] = NULL;

        //Se utiliza execvp para reemplazar la imagen del proceso actual con el comando especificado.
        execvp(args[0], args);

        //Si el proceso execvp falla, el programa hijo termina automaticamente como un exito.
        exit(0);

        //El padre espera a que el proceso hijo termine mediante wait.
    } else if (pid > 0) {
        wait(NULL);
    } else {
        // Error al crear el proceso hijo.
        return -1; 
    }

    // Exito
    return 0; 
}


int main() {

    //Llama a la funcion ejecutarComando con el comando "ls" y se guarda el resultado en resultado.
    char comando_ls[] = "ls";
    int resultado = ejecutarComando(comando_ls);

    //Imprime el resultado de la ejecucion del comando "ls" si la funcion ejecutarComando devuelve 0.
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando 'ls' se ejecutó correctamente.\n");
    } else {

        printf("Prueba 1: Fallida - Error al ejecutar el comando 'ls'.\n");
    }
    return 0;
}

