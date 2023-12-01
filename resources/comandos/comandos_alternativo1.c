#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 

int main()
{
    char input[MAX_LINE]; 
    int n_comandos = 0;
    int should_run = 1;   

    while (should_run)
    {
        printf("MiShell> ");
        fflush(stdout);

        fgets(input, MAX_LINE, stdin); 
        input[strlen(input) - 1] = '\0'; 
        n_comandos++;
        
        if (strcmp(input, "exit") == 0) 
        {
            
            printf("Fin del programa.\n");
            if(n_comandos==1) printf("Se ha introducido un comando.\n");
            else printf("Se han introducido un total de %d comandos.\n", n_comandos);
            should_run = 0; 
        }
        else
        {
            pid_t pid = fork(); 

            if (pid == 0)
            {                                    
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

                execvp(args[0], args);

                exit(0); // Sale del proceso hijo
            }
            else
            {               
                wait(NULL); 
            }
        }
    }

    return 0; // Fin del programa
}