#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* Constante llamada "MAX_LINE" que define la longitud máxima del comando */

int main() {
    char input[MAX_LINE];
    int should_run = 1; /* Variable de control*/

    while (should_run) {
        printf("MiShell> ");
        fflush(stdout);
        /* Por lo general cuando damos un printf, no se imprime directamente en la pantalla sino que se guarda en un buffer */
        /* este fflush(stdout) vacía el buffer en cada iteración*/

        fgets(input, MAX_LINE, stdin);

        // Reemplazar el salto de línea con un terminador de cadena
        input[strlen(input) - 1] = '\0';

        // Salir si el usuario ingresa "exit"
        if (strcmp(input, "exit") == 0) {
            should_run = 0;
        } else {
            pid_t pid = fork();

            if (pid == 0) {
                // Este es el proceso hijo
                char *args[MAX_LINE / 2 + 1]; // Argumentos del comando
                char *token = strtok(input, " ");

                int i = 0;
                while (token != NULL) {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL; // Marcar el final de los argumentos

                execvp(args[0], args);
                exit(0);
            } else {
                // Este es el proceso padre
                wait(NULL);
            }
        }
    }

    return 0;
}