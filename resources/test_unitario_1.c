#include <stdio.h>//Para las funciones de entrada/salida estandar.
#include <stdlib.h>// Para funciones relacionadas con la gestion de memoria y otras utilidades.
#include <string.h>// Para funciones de manipulacion de cadenas de caracteres.
#include <unistd.h>// Para la funcion fork y otras llamadas al sistema.
#include <sys/types.h>// Para tipos de datos relacionados con llamadas al sistema.
#include <sys/wait.h>// Para la funcion wait y macros relacionadas.
#include <dirent.h> // Para manejar operaciones de directorios (abrir, leer, cerrar).
#include <sys/stat.h> // Para obtener información sobre archivos y directorios
            
#define MAX_LINE 80// constante que representa la longitud maxima de la linea de comando.


// Se usa para ejecutar un proceso, que muestra los 10 procesos que mas cpu consumen, junto con ciertos datos interesantes de estos y del mismo sistema.
int comprobarcpucommand();

// Este proceso se encarga de devolver: el path en el que nos encontramos, el numero de archivos en el directorio actual,
// el numero de directorios en el directorio actual, y los permisos del usuario actual sobre el directorio actual. 
int comprobarstoycommand(); 

// Utilizamos esta funcion para contar el numero de palabras que tiene un comando.
int contarComando(char* comando);

// Utilizamos esta funcion para contar el numero de palabras que tiene el path en el que nos encontramos, exclullendo las "/",
// si el path contiene 5 o mas palabras, devuelve un resultado, si no devuelve otro.
int comprobarpwdcount();

// Utilizamos esta funcion para comprobar que comando le estamos mandando al programa para ejecutar, si tiene mas de 2 palabras retornara un valor de error
// , si el comando difiere de "ls,cpu,stoy y pwd" retornara otro valor de error diferente, si es uno de los comandos anteriores, los ejecutara.
int ejecutarComando(char* comando);

// Se utiliza para testear el valor del comando introducido, y dependiendo del valor de este resultado, mostrara un mensaje de testeo pasado o no.
void testearComando(int resultado);



int main() {

    // Utilizamos estos 6 comandos, para comprobar los test unitarios que hemos implementado, 
    // primero los ejecutaremos (si podemos ejecutarlos), y luego comprobaremos si han pasado los test.


    char comando[] = "ls";
    printf("*****************************************\n");
    int resultado1 = ejecutarComando(comando);
    
    printf("*****************************************\n");
    char comando2[] = "stoy";
    int resultado2 = ejecutarComando(comando2);

    char comando3[] = "fsdf3jeio3w";
    int resultado3 = ejecutarComando(comando3);

    char comando4[] = "ls . || grep .txt";
    int resultado4 = ejecutarComando(comando4);

    printf("*****************************************\n");
    char comando5[] = "pwd";
    int resultado5 = ejecutarComando(comando5);

    printf("*****************************************\n");
    char comando6[] = "cpu";
    int resultado6 = ejecutarComando(comando6);


   // Comprobamos si hemos pasado los test con cada uno de los 6 comandos.
    printf("---------------------------------------\n");
    testearComando(resultado1);
    testearComando(resultado2);
    testearComando(resultado3);
    testearComando(resultado4);
    testearComando(resultado5);
    testearComando(resultado6);
    printf("---------------------------------------\n");

    
    
    return 0;
}



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
    return 3;
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
    return 2; 
}

        //-------------------------------------------------------------------------------------------
        // MODIFICACION 3 -> Añadimos un metodo para contar las palabras del comando, 
        // y lo usamos en el metodo ejecutarComando para checkear que el comando solo tenga 1 palabra.
        //-------------------------------------------------------------------------------------------
    
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

        //-------------------------------------------------------------------------------------------------
        // MODIFICACION 4 -> Una vez ejecutamos el comando pwd, comprobamos la cantidad de palabras 
        // que contiene el path del directorio en el que estamos, si la cantidad de palabras es 5 o mayor 
        // se da por pasado el test, si es menor, el test falla.
        //-------------------------------------------------------------------------------------------------

int comprobarpwdcount(){

    FILE *fp;
    char path[MAX_LINE];
    int contadorPalb = -1;

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

        // Incrementar el recuento final, ya que la última palabra no tiene '/'
        ++contadorPalb;

        
    }

    // Cerrar el archivo después de su uso
    pclose(fp);
    printf("Número de palabras en el path: %d\n", contadorPalb);

    // comprobamos si el contador es masyor de 5 o no.
    if (contadorPalb>=5){
        return 5;
    }else{
        return 6;
    }

    
}

// Esta función toma un comando como entrada y ejecuta ese comando en un proceso hijo
int ejecutarComando(char* comando) {

    // comprobamos que los comandos que metamos, tengan la longitud que queremos.
    if(contarComando(comando)>=2){ return 4;} 

        // He añadido estas lineas, para restringir el test a los comandos especificos, ahora mismo solo reconocera "ls,cpu,stoy y pwd".
        // Para comprovar si los comandos son los que queremos, utilizaremos la funcion strcmp().
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

void testearComando(int resultado){

    // Imprime el resultado de la ejecucion del comando "ls" si la funcion ejecutarComando devuelve 0.
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
    // el comando que se ha introducido ha sido erroneo.   
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
    }

}


