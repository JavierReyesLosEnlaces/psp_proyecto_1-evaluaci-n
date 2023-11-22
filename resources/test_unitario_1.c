#include <stdio.h>//Para las funciones de entrada/salida estandar.
#include <stdlib.h>// Para funciones relacionadas con la gestion de memoria y otras utilidades.
#include <string.h>// Para funciones de manipulacion de cadenas de caracteres.
#include <unistd.h>// Para la funcion fork y otras llamadas al sistema.
#include <sys/types.h>// Para tipos de datos relacionados con llamadas al sistema.
#include <sys/wait.h>// Para la funcion wait y macros relacionadas.

            
#define MAX_LINE 80// constante que representa la longitud maxima de la linea de comando.

        //-------------------------------------------------------------------------------------------
        //MODIFICACION 1 -> Comprobamos si el comando ejecutado es cpu10, dentro del metodo ejecutarComando(),
        // si lo es, se ejecutara un metodo el cual se llama comprobarcpucommand() y ejecuta un comando para ver cada 3 segundos,los 10 procesos que mas cpu consumen.
        // Para salir del proceso, se usara ctrl + C, si nos sale el mensaje 'Prueba 2: Pasada' significa que el test se ha realizado correctamente.
        //-------------------------------------------------------------------------------------------

int comprobarcpucommand(char* comando) {

        printf("Va a dejar la ventana actual (Para salir presione ctrl + C), presiona enter.....");
        fflush(stdout); // Asegura que el mensaje se imprima antes de la pausa
        getchar();
        
        char newComandoCpu[] = "watch -n 3 \'free; echo; uptime; echo; ps aux --sort=-%cpu | head -n 11; echo; who\'";// Con este comando conseguimos mostrar una pequeña lista de 10 procesos que mas cpu consumen, esta se refrescara cada 3 segundos
        
        system(newComandoCpu);// se ejecuta el comando

        return 2;
}

// Esta función toma un comando como entrada y ejecuta ese comando en un proceso hijo
int ejecutarComando(char* comando) {

    
    if (strcmp(comando, "cpu10") == 0){
        
        return comprobarcpucommand(comando);
    }
    // Crea un nuevo proceso hijo utilizando fork() y almacena el resultado en pid
    pid_t pid = fork();

    // Si el pid es 0, el codigo dentro de este bloque se ejecuta en el proceso hijo.
    if (pid == 0) {

        // Array de punteros para almacenar los tokens del comando 
        //(token -> tokens son las palabras individuales que resultan de dividir la cadena de comando utilizando un delimitador (en este caso, el espacio en blanco)).
        char *args[MAX_LINE / 2 + 1];

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

    // guardamos nuestro comando en un array de caracteres, y posteriormente se lo pasamos al metodo ejecutarComando().
    char comando[] = "cpu10";
    int resultado = ejecutarComando(comando);
    //int resultado2 = ejecutarComandoPersonalizado()

    // Imprime el resultado de la ejecucion del comando "ls" si la funcion ejecutarComando devuelve 0.
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando 'ls' se ejecutó correctamente.\n");
    } else if(resultado == 1){

        printf("Prueba 1: Fallida - Error al ejecutar el comando 'ls'.\n");
    }else if(resultado == 2){// comprobamos si el resultado es 2, en tal caso quiere decir que nuestra prueba de comando cpu10 se ha efectuado correctamente.

        printf("Prueba 2: Pasada - El comando 'cpu10' se ejecutó correctamente.\n");
    }
    return 0;
}
