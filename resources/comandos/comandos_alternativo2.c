#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define HISTORY_SIZE 10

struct CommandHistory {
    char commands[HISTORY_SIZE][MAX_LINE];
    int count;
};

void mostrarHistorial(struct CommandHistory *historial) {
    printf("HISTORIAL DE COMANDOS INTRODUCIDOS:\n");
    fflush(stdout);
    for (int i = 0; i < historial->count; i++) {
        printf("--> %s", historial->commands[i]);
    }
}

int main() {
    char input[MAX_LINE];
    int should_run = 1;

    struct CommandHistory historial;
    historial.count = 0;

    while (should_run) {
        printf("MiShell> ");
        fflush(stdout);

        fgets(input, MAX_LINE, stdin);
        strcpy(historial.commands[historial.count], input);
        historial.count++;

        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit") == 0) {
            mostrarHistorial(&historial);
            should_run = 0;
        } else {
            pid_t pid = fork();

            if (pid == 0) {
                char *args[MAX_LINE / 2 + 1];
                char *token = strtok(input, " ");
                int i = 0;

                while (token != NULL) {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL;

                if (execvp(args[0], args) == -1) {
                    perror("Error al ejecutar el comando");
                    exit(EXIT_FAILURE);
                }
            } else {
                wait(NULL);
            }
        }
    }

    return 0;
}