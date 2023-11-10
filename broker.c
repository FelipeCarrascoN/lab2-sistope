
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "fbroker.h"

/*int main(){
    int A = 1;
    int pid;
    int fd[2];
    
    if(pipe(fd) == -1)
        return 0;
    
    pid = fork();
    
    if(pid == 0){
        char buffer[10];
        close(fd[1]);
        read(fd[0],buffer, sizeof(char)*10);
        
        printf("soy el hijo y tengo el mensaje %s\n", buffer);
    }else{
        char buffer[10] = "hola hijo";
        
        close(fd[0]);
        write(fd[1],buffer,sizeof(char)*10);
    }
    
    return 0;
}*/


/*  TO DO 
*   1.- Cambiar entrada por línea de comandos, archivo entrada, archivo salida, cantidad de workers, cantidad de chunks.
*   2.- Bloque main validar los argumentos. (hecho)
*   3.- Validados los datos ejecutar broker con fork, y tmbn ejecutar exec.
*   4.- Broker recibe argumentos del proceso padre y los usa para las tareas necesarias.
*   5.- Broker llama a los worker mediante exec.
*   6.- Broker se comunica con workers mediante pipes.
*   7.- Broker lee el archivo por "chunks" lineas.
*   8.- Broker elige un worker random para trabajar esas líneas.
        -> Cada worker procesa esas líneas.
        -> Cada worker realiza el calculo de cada chunk de lineas
        -> Si el worker recibe "FIN" deberá dejar de esperar líneas desde el broker y entregar a este la cantidad de líneas procesadas.
        -> Si el worker recibe "FIN" deberá terminar su ejecución. exit(0)
*   9.- Broker avisa a los workers si no queda archivo por leer mediante "FIN"
*  10.- Broker retoma el control cuando todos los workers terminan su ejecución.
*  11.- Broker escribe el archivo de salida con los resultados.
*  12.- Flag -D ... (hecho)
*  13.- Si flag -D existe, indicar cuantas líneas procesó cada worker, indicando su PID o un ID asignado.
*  14.- Terminar ejecución, de broker, workers y del proceso padre.
*/



    
    
    
    //Ya tenemos la cantidad de workers necesarios, debemos crearlos y también crear la cantidad correspondientes de pipes
    //deberán ser 2 * cantida de workers
    
    //creo que en la función de lectura debería ir el llamado a los workers, para que así cada cierta cantidad de lineas se llamen a los workers.
     
    
    //aquí hay q pasarle los valores a través de pipes desde el broker.
 //   int fd[2];
 //   if(pipe(fd) == -1){
 //       perror("error lol");
 //       exit(EXIT_FAILURE);
 //   }
    // Se debe hacer fork aquí para que sean los hijos los que trabajen en procesar los datos
    // fork lol
    // aquí la info se pasa a través de dup2, en std_id
    
    //https://chat.openai.com/share/3c014f3e-b77b-4919-9003-da6b808956d5


// Entradas: argc y argv
// Salidas: int
// Descripción: Valida parametros de entrada e invoca a broker con execv
int main(int argc, char *argv[]){

    int N = atoi(argv[1]); // Numero de celdas
    int wkrs = atoi(argv[2]); // Cantidad de workers
    char* archEntrada = argv[3]; 
    char* archSalida = argv[4];  // Nombres de entrada y salida de ficheros
    int chunks = atoi(argv[5]); // Tamaño del chuck, cantidad de lineas a leer por chunk
    int flagD = atoi(argv[6]); // Flag: Cuando se activa muestra por pantalla los resultados

    int buffer = 250;
    //propuesta solucion broker.c


// Pedir memoria para los pipes
    int** fd_padre_hijo = (int**) malloc(sizeof(int* ) * wkrs); // Hijo escribe padre lee
    int** fd_hijo_padre = (int**) malloc(sizeof(int* ) * wkrs); // Padre escribe hijo lee
    for(int i = 0; i < wkrs; i++) {
        fd_padre_hijo[i] = (int*) malloc(sizeof(int) * 2);
        fd_hijo_padre[i] = (int*) malloc(sizeof(int) * 2);
    }
    particulas * salida = (particulas*) malloc(sizeof(particulas) * N); // No estaba seguro si era un malloc o un arreglo estatico
    // Crear workers
    for(int i = 0; i < N; i++){
        // Crear pipe
        if(pipe(fd_padre_hijo[i]) == -1 || pipe(fd_hijo_padre[i]) == -1){
            printf("Error al crear el pipe");
            exit(1);
        }

        // Crear un proceso hijo
        int pid = fork();
        if(pid == -1){
            printf("Error al crear un nuevo proceso");
            exit(1);
        }   

        if (pid == 0){
            // Reemplazar la entrada y salida estandar por los pipes
            dup2(fd_padre_hijo[i][0], STDIN_FILENO);
            dup2(fd_hijo_padre[i][1], STDOUT_FILENO);

            // Id del worker
            char N_Celdas[wkrs];
            sprintf(N_Celdas, "%d", N);

            // FD para lectura
            char fd_read[buffer];
            sprintf(fd_read, "%d", fd_padre_hijo[i][0]);

            // Canal de escritura del hijo
            char fd_write[buffer];
            sprintf(fd_write, "%d", fd_hijo_padre[i][1]);

            // Tamaño del chunk
            char chunk_str[buffer];
            sprintf(chunk_str, "%d", chunks);

            // Argumentos
            char* args[] = {"./worker", N_Celdas,fd_read, fd_write, chunk_str, NULL};

            // Ejecutar worker
            execv(args[0], args);

            printf("Error creando el worker");
            exit(1);
        }
    }

    
    srand(time(NULL));
    int numero = rand() % wkrs;

    //leer archivo por chuck lineas
    particulas * entrada;
    int part = lecturaArchivo(archEntrada, &entrada, N);
    //entregar al worker escogido con rand
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < chunks; j++){
            //enviar a worker 
            write(fd_padre_hijo[numero][1], entrada, &salida);
            //recibir de worker no estoy seguro del orden en estas 2 xd
            read(fd_hijo_padre[numero][0], entrada, salida);
            // entrada[i];
        }
    }
    
    //DUDAS:
    // Ordenamos las energías por posición antes de enviarlas?
    // Los valores recibidos por pipes se leen como se lee un archivo?
    // Hay que almacenar de alguna forma los calculos realizados por los hijos para todas las posiciones

}