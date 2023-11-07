#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigint_handler(int sig) {
    write(1, "Ahhh! SIGINT!\n", 14);
} // Funcion que maneja el handler, cuando se reciba la senal
 //SIGING (control+c) por el interprete de bash


int main(void) 
{
    char s[200];
    struct sigaction sa;//Creamos la estructura del tipo sigaction 
                        // que contiene los siguientes campos
                        // sa_handler : Función handler (o SIG_IGN)
                        //sa_mask : Indica si otras signals pueden interrumpir a este handler o no.(ver sigaddset())
                        //  sa_flags : Modifican el comportamiento del handler.
    sa.sa_handler = sigint_handler; //este es un puntero a funcion, en este caso le asignamos la funcion que manejara el handler
    sa.sa_flags = 0; //SA_RESTART;
    sigemptyset(&sa.sa_mask);//cargamos la mascara con cero
    if (sigaction(SIGINT, &sa, NULL) == -1) //aqui llamamos la funcion que escuchara a la senal SIGINT (PRIMER ARGUMENTO)
    {                                      // Y Le pasamos la estructura creada
        perror("sigaction");
        exit(1);
    }
    printf("Enter a string:\n");
    if(fgets(s, sizeof s, stdin) == NULL)
        perror("fgets");
    else
        printf("You entered: %s\n", s);
    return 0;
}