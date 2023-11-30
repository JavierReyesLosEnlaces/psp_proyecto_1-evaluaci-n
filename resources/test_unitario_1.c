#include <stdio.h>//Para las funciones de entrada/salida estandar.
#include <stdlib.h>// Para funciones relacionadas con la gestion de memoria y otras utilidades.
#include <string.h>// Para funciones de manipulacion de cadenas de caracteres.
#include <unistd.h>// Para la funcion fork y otras llamadas al sistema.
#include <sys/types.h>// Para tipos de datos relacionados con llamadas al sistema.
#include <sys/wait.h>// Para la funcion wait y macros relacionadas.
#include <dirent.h> // Para manejar operaciones de directorios (abrir, leer, cerrar).
#include <sys/stat.h> // Para obtener información sobre archivos y directorios
            
#define MAX_LINE 80// constante que representa la longitud maxima de la linea de comando.

        //-------------------------------------------------------------------------------------------
        //MODIFICACION 1 -> Comprobamos si el comando ejecutado es cpu10, dentro del metodo ejecutarComando(),
        // si lo es, se ejecutara un metodo el cual se llama comprobarcpucommand() y ejecuta un comando para ver cada 3 segundos,los 10 procesos que mas cpu consumen.
        // Para salir del proceso, se usara ctrl + C, si nos sale el mensaje 'Prueba 2: Pasada' significa que el test se ha realizado correctamente.
        //-------------------------------------------------------------------------------------------

int comprobarcpucommand() {

    printf("Va a dejar la ventana actual (Para salir presione ctrl + C), presiona enter.....");

    // Asegura que el mensaje se imprima antes de la pausa
    fflush(stdout); 
    getchar();
    pid_t pid = fork();
    int pidhijo;
    if(pid == 0){
        pidhijo= (int) getpid();

        // Con este comando conseguimos mostrar una pequeña lista de 10 procesos que mas cpu consumen, esta se refrescara cada 3 segundos
        char newComandoCpu[] = "watch -n 3 \'free; echo; uptime; echo; ps aux --sort=-%cpu | head -n 11; echo; who\'";
        
        system(newComandoCpu);// se ejecuta el comando
        
    }else if (pid<0){
        printf("Error al crear el proceso hijo del comando cpu.");
        return -1;
    }else{
        printf("se ha pasado al proceso padre del comando cpu.");
        wait(NULL);
    }
    return 2;
}

        //-------------------------------------------------------------------------------------------
        //MODIFICACION 2 -> Usamos este metodo, para manejar la informacion de nuestro directorio, 
        // con el mostramos diferente informacion del directorio como: la ruta, el numero de archivos, 
        // el numero de directorios, y los permisos del directorio en el que estamos.
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

        // Retorna un codigo de error
        return 1; 
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
        return 1;
    }

    // Cerrar el flujo del proceso
    if (pclose(fp) == -1) {
        perror("Error al cerrar popen");
        return 1;
    }

    // Se termina el test con Exito.
    return 3; 
}

        //-------------------------------------------------------------------------------------------
        // MODIFICACION 3 -> Añadimos un metodo para contar las palabras del comando, 
        // y lo usamos en el metodo ejecutarComando para checkear que el comando solo tenga 1 palabra.
        //-------------------------------------------------------------------------------------------
    

// Utilizamos esta funcion para contar el numero de palabras que tiene un comando.
int contarComando(char* comando){
    int contador = 0;

    // Divide el comando en diferentes tokens, los cuales usaremos para contar las palabras.
    const char *token = strtok((char *)comando, " ");
    while (token != NULL) {
        contador++;
        token = strtok(NULL, " ");
    }

    return contador;
}

// Esta función toma un comando como entrada y ejecuta ese comando en un proceso hijo
int ejecutarComando(char* comando) {

    // comprobamos que los comandos que metamos, tengan la longitud que queremos.
    if(contarComando(comando)>=2){ return 4;} 

        // He añadido estas lineas, para restringir el test a los comandos especificos, ahora mismo solo reconocera "ls,cpu y stoy".
        // Para comprovar si los comandos son los que queremos, utilizaremos la funcion strcmp().
        if (strcmp(comando, "cpu") == 0 || contarComando(comando)>=2){
        
            return comprobarcpucommand();

        }else if (strcmp(comando, "stoy" ) == 0 || contarComando(comando)>=2){
        
            return comprobarstoycommand();
        }else if(strcmp(comando, "ls") != 0){ 
            return 1;
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
            return 0;
        } else {
            // Error al crear el proceso hijo.
            return -1; 
        }
    

   
    return 0; 
}


int main() {

    // guardamos nuestro comando en un array de caracteres, y posteriormente se lo pasamos al metodo ejecutarComando().
    char comando[] = "ls";
    int resultado = ejecutarComando(comando);
    //int resultado2 = ejecutarComandoPersonalizado()

    
    // Imprime el resultado de la ejecucion del comando "ls" si la funcion ejecutarComando devuelve 0.
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando 'ls' se ejecutó correctamente.\n");
    } else if(resultado == 1){

        printf("Fallo 1 - No es un comando permitido, prueba con otro.\n");

    // comprobamos si el resultado es 2, en tal caso quiere decir que nuestra prueba de comando cpu10 se ha efectuado correctamente.
    }else if(resultado == 2){

        printf("Prueba 2: Pasada - El comando 'cpu' se ejecutó correctamente.\n");

    // comprobamos si el resultado es 2, en tal caso quiere decir que nuestra prueba de comando cpu10 se ha efectuado correctamente.
    }else if(resultado == 3){

        printf("Prueba 3: Pasada - El comando 'stoy' se ejecutó correctamente.\n");
    }else if (resultado == 4){
        printf("Fallo 2 - El comando tiene demasiadas palabras.\n");
    }
    return 0;
}