
//Declaración de las bibliotecas de C necesarias para ejecutar el código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*Se define una constante entera (int) con valor 80. 
El tipo se asigna de manera implícita*/
#define MAX_LINE 80

/*Se declara y describe una función llamada ejecutarComando. 
    -Esta función recibe por parámetros un tipo puntero a char 
    - Los punteros apuntan a una dirección de memoria */

int ejecutarComando(char* comando) {
	
	/*Se declara el tipo integer pid_t que representa el id de un proceso con el nombre "pid"
	 La función "fork()" crea un proceso hijo a través del proceso padre.*/
    pid_t pid = fork();

	//Si el valor de pid es igual a 0, se inicia el proceso hijo
    if (pid == 0) {
		
		//Se declara un array de punteros de longitd MAX_LINE dividada entre 2 más 1
        char *args[MAX_LINE / 2 + 1];
		
		/*El puntero token se le asigna el valor de la función strtok, necesita dos argumentos; una cadena que se tiene que dividir y un delimitador
		strtok divine una cadena (tokenización)*/
        char *token = strtok(comando, " ");
		
		// Se declara una variable entera int con nombre "i" con valor 0
        int i = 0;
		
		//Mientras token no sea null
        while (token != NULL) {
			
			//Se recorre el array de argumentos y se le asigna el valor de token en cada posición
            args[i] = token;
			
			/*Se vuelve a dividir la cadena, hasta que token sea NULL
			    -Solo en la primera tokenización le pasamos la cadena*/
            token = strtok(NULL, " ");
			//Se suma 1 a la  "i"
            i++;
        }
		
		//Se asigna a la última posición del arrray el valor NULL
        args[i] = NULL;

		/*TO DO: definir esto wtf*/

        /*execvp es una función que ejecuta un programa desde el programa en C
            -Busca el programa especifiado en el primer argumento
            -Ejecuta ese programa con los argumentos proporcioandos en el segundo argumento*/
        execvp(args[0], args);
		
		//La función exit(0) termina el proceso hijo
        exit(0);
		
		
	// Se espera al proceso hijo a que acabe
    } else if (pid > 0) {
        wait(NULL);
			
    } else {
        return -1; // Error al crear el proceso hijo
    }

    return 0; // Éxito
}
//Se declara la función main que inicia el programa
int main() {
	
	//Se declara un array de caracteres y se le da un valor
    char comando_inexistente[] = "xyzabc";
	
	// Se declara un entero que recogerá el resultado de la función "ejecutarComando(comando_inexistente)"
    int resultado = ejecutarComando(comando_inexistente);

	//Se realiza un test: si el resultado es -1 es que la prueba ha funcionada porque no reconoce el comando pasado por argumentos.
    if (resultado == -1) {
        printf("Prueba 2: Pasada - Error al ejecutar comando inexistente.\n");
	//Cualquier otro resultado es erróneo: el comando que no existe se ha popdido ejecutar
    } else {
        printf("Prueba 2: Fallida - El comando inexistente se ejecutó correctamente.\n");
    }
	//Termina main
    return 0;
}

