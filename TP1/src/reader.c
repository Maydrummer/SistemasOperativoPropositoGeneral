#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300


//Prototipos de funciones
FILE* abrirArchivo(const char* nombreArchivo, const char* modo);

//Variables globales
uint8_t inputBuffer[BUFFER_SIZE];
int32_t bytesRead, returnCode, fd;
const char comparador_dato[]="DATA:";
const char comparador_sign[]="SIGN:";
int main(void)
{
    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }
    
    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for writers...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a writer\n");

    FILE* p_log = abrirArchivo("log.txt", "w");
    FILE* p_signals = abrirArchivo("sign.txt", "w");
    /* Loop until read syscall returns a value <= 0 */
	do
	{
        /* read data into local buffer */
		if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1)
        {
			perror("read");
        }
        else
		{
			inputBuffer[bytesRead] = '\0';
            printf("El input es:%s \n",inputBuffer);
			printf("reader: read %d bytes:%s\n", bytesRead, inputBuffer);
            //Clasificar si es dato o signal
            if( strstr(inputBuffer,comparador_dato) != NULL )
            {
                fputs(inputBuffer,p_log);   
            }
            else if( strstr(inputBuffer, comparador_sign) != NULL )
            {
                fputs(inputBuffer,p_signals);   
            }
		}
	}
	while (bytesRead > 0);
    fclose(p_log);
    fclose(p_signals);
	return 0;
}

//Desarrollo de funciones

FILE* abrirArchivo(const char* nombreArchivo, const char* modo) {
    FILE* fptr = fopen(nombreArchivo, modo);

    // Verificar si la apertura fue exitosa
    if (fptr == NULL) {
        printf("No se pudo abrir el archivo '%s'. El programa se cerrará.\n", nombreArchivo);
        exit(EXIT_FAILURE);
    }

    return fptr;
}