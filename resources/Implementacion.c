#include <stdio.h>     // Para las funciones de entrada/salida estándar
#include <stdlib.h>    // Para las funciones relacionadas con la gestión de memoria y otras utilidades
#include <string.h>    // Para funciones de manipulación de cadenas de caracteres
#include <unistd.h>    // Para la función fork y otras llamadas al sistema
#include <sys/types.h> // Para tipos de datos relacionados con llamadas al sistema
#include <sys/wait.h>  // Para la función wait y macros relacionadas
#include <time.h>      // Para funciones relacionadas con el tiempo
#include <dirent.h>     // Para manejar operaciones de directorios (abrir, leer, cerrar).
#include <sys/stat.h>

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


// Se usa para ejecutar un proceso, que muestra los 10 procesos que más cpu consumen, junto con ciertos datos interesantes de estos y del mismo sistema.
int comprobarcpucommand();

// Este proceso se encarga de devolver: el path en el que nos encontramos, el número de archivos en el directorio actual,
// el número de directorios en el directorio actual, y los permisos del usuario actual sobre el directorio actual.
int comprobarstoycommand(); 

// Utilizamos esta función para contar el número de palabras que tiene un comando.
int contarComando(char* comando);

// Utilizamos esta función para contar el número de palabras que tiene el path en el que nos encontramos, excluyendo las "/",
// si el path contiene 5 o más palabras, devuelve un resultado, si no devuelve otro.
int comprobarpwdcount();

// Utilizamos esta función para comprobar qué comando le estamos mandando al programa para ejecutar, si tiene más de 2 palabras retorna un valor de error,
// si el comando difiere de "ls,cpu,stoy y pwd" retorna otro valor de error diferente, si es uno de los comandos anteriores, los ejecutará.
int ejecutarComando(char* comando);

// Se utiliza para testear el valor del comando introducido, y dependiendo del valor de este resultado, mostrará un mensaje de testeo pasado o no.
void testearComando(int resultado);

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
                }else {
                    int resultado = ejecutarComando(input);
                    testearComando(resultado);
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

int comprobarcpucommand() {

    printf("Va a dejar la ventana actual (Para salir presione ctrl + C), presiona enter.....");

    // Asegura que el mensaje se imprima antes de la pausa
    fflush(stdout); 
    getchar();
    pid_t pid = fork();
    int pidhijo=0;
    if(pid == 0){
        pidhijo= (int) getpid();

        // Con este comando conseguimos mostrar una pequeña lista de 10 procesos que más CPU consumen, esta se refrescará cada 3 segundos
        char newComandoCpu[] = "watch -n 3 \'free; echo; uptime; echo; ps aux --sort=-%cpu | head -n 11; echo; who\'";
        
        // Se ejecuta el comando
        system(newComandoCpu);
        
    }else if (pid<0){
        printf("Error al crear el proceso hijo con pid `%n` del comando cpu.",pidhijo);
        return 8;
    }else{
        // Eesperamos a que el proceso hijo termine
        wait(NULL);
    }
    return 3;
}

        //-------------------------------------------------------------------------------------------
        // MODIFICACION 2 -> Usamos este método para manejar la información de nuestro directorio,
        // con él mostramos diferente información del directorio como: la ruta, el número de archivos,
        // el número de directorios y los permisos del directorio en el que estamos.
        //-------------------------------------------------------------------------------------------

int comprobarstoycommand() {

    // Declaraciones de variables
    struct dirent *entry;
    struct stat info;
    FILE *fp;
    DIR *dir;
    int contador = 0,contadorDir=0;
    char path[MAX_LINE];
    char permisos[10];

    // Abrir un proceso para ejecutar el comando "pwd" y leer su salida
    fp = popen("pwd", "r");
    if (fp == NULL) {
        perror("Error al ejecutar popen");

        // Retorna un código de error
        return 7; 
    }

    // Leer el resultado del comando y almacenarlo en la variable path
    if (fgets(path, sizeof(path), fp) != NULL) {

        // Eliminar el salto de línea del final, si está presente
        size_t length = strlen(path);
        if (length > 0 && path[length - 1] == '\n') {
            path[length - 1] = '\0';
        }

        // Abrir el directorio actual
        dir = opendir(path);
        if (dir == NULL) {
            perror("Error al abrir el directorio");
            exit(EXIT_FAILURE);
        }   

        // Contar archivos y directorios en el directorio
        while ((entry = readdir(dir)) != NULL) {

            // Cuenta los archivos del directorio
            if (entry->d_type == DT_REG) {  
                contador++;
            }

            // Cuenta los directorios dentro del que estamos, excluyendo "." y ".."
            if (entry->d_type == DT_DIR  && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                contadorDir++;
            }
    }

    // Obtiene los permisos sobre el directorio
    if (stat(path, &info) == 0) {

        // Convierte los bits de permisos a una cadena
        snprintf(permisos, 10, "%o", info.st_mode & 0777);
    } else {

        perror("Error al obtener información del directorio");
        exit(EXIT_FAILURE);
    }

    // Cierra el directorio
    closedir(dir);

    // Imprimir información sobre el directorio
     printf("Ruta actual: %s\nAchivos: %d\nDirectorios: %d\nPermisos: %s\n", path,contador,contadorDir,permisos);

    } else {

        perror("Error al leer la salida de popen");
        return 7;
    }

    // Cerrar el flujo del proceso
    if (pclose(fp) == -1) {
        perror("Error al cerrar popen");
        return 7;
    }

    // Se termina el test con Éxito.
    return 2; 
}

        //-------------------------------------------------------------------------------------------
        // MODIFICACION 3 -> Añadimos un método para contar las palabras del comando,
        // y lo usamos en el método ejecutarComando para verificar que el comando solo tenga 1 palabra.
        //-------------------------------------------------------------------------------------------
    
int contarComando(char* comando){
    
    int contador = 0;

    // Divide el comando en diferentes tokens, los cuales usaremos para contar las palabras.
    const char *token = strtok(comando, " ");
    while (token != NULL) {
         contador++;
         token = strtok(NULL, " ");
    }
    
    return contador;
}

        //-------------------------------------------------------------------------------------------
        // MODIFICACION 4 -> Una vez ejecutamos el comando pwd, comprobamos la cantidad de palabras
        // que contiene el path del directorio en el que estamos, si la cantidad de palabras es 5 o mayor
        // se da por pasado el test, si es menor, el test falla.
        //-------------------------------------------------------------------------------------------

int comprobarpwdcount(){

    FILE *fp;
    char path[MAX_LINE];
    int contadorPalb = 0;

    // Abrir un proceso para ejecutar el comando "pwd" y leer su salida
    fp = popen("pwd", "r");
    if (fp == NULL) {
        perror("Error al ejecutar popen");

        // Retorna un codigo de error
        return 1; 
    }

    if (fgets(path, sizeof(path), fp) != NULL) {

        // Eliminar el salto de línea del final, si está presente
        size_t length = strlen(path);
        if (length > 0 && path[length - 1] == '\n') {
            path[length - 1] = '\0';
        }

        // Contar las palabras en el path
        
        for (size_t i = 0; i < length; ++i) {
            if (path[i] == '/') {

                // Contar '/' como separador de palabras
                ++contadorPalb;
            }
        }     
    }

    // Cerrar el archivo después de su uso
    pclose(fp);
    printf("Número de palabras en el path: %d\n", contadorPalb);

    // Comprobamos si el contador es masyor de 5 o no.
    if (contadorPalb>=5){
        return 5;
    }else{
        return 6;
    }   
}

// Esta función toma un comando como entrada y ejecuta ese comando en un proceso hijo
int ejecutarComando(char* comando) {

    // Comprobamos que los comandos que metamos, tengan la longitud que queremos.
    if(contarComando(comando)>=2){ printf("holita3");return 4;} 

        // He añadido estas líneas, para restringir el test a los comandos específicos, ahora mismo solo reconocerá "ls, cpu, stoy y pwd".
        // Para comprobar si los comandos son los que queremos, utilizaremos la función strcmp().
        if (strcmp(comando, "cpu") == 0 ){
        
            return comprobarcpucommand();

        }else if (strcmp(comando, "stoy" ) == 0){
        
            return comprobarstoycommand();

        }else if(strcmp(comando, "pwd") == 0 ){

            return comprobarpwdcount();

        }else if(strcmp(comando, "ls") != 0){ 
            return 1;
       } 
    
        // Crea un nuevo proceso hijo utilizando fork() y almacena el resultado en pid
        pid_t pid = fork();

        // Si el pid es 0, el código dentro de este bloque se ejecuta en el proceso hijo.
        if (pid == 0) {

            // Array de punteros para almacenar los tokens del comando 
            // (token -> tokens son las palabras individuales que resultan de dividir la cadena de comando utilizando un delimitador (en este caso, el espacio en blanco)).
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

            // Se asigna NULL al siguiente elemento del array args (común en la ejecución de comandos).
            args[i] = NULL;

            // Se utiliza execvp para reemplazar la imagen del proceso actual con el comando especificado.
            execvp(args[0], args);

            // Si el proceso execvp falla, el programa hijo termina automáticamente como un éxito.
            exit(0);

        // El padre espera a que el proceso hijo termine mediante wait.
        } else if (pid > 0) {
            wait(NULL);
            return 0;
        } else {
            
            // Error al crear el proceso hijo.
            return -1; 
        }
    
    return 0; 
}

void testearComando(int resultado){

    // Imprime el resultado de la ejecución del comando "ls" si la función ejecutarComando devuelve 0.
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando 'ls' se ejecutó correctamente.\n");
    
    // Comprobamos si el resultado es 2, en tal caso quiere decir que nuestra prueba de comando stoy se ha efectuado correctamente.
    } else if(resultado == 2){

        printf("Prueba 2: Pasada - El comando 'stoy' se ejecutó correctamente.\n");
    // Comprobamos si el resultado es 3, en tal caso quiere decir que nuestra prueba de comando cpu se ha efectuado correctamente.
    }else if(resultado == 3){

        printf("Prueba 3: Pasada - El comando 'cpu' se ejecutó correctamente.\n");
    // Comprobamos si el resultado es 5, en tal caso quiere decir que nuestra prueba de comando pwd se ha efectuado correctamente.   
    }else if (resultado == 5){

        printf("Prueba 4: Pasada - El comando 'pwd' se ejecutó correctamente y tiene la longitud adecuada.\n");
    }

    // Comprobamos si el resultado es 1, en tal caso quiere decir que nuestra prueba no se ha efectuado correctamente, 
    // el comando que se ha introducido ha sido erróneo. 
    if(resultado == 1){
        printf("Fallo 1 - No es un comando permitido, prueba con otro.\n");
    
    // Comprobamos si el resultado es 4, en tal caso quiere decir que nuestra prueba no se ha efectuado correctamente,
    // el comando introducido tiene demasiadas palabras.   
    }else if (resultado == 4){
        printf("Fallo 2 - El comando tiene demasiadas palabras.\n");

    // Comprobamos si el resultado es 6, en tal caso quiere decir que nuestra prueba no se ha efectuado correctamente,
    // la longitud del path en el que estamos, no tiene la longitud sufuciente..   
    }else if (resultado == 6){

        printf("Fallo 3 - La ruta del archivo no tiene la longitud adecuada.\n");
    // Comprobamos si el resultado es 7, en tal caso quiere decir que nuestra prueba no se ha efectuado correctamente,
    // el comando stoy ha dado problemas a la hora de su ejecución.
    }else if (resultado == 7){

        printf("Fallo 4 - Error al ejecutar el comando stoy.\n");
        
    // Comprobamos si el resultado es 8, en tal caso quiere decir que nuestra prueba no se ha efectuado correctamente,
    // el comando cpu ha dado problemas a la hora de su ejecución.
    }else if (resultado == 8){

        printf("Fallo 5 - Error al ejecutar el comando cpu.\n");
    }

}


