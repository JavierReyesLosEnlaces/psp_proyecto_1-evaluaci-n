#include <stdio.h>//Para las funciones de entrada/salida estandar.
#include <stdlib.h>// Para funciones relacionadas con la gestion de memoria y otras utilidades.
#include <string.h>// Para funciones de manipulacion de cadenas de caracteres.
#include <unistd.h>// Para la funcion fork y otras llamadas al sistema.
#include <sys/types.h>// Para tipos de datos relacionados con llamadas al sistema.
#include <sys/wait.h>// Para la funcion wait y macros relacionadas.

// -----------------------------------------------------------------------------------------------------------------------------------------   
// MODIFICACION 1-> Cambiamos el valor de MAX_LINE a 40, ya que es el valor que actualmente usamos 
// cuando instanciamos *args, mas 1 para dejar un espacio nulo al final del array, evitandonos asi una operacion inecesaria dividir para 2.
// Con esto tambien tendremos que modificar la longitud del array *args[] a MAX_LINE + 1.
// SIN MODIFICACION -> #define MAX_LINE 80
// -----------------------------------------------------------------------------------------------------------------------------------------   

// constante que representa la longitud maxima de la linea de comando.
#define MAX_LINE 40

// Esta función toma un comando como entrada y ejecuta ese comando en un proceso hijo
int ejecutarComando(char* comando) {

    // Crea un nuevo proceso hijo utilizando fork() y almacena el resultado en pid
    pid_t pid = fork();

    // Si el pid es 0, el codigo dentro de este bloque se ejecuta en el proceso hijo.
    if (pid == 0) {
        
        
        // -----------------------------------------------------------------------------------------------------------------------------------------    
        // MODIFICACION 1.2 -> debemos modificar el valor de la logitud del array por MAX_LINE + 1  como hemos comentado en la modificacion 1.
        // SIN MODIFICACION -> char *args[MAX_LINE / 2 + 1];
        // -----------------------------------------------------------------------------------------------------------------------------------------  
    
        // Array de punteros para almacenar los tokens del comando 
        //(token -> tokens son las palabras individuales que resultan de dividir la cadena de comando utilizando un delimitador (en este caso, el espacio en blanco)).
        char *args[MAX_LINE + 1];

        // Inicialización del *token con el primer token de la cadena de comando.
        char *token = strtok(comando, " ");

        // Bucle para dividir la cadena de comando en tokens y almacenarlos en args.
        int i = 0;
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }

        // Se asigna NULL al siguiente elemento del array args(comun en la ejecucion de comandos).
        args[i] = NULL;

        // Se utiliza execvp para reemplazar la imagen del proceso actual con el comando especificado.
        execvp(args[0], args);

        // Si el proceso execvp falla, el programa hijo termina automaticamente como un exito.
        exit(0);

        // El padre espera a que el proceso hijo termine mediante wait.
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

    // Llama a la funcion ejecutarComando con el comando "ls" y se guarda el resultado en resultado.
    char comando_ls[] = "find /home/alumno/Escritorio/ -type f -name \"*.txt\" -exec grep -i \"patrón\" {} +";
    int resultado = ejecutarComando(comando_ls);

    // Imprime el resultado de la ejecucion del comando "ls" si la funcion ejecutarComando devuelve 0.
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando 'ls' se ejecutó correctamente.\n");
    } else {

        printf("Prueba 1: Fallida - Error al ejecutar el comando 'ls'.\n");
    }
    return 0;
}

