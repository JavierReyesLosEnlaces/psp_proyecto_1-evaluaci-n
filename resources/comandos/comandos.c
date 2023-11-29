#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 

#define MAX_LINE 80 /* Constante llamada "MAX_LINE" que define la longitud máxima del comando */
#define HISTORY_SIZE 100

void cincoPrimerosProcesos();
void mostrarHistorial();
void loadingBar();

int main()
{
    char input[MAX_LINE];           // Es una cadena de caracteres que almacena la entrada del usuario
    int should_run = 1;             // Variable de control para el bucle principal
    char *historial[HISTORY_SIZE];  // Array de cadenas que almacena el historial de comandos introducidos
    int historial_contador = 0;     // Contador del número de comandos introducidos

    while (should_run)
    { // Es como un while(true), un "si no ha concluido"
        printf("MiShell> ");
        fflush(stdout); // Limpia el buffer de salida para asegurar que se imprima el mensaje
        fgets(input, MAX_LINE, stdin); // Lee la entrada del usuario desde la consola y lo mete en la variable input
                                       // stdin -> entrada estándar (por teclado)
        input[strlen(input) - 1] = '\0'; 
        // fgets incluye el carácter de nueva línea ('\n') al final de la cadena leída desde la entrada estándar, y luego se elimina en la línea siguiente con input[strlen(input) - 1] = '\0';.
        // input[strlen(input) - 1] = '\0'; -> verifica que el último caracter de la cadena "input" se le asigne el caracter nulo
        // el último caracter es el salto de línea y queremos que sea nulo, por eso lo ponemos como nulo


        historial[historial_contador] = strdup(input);
        historial_contador++;

        if (strcmp(input, "exit") == 0) // strcmp(a,b) compara a y b. Si es cierto, "se ha concluido" -> 0
        {
            should_run = 0; // se ha concluido el "should_run"
        }
        else
        {
            pid_t pid = fork(); // Crea un nuevo proceso hijo

            if (pid == 0)
            { // Código del proceso hijo
                char *args[MAX_LINE / 2 + 1];
                char *token = strtok(input, " ");
                int i = 0;
                while (token != NULL)
                {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL;

                //historial[historial_contador] = NULL; // Ensure the next element is NULL for execvp

                if (strcmp(input, "5pp") == 0)
                {
                    cincoPrimerosProcesos();
                    exit(0); 
                }

                if (strcmp(input, "historial") == 0)
                {
                    printf("\n");
                    mostrarHistorial(historial, historial_contador);
                    exit(0); // Exit after the command is executed
                }

                if (strcmp(input, "lb") == 0)
                {
                    loadingBar();
                    exit(0); // Exit after the command is executed
                }
                
                execvp(args[0], args);


                // The code below this line will only be reached if execvp fails
                perror("Error in execvp");
                exit(EXIT_FAILURE);
            }
            wait(NULL);

        }
    }

    return 0; // Fin del programa
    
}
 
void cincoPrimerosProcesos(){
    printf("5 primeros procesos:\n");
    char comando[] = "ps -eo pid,cmd | head -6";
    system(comando);
}

void mostrarHistorial(char *historial[HISTORY_SIZE], int historial_contador){
    printf("HISTORIAL DE COMANDOS:\n");
    for (int i = 0; i < historial_contador; i++)
    {
        printf("Comando %d: %s\n", i+1, historial[i]);
    }
}

void loadingBar() {
    char progreso[101] = ""; // Se cambia el tamaño de 100 a 101 para incluir el carácter nulo
    int porcentaje = 0;
    int numeroAleatorio;

    srand(time(NULL)); // Inicializar la semilla del generador de números aleatorios

    for (int i = 0; i < 100; i++) {
        system("clear");
        numeroAleatorio = rand() % 100 + 1;
        porcentaje += 1;
        strcat(progreso, "#");

        printf("CARGANDO: %d%% %s", porcentaje, progreso);
        fflush(stdout);

        usleep(numeroAleatorio * 2000); // Ajustte de velocidad, convertir milisegundos a microsegundos
    }
    printf("\nInstalación completada.\n");
}
