#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
char *args[MAX_LINE / 2 + 1];

int ejecutarComando(char *comando)
{
    pid_t pid = fork();
    int status_code = 100;
    //proceso hijo
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

//PRIMERA MODIFICACION ->

        int commad_status_code = execvp(args[0], args);
        /*since you want to return errno to parent
                            do a simple exit call with the errno*/
        printf("ESTO SOLO SALDRA SI EL COMANDO INTRODUCIDO NO EXISTE, joder\n");
        exit(commad_status_code);
     
    }
    else if (pid > 0)
    {

        //SEGUNDA MODIFICACION ->

        //esperamos al proceso hijo a que acabe para asi comprobar la respuesta del comando (osea su status code)
        waitpid(pid, &status_code,0);
        // if (status_code == EXIT_SUCCESS) {
        //     return EX
        // }
        if (status_code == 0){
            return EXIT_SUCCESS;
        }else {
            return EXIT_FAILURE;
        }
        //devolvemos el status code del proceso hijo que ha ejecutado el comando
        return status_code;
        // wait(NULL);
        //return EXIT_SUCCESS;

    }else {
        //fallo al crear el proceso hijo
        return -1;
    }

 // falta comentar todos los cambios y explicar de nuevo el funcionamiento
}

int main()
{
    char comando_inexistente[] = "sdsd";
    int resultado = ejecutarComando(comando_inexistente);

    if (resultado == EXIT_SUCCESS)
    {
        printf("Prueba 2: Fallida - el comando existe.\n"); //Modificacion de los textos para dejarlos más claros
    }
    else if (resultado == EXIT_FAILURE)
    {
        printf("Prueba 2: Pasada - El comando inexistente no se ha encontrado.\n");
    }

    return 0;
}
