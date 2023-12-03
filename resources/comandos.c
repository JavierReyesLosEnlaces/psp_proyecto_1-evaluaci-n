#include <stdio.h>     // Para las funciones de entrada/salida estándar
#include <stdlib.h>    // Para las funciones relacionadas con la gestión de memoria y otras utilidades
#include <string.h>    // Para funciones de manipulación de cadenas de caracteres
#include <unistd.h>    // Para la función fork y otras llamadas al sistema
#include <sys/types.h> // Para tipos de datos relacionados con llamadas al sistema
#include <sys/wait.h>  // Para la función wait y macros relacionadas
#include <time.h>      // Para funciones relacionadas con el tiempo

#define MAX_LINE 80      // Constante llamada "MAX_LINE" que define la longitud máxima del comando
#define HISTORY_SIZE 100 // Constante llamada "HISTORY_SIZE" que define la longitud máxima del array "historial"

// Se usa para ejecutar un proceso hijo que muestra información sobre los cinco primeros procesos en ejecución
// La información mostrada es el PID del proceso y su comando asociado (CMD), presentado en un encabezado
void cincoPrimerosProcesos();

// Se usa para mostrar un historial de los comandos introducidos en el prompt de "MiShell>"
// El programa emplea el array "historial" y la variable "historial_contador" para llevar un recuento de los comandos
void mostrarHistorial();

// Se usa para simular una pantalla de carga o progreso de forma realista
// Para crear mayor sensación de naturalidad, se emplean números aleatorios en su funcionamiento
void pantallaCarga();

int main()
{
    char input[MAX_LINE];          // Cadena de caracteres que almacena la entrada del usuario
    int should_run = 1;            // Variable de control para el bucle principal
    char *historial[HISTORY_SIZE]; // Array de cadenas que almacena el historial de comandos introducidos
    int historial_contador = 0;    // Contador del número de comandos introducidos

    while (should_run)
    {
        printf("MiShell> ");
        fflush(stdout);
        fgets(input, MAX_LINE, stdin);
        input[strlen(input) - 1] = '\0';

        // Se crea un duplicado "input" y se introduce en la instancia "historial_contador" del array "historial"
        historial[historial_contador] = strdup(input);
        // Se incrementa en uno el valor de "historial_contador"
        historial_contador++;

        if (strcmp(input, "exit") == 0)
        {
            should_run = 0;
        }
        else
        {
            pid_t pid = fork();

            // Código del proceso hijo
            if (pid == 0)
            {
                // Array que almacena los argumentos introducidos por el usuario
                char *args[MAX_LINE / 2 + 1];
                // La función strtok divide la cadena input en tokens.
                char *token = strtok(input, " ");
                // El siguiente entero sirve para poder identificar posiciones en el array "args"
                int i = 0;

                // Se almacenan estos tokens en el array args.
                while (token != NULL)
                {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                // Se establece el fin del array, no hay más argumentos
                args[i] = NULL;

                // Condición de acceso a la función "cincoPrimerosProcesos()" por medio del prompt
                if (strcmp(input, "5pp") == 0)
                {
                    cincoPrimerosProcesos();
                    exit(0);
                }

                // Condición de acceso a la función "mostrarHistorial()" por medio del prompt
                if (strcmp(input, "historial") == 0)
                {
                    mostrarHistorial(historial, historial_contador);
                    exit(0);
                }

                // Condición de acceso a la función "pantallaCarga()" por medio del prompt
                if (strcmp(input, "pc") == 0)
                {
                    pantallaCarga();
                    exit(0);
                }

                execvp(args[0], args);

                // Este código solo se ejecutará en el caso de que falle "execvp"
                // Se muestra por pantalla un mensaje de error
                perror("Error in execvp");
                // Termina el proceso actual
                exit(EXIT_FAILURE);
            }
            // Se espera a que el proceso hijo termine antes de seguir con el padre
            wait(NULL);
        }
    }
    // Fin del programa
    return 0;
}

        //----------------------------------------------------------------------------------------------
        // MODIFICACIÓN 8 -> Una vez se introduzca el comando "5pp" por consola, el proceso hijo ejecuta
        // la función cincoPrimerosProcesos(), la cual ejecuta un comando para ver el PID y el comando
        // asociado de los primeros cinco procesos del sistema
        //----------------------------------------------------------------------------------------------

void cincoPrimerosProcesos(){
    printf("5 primeros procesos:\n");
    char comando[] = "ps -eo pid,cmd | head -6";
    system(comando);
}

        //------------------------------------------------------------------------------------------------
        // MODIFICACIÓN 9 -> Una vez se introduzca el comando "historial" por consola, el proceso hijo
        // ejecutará la función mostrarHistorial(), la cual mostrará el historial de comandos introducidos
        // por consola a lo largo de la sesión
        //------------------------------------------------------------------------------------------------

void mostrarHistorial(char *historial[HISTORY_SIZE], int historial_contador){
    printf("HISTORIAL DE COMANDOS:\n");
    for (int i = 0; i < historial_contador; i++)
    {
        printf("Comando %d: %s\n", i+1, historial[i]);
    }
}

        //------------------------------------------------------------------------------------------------
        // MODIFICACIÓN 10 -> Una vez se introduzca el comando "pc" por consola, el proceso hijo ejecutará
        // la función "pantallaCarga()", la cual simulará una pantalla de carga o loading bar cargándose
        // desde el 0% al 100% de forma realista
        //------------------------------------------------------------------------------------------------

void pantallaCarga() {
    // Tamaño de 101 para incluir el carácter nulo
    char progreso[101];
    int porcentaje = 0;
    int numeroAleatorio;

    // Inicialización de la semilla del generador de números aleatorios
    srand(time(NULL));
    printf("\n");

    // Loop para la creación de la pantalla de carga
    for (int i = 0; i < 100; i++) {
        // Limpiar la línea anterior en la consola utilizando "escape codes" ANSI
        system("echo -e '\033[1A\033[K'");

        numeroAleatorio = rand() % 100 + 1;
        porcentaje += 1;

        // Concatenación de "#" al progreso previo
        strcat(progreso, "#");

        printf("CARGANDO: %d%% %s", porcentaje, progreso);
        fflush(stdout);

        // Ajuste de velocidad, conversión de milisegundos a microsegundos
        usleep(numeroAleatorio * 2000);
    }
    printf("\nInstalación completada.\n");
}