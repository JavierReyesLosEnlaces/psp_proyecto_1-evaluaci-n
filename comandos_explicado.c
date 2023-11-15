#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* Constante llamada "MAX_LINE" que define la longitud máxima del comando */

int main()
{
    char input[MAX_LINE]; // Es una cadena de caracteres que almacena la entrada del usuario
    int should_run = 1;   // Variable de control para el bucle principal

    while (should_run)
    { // Es como un while(true), un "si no ha concluido"
        printf("MiShell> ");
        fflush(stdout); // Limpia el buffer de salida para asegurar que se imprima el mensaje

        fgets(input, MAX_LINE, stdin); // Lee la entrada del usuario desde la consola
                                       // stdin -> entrada estándar (por teclado)

        input[strlen(input) - 1] = '\0'; // strlen(input) devuelve la longitud del string "input"
                                         // '\0' -> null
                                         // se resta 1 porque queremos acceder al último valor y las cadenas están indexadas en 0
        // input[strlen(input) - 1] = '\0'; -> verifica que el último caracter de la cadena "input" se le asigne el caracter nulo
        // el último caracter es el salto de línea y queremos que sea nulo, por eso lo ponemos como nulo

        if (strcmp(input, "exit") == 0) // strcmp(a,b) compara a y b. Si es cierto, "se ha concluido" -> 0
        {
            should_run = 0; // se ha concluido el "should_run"
        }
        else
        {
            pid_t pid = fork(); // Crea un nuevo proceso hijo

            if (pid == 0)
            {                                     // Código del proceso hijo
                char *args[MAX_LINE / 2 + 1];     // * es como un puntero       char *args[MAX_LINE / 2 + 1] es como un array de punteros a caracteres de tamaño "MAX_LINE / 2 + 1"
                char *token = strtok(input, " "); // strtok -> divide la entrada en tokens usando espacios como delimitadores
                                                  // En otras palabras, descompone una cadena más grande en partes más pequeñas o tokens basándose en ciertos caracteres de separación
                                                  // En este caso, el caracter de separación es un espacio
                int i = 0;
                while (token != NULL) // Mientras el token es null
                {
                    args[i] = token; // Almacena cada token como un argumento
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL; // Marca el final de los argumentos con NULL
                                // args[i] = NULL;: Al final del bucle, se coloca un puntero nulo (NULL) para marcar el final de la lista de argumentos.
                                // Esto es necesario para que execvp() pueda determinar el final de los argumentos.

                execvp(args[0], args);
                // UNA MOVIDA:
                /*
                execvp: Es una función de la familia exec en C que se utiliza para ejecutar un programa. La letra 'v' indica que se pasa un vector (array) de argumentos al programa.
                args[0]: Es el primer elemento del array args, que contiene el nombre del programa que se va a ejecutar.
                    *En C, el nombre del programa es el primer argumento de la lista de argumentos.
                args: Es el array de punteros a caracteres que representa los argumentos del programa.
                Cada elemento en args es un puntero a una cadena de caracteres, y la última entrada en este array debe ser NULL para indicar el final de la lista de argumentos.
                */

                exit(0); // Sale del proceso hijo
            }
            else
            {               // Código del proceso padre
                wait(NULL); // Espera a que el proceso hijo termine antes de continuar
            }
        }
    }

    return 0; // Fin del programa
}
