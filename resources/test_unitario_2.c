#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_LINE 80
char *args[MAX_LINE / 2 + 1];

//TODO: comentar todo el codigo y que hace y las moficaciones

int esFichero(char* ruta){

    struct stat path_stat;
    stat(ruta, &path_stat);
    return  S_ISREG(path_stat.st_mode);
}

int reemplazar(char* buscar, char* reemplazo, char* ruta){

    //TODO: falta hacer comprobaciones de que los valores no sean nulos
    // TODO: comprobar que se tienen permisos sobre el archivo


    int resultado = esFichero(ruta);
    if (resultado == 0)
    {
        return EXIT_FAILURE;
    }

    FILE *file = fopen(ruta, "r+");
    if (file == NULL) {
        return -1;
    }

    char palabra_fichero[100]; // Tamaño suficiente para almacenar una palabra del archivo

    while (fscanf(file, "%s", palabra_fichero) != EOF) {
        if (strcmp(palabra_fichero, buscar) == 0) {
            fseek(file, -strlen(buscar), SEEK_CUR);
            fprintf(file, "%s",reemplazo );
        }
    }
    fclose(file);
    return 0;
 
}

int buscarPalabra(char *comando)

{
    // Comprobar que el comando está correctamente formado
    int i = 0;
    pid_t pid;
    char *cmd = NULL, *directorio = NULL, *palabra = NULL; // SMI-> SE TIENE QUE INICIALIZAR A 0 PORQUE C POR DEFECTO HACE LO QUE LE SALE DEL MOÑO

    const char *pch = strtok((char *)comando, " "); // Dividir *comando por espacios en blanco

    while (pch != NULL)
    {
        if (i == 0)
        { // Asignar los valores que se han obtenido de divir el array
            cmd = pch;
        }
        else if (i == 1)
        {
            palabra = pch;
        }
        else if (i == 2)
        {
            printf("AAAAAAAAA\n");
            directorio = pch;
        }
        pch = strtok(NULL, " ");
        i++;
    }
    if (strcmp("buscar", cmd) != 0)
    { // comprobamos que es el comando correcto
        return -1;
    }
    if (i > 3)
    {
        return -127;
    }
    // No se ha introducido una palabra a buscar

    if (!palabra)
    {
        return -126;
    }

    if (!directorio)
    {
        return -128;
    }

    // Ejecutamos el comando con las instrucciones en un proceso hijo

    char *comandoGrep = "grep";
    char *lista_argumentos[] = {comandoGrep, "-rni", palabra, directorio, NULL};
    pid = fork();
    if (pid == 0)
    {
        printf("Es la hora del kk %s, %s\n", palabra, directorio);
        int status_code = execvp(comandoGrep, lista_argumentos);
        return EXIT_SUCCESS;
    }
    else if (pid > 0)
    {
        wait(NULL);
        return EXIT_SUCCESS;
    }
    else
    {
        return -1;
    }
}

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

        // 1. MODIFICACION ->

        int commad_status_code = execvp(args[0], args);
        // Si execvp funciona, no se escribe lo demás a partir de aquí
        printf("ESTO SOLO SALDRA SI EL COMANDO INTRODUCIDO NO EXISTE, joder\n");
        exit(commad_status_code); //
    }
    else if (pid > 0)
    {

        // esperamos al proceso hijo a que acabe para asi comprobar la respuesta del comando (osea su status code)
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
        // wait(NULL);
        // return EXIT_SUCCESS;
    }
    else
    {
        // fallo al crear el proceso hijo
        return -1;
    }

    // TO DO: falta comentar todos los cambios y explicar de nuevo el funcionamiento
}

int main()
{
    char comando_inexistente[] = "ls";
    int resultado = ejecutarComando(comando_inexistente);

    if (resultado == EXIT_SUCCESS)
    {
        printf("Prueba 2: Fallida - el comando existe.\n"); // Modificacion de los textos para dejarlos más claros
    }
    else if (resultado == EXIT_FAILURE)
    {
        printf("Prueba 2: Pasada - El comando inexistente no se ha encontrado.\n");
    }

    char comando[] = "buscar Prueba /home/alumno"; // indicamos el comando, la palabra y el directorio
    
    int resultadoVis = buscarPalabra(comando);

    if (resultadoVis == EXIT_SUCCESS)
    {
        printf("Prueba 3: Exito - se ha ejecutado correctamente. \n");
    }
    else
    {
        printf("Prueba 3: Fallo - no se ha ejecutado correctamente. \n");
    }
    
//test para el programa de buscar 
    char ruta []= "/home/alumno/prueba.txt";
    int resultado =  reemplazar("patacas", "gatetes", ruta);
    if (resultado == 0){
        printf("TODO OK\n");
    }
    else {
        printf("NO TODO OK\n");
    } 


    

    return 0;
}
