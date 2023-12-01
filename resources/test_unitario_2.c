//Librerias necesarias para la utilización de algunas funciones y variables 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

// Declaración de variables globales
#define MAX_LINE 80
char *args[MAX_LINE / 2 + 1];

//Declaracion de las funciones que se van a utilizar
int buscarPalabra(char *comando);
int comprobarPermisos(char *ruta);
int buscarNULL(char *valor);
int esFichero(char *ruta);
int reemplazar(char *buscar, char *reemplazo, char *ruta);
int ejecutarComando(char *comando);

//----------------------------------------------------------------------------------------------------------
    // MODIFICACION 2 -> Se crea una función que busque una palabra en todos los archivos de un directorio
    // Sería como el ctrl + f de Visual Studio
    // Nos muestra todas las líneas en todos los archivos que se encuentre la palabra buscada. 
    // Creamos esta función para usar de manera más intuitiva el comando "grep"
//---------------------------------------------------------------------------------------------------------

int buscarPalabra(char *comando)
{
    // Comprobar que el comando está correctamente formado
    int i = 0;
    pid_t pid;
    // IMPORTANTE: SE TIENE QUE INICIALIZAR A NULL LAS VARIABLES,c a veces no inicia los valores de forma correcta
    char *cmd = NULL, *directorio = NULL, *palabra = NULL;

    // Dividir *comando por espacios en blanco
    const char *pch = strtok((char *)comando, " ");

    while (pch != NULL)
    {
        // Asignar los valores que se han obtenido de divir el array
        if (i == 0)
        {
            cmd = pch;
        }
        else if (i == 1)
        {
            palabra = pch;
        }
        else if (i == 2)
        {
            directorio = pch;
        }
        pch = strtok(NULL, " ");
        i++;
    }

    // Comprobamos que es el comando correcto. 
    // Usamos "buscar" para indicar a la función que se deberá usar grep después
    if (strcmp("buscar", cmd) != 0)
    {
        return -1;
    }
    //Se han introducido más de 3 valores en el array
    if (i > 3)
    {
        return -127;
    }
    // No se ha introducido una palabra a buscar
    if (!palabra)
    {
        return -126;
    }
    // No se ha introducido un directorio
    if (!directorio)
    {
        return -128;
    }

    // Ejecutamos el comando con las instrucciones en un proceso hijo. Usaremos un comando del cmd de linux para realizar esta búsqueda
    char *comandoGrep = "grep";
    //Se almacenan los argumentos que se pasarán a una llamada al comando "grep"
    char *lista_argumentos[] = {comandoGrep, "-rni", palabra, directorio, NULL};
    
    //Se crea un proceso hijo
    pid = fork();
    if (pid == 0)
    {   
        //Se usa la función execvp para que muestre el resultado
        int status_code = execvp(comandoGrep, lista_argumentos);
        return EXIT_SUCCESS;
    }
    else if (pid > 0)
    {
        wait(NULL);
        return EXIT_SUCCESS;
    }
    else
    {  //No se ha podido crear el proceso hijo
        return -1;
    }
}

        //----------------------------------------------------------------------------------------------------------
                     // MODIFICACION 3 -> Se crea una función que busque una palabra en un fichero y la sustituya por otra
                    // Dará exito si se ha encontrado la palabra y se ha podido sustituir por otra
                    //Se comprueban si los valores dados a la función son nulos o vacíos. De ser así retorna error
        //----------------------------------------------------------------------------------------------------------


// Funcion que comrpueba si se tienen permisos sobre el fichero a modificar
int comprobarPermisos(char *ruta)
{    //La función accces, comprueba los persmisos que se tienen. W_OK comprueba permisos de escritura (W -> write)
    if (access(ruta, W_OK) == 0)
    {
        return 0;
    }
}

// Funcion que comprueba que los datos no son NULL o están vacíos
int buscarNULL(char *valor)
{

    if (valor == NULL)
    {
        return 0;
    }
    else if (valor[0] == '\0')
    {
        return 0;
    }
}

// Funcion que nos comprueba si lo que se le pasa a la ruta es un fichero
int esFichero(char *ruta)
{
    struct stat path_stat;
    stat(ruta, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int reemplazar(char *buscar, char *reemplazo, char *ruta)
{

    // Comprobamos si los valores de no son NULL o están vacíos

    int resultadoBuscar = buscarNULL(buscar);
    if (resultadoBuscar == 0)
    {
        return EXIT_FAILURE;
    }

    int resultadoReemplazo = buscarNULL(reemplazo);
    if (resultadoReemplazo == 0)
    {
        return EXIT_FAILURE;
    }

    int resultadoRuta = buscarNULL(ruta);
    if (resultadoRuta == 0)
    {
        return EXIT_FAILURE;
    }

    // Comprobamos si la ruta es un fichero
    int resultado = esFichero(ruta);
    if (resultado == 0)
    {
        return EXIT_FAILURE;
    }

    // Abrimos el fichero, si está vacío retornar error
    FILE *file = fopen(ruta, "r+");
    if (file == NULL)
    {
        return -1;
    }

    // Tamaño suficiente para almacenar una palabra del archivo
    char palabra_fichero[100];

    // Bucle que nos lee la información del fichero hasta el final del mismo
    while (fscanf(file, "%s", palabra_fichero) != EOF)
    {

        // Compara las palabra encontradas en el fichero una por una
        // Si es igual con la palabra a reemplazar, devuelve 0
        if (strcmp(palabra_fichero, buscar) == 0)
        {
            fseek(file, -strlen(buscar), SEEK_CUR);
            fprintf(file, "%s", reemplazo);
        }
    }
    // Cerramos el fichero
    fclose(file);
    // Si todo está bien, retorna 0
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
    //      MODIFICACION 1 -> Arreglar el funcionamiento de la función ejevutarComando()
    //      Cuando se hacía el test, daba igual que comando introdujeras que siempre arrojaba el mismo resultado
//------------------------------------------------------------------------------------------------------------------

int ejecutarComando(char *comando)
{
    pid_t pid = fork();
    int status_code = 100;

    // proceso hijo
    if (pid == 0)
    {
        char *token = strtok(comando, " ");

        int i = 0;
        while (token != NULL)
        {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        // 1. MODIFICACION -> se captura en una variable el resutlado de ejecutar execvp
        int commad_status_code = execvp(args[0], args);

        // Si execvp funciona, no se escribe lo demás a partir de aquí
        printf("ESTO SOLO SALDRA SI EL COMANDO INTRODUCIDO NO EXISTE\n");
        exit(commad_status_code); //
    }
    else if (pid > 0)
    {

        // Esperamos al proceso hijo a que acabe para asi comprobar la respuesta del comando (osea su status code)
        waitpid(pid, &status_code, 0);
        
        if (status_code == 0)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
        // devolvemos el status code del proceso hijo que ha ejecutado el comando
        return status_code;
    }
    else
    {
        // fallo al crear el proceso hijo
        return -1;
    }

    // Se ha eliminado el return del final de la función
}

int main()
{

    //-------------------------------------------------------------------------------------------
    //               TEST DE LA FUNCIÓN BUSCAR PALABRA EN TODOS LOS ARCHIVOS DE UN DIRECTORIO
    //-------------------------------------------------------------------------------------------

    // Indicamos el comando, la palabra y el directorio en un mismo array
    char comando[] = "buscar printf /home/alumno";
    
     // Guardamos el resultado de pasarle el comando
    int resultado = buscarPalabra(comando);

    
    if (resultado == EXIT_SUCCESS)
    {
        printf("Prueba 3: Exito - se ha encontrado la palabra. \n");
    }
    else if (resultado == -127)
    {
        printf("Prueba 3: Fallo - se han introducido más de 3 argumentos. \n");
    }
    else if(resultado == -126){

        printf("Prueba 3: Fallo - no se ha introducido la palabra a buscar. \n");
    }
      else if(resultado == -128){

        printf("Prueba 3: Fallo - se ha introducido un directorio. \n");
    }
    else
    {
        printf("Prueba 3: Fallo - no se ha ejecutado correctamente. \n");
    }

    //-------------------------------------------------------------------------------------------
    //                   TEST DE FUNCION REEMPLAZAR PALABRA POR OTRA
    //-------------------------------------------------------------------------------------------

    // Definimos la ruta a buscar
    char ruta[] = "/home/alumno/prueba.txt";

    // Guardamos el resultado de pasarle una palabra a buscar, una palabra a cambiar y la ruta
    int resultado3 = reemplazar("burpepes", "cc", ruta);

    // TEST:
    if (resultado3 == 0)
    {
        printf("Prueba 4: éxito - se han cambiado los datos con éxito.\n");
    }
    // Si el resultado es
    else if (resultado3 == EXIT_FAILURE)
    {
        printf("Prueba 4: fallo - los datos son nulos.\n");
    }
    else
    {
        printf("Prueba 4: fallo - no se han encontrado los datos\n");
    }


    //-------------------------------------------------------------------------------------------
    //               TEST DEL CORRECTO FUNCIONAMIENTO DE LA FUNCION EJECUTAR COMANDO
    //-------------------------------------------------------------------------------------------

    // Primero se comprueba si el test funciona pasandole un comando que SI existe

    char comando_existente[] = "ls";
    // Se guarda en una variable entera el resultado de ejecutar la funcion
    int resultadoexistene = ejecutarComando(comando_existente);

    if (resultadoexistene == EXIT_SUCCESS)
    {
        // Modificacion -> los textos se han dejado más claros
        printf("Prueba 2: Fallida - el comando existe.\n");
    }
    else if (resultadoexistene == EXIT_FAILURE)
    {
        printf("Prueba 2: Pasada - El comando inexistente no se ha encontrado.\n");
    }

    // Segundo TEST se comprueba si el test funciona pasandole un comando que NO existe

    char comando_inexistente[] = "asdfgh";
    int resultadoinexistente = ejecutarComando(comando_inexistente);

    if (resultadoinexistente == EXIT_SUCCESS)
    { 
        // Modificacion de los textos para dejarlos más claros
        printf("Prueba 2: Fallida - el comando existe.\n");
    }
    else if (resultadoinexistente == EXIT_FAILURE)
    {
        printf("Prueba 2: Pasada - El comando inexistente no se ha encontrado.\n");
    }

    return 0;
}
