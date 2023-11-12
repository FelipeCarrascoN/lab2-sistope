#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "fbroker.h"
#include <sys/wait.h>



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
    //printf("entré al broker\n");
    //exit(1);
    int N = atoi(argv[1]); // Numero de celdas
    int wkrs = atoi(argv[2]); // Cantidad de workers
    char* archEntrada = argv[3]; 
    char* archSalida = argv[4];  // Nombres de entrada y salida de ficheros
    int chunk = atoi(argv[5]); // Tamaño del chunk, cantidad de lineas a leer por chunk
    int flagD = atoi(argv[6]); // Flag: Cuando se activa muestra por pantalla los resultados
    //printf("%d\n",flagD);
    //printf("%s\n",archSalida);
    int buffer = 250;
    //propuesta solucion broker.c

    printf("N: %d, P: %d, i: %s, o: %s, c: %d, D: %d\n", N, wkrs, archEntrada, archSalida, chunk, flagD);

    int pipePadreHijo[wkrs][2]; // Hijo escribe padre lee
    int pipeHijoPadre[wkrs][2]; // Padre escribe hijo lee

    int i;

    for(i=0; i<wkrs; i++){
        pipe(pipePadreHijo[i]);
        pipe(pipeHijoPadre[i]);
    }

    int pid[wkrs];
    for(i=0;i<wkrs;i++){
        pid[i] = fork();

        if(pid[i] == -1){
            //printf("hijo %d\n",i);
            printf("Error al crear el proceso hijo.\n");
            exit(1);
        }

        else if(pid[i] == 0){
            //estamos en el hijo
            fflush(stdout);

            dup2(pipeHijoPadre[i][1], STDOUT_FILENO);
            dup2(pipePadreHijo[i][0], STDIN_FILENO);
            close(pipeHijoPadre[i][0]);
            close(pipePadreHijo[i][1]);
            execl("./worker", "./worker", N, NULL);
            exit(0);
        }

        //Abrir archivo de entrada
        FILE * arch = fopen(archEntrada, "r");
        
        if(arch == NULL){
            printf("Error al abrir el archivo de entrada.\n");
            exit(1);
        }

        int cantParticulas;
        fscanf(arch, "%d", &cantParticulas);

        char lectura[64];
        char contenido[chunk][64];
        i=0;
        int workerEscogido;
        //int j=0;
        while(!feof(arch)){
            if(fgets(lectura[i], 64, arch) != NULL){
                i++;
                strcpy(contenido[i], lectura[i]);
                if (i == chunk){
                    i=0;
                    workerEscogido = rand() % wkrs;
                    write(pipePadreHijo[workerEscogido][1], contenido, chunk);
                }
            }
            
        }
        fclose(arch);

        char mensajeFin [5] = "FIN"; //dudoso la vdd, cambiar esto si no funca xdd
        for(i=0;i<wkrs;i++){
            write(pipePadreHijo[i][1], mensajeFin, 5);
            close(pipePadreHijo[i][1]);
        }

        for(i=0;i<wkrs;i++){
            wait(NULL);
            //Así espero a que los workers terminen, wait(NULL) espera a que cualquier hijo termine
            //entonces si hago que espere a que cualquier hijo termine wkrs veces
            //esperará hasta que wkrs procesos hijos terminen su ejecución.
        }
        
        /** aquí escribir archivo
        <------------------------>
        int escribirArchivo(archSalida, salida, N); está en fbroker.c/.h
        <------------------------>
        <------------------------>
        <------------------------>
        <------------------------>
        Revisar flag e imprimir gráfico por consola.
        <------------------------>
        void imprimirGrafico(int N, particulas * salida); está en fbroker.c/.h
        */
    }
    

// Pedir memoria para los pipes
    /*
    int **fd_padre_hijo = (int **)malloc(sizeof(int *) * wkrs); // Hijo escribe padre lee
    int **fd_hijo_padre = (int **)malloc(sizeof(int *) * wkrs); // Padre escribe hijo lee
    for (int i = 0; i < wkrs; i++)
    {
        fd_padre_hijo[i] = (int *)malloc(sizeof(int) * 2);
        fd_hijo_padre[i] = (int *)malloc(sizeof(int) * 2);
    }
    particulas *salida = (particulas *)malloc(sizeof(particulas) * N); // No estaba seguro si era un malloc o un arreglo estatico
    // Crear workers
    for (int i = 0; i < N; i++)
    {
        // Crear pipe
        if (pipe(fd_padre_hijo[i]) == -1 || pipe(fd_hijo_padre[i]) == -1)
        {
            printf("Error al crear el pipe");

            exit(1);
            // return 1;
        }

        // Crear un proceso hijo
        int pid = fork();
        if (pid == -1)
        {
            printf("Error al crear un nuevo proceso");
            exit(1);
        }

        if (pid == 0)
        {
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
            char *args[] = {"./worker", N_Celdas, fd_read, fd_write, chunk_str, NULL};

            // Ejecutar worker
            execv(args[0], args);

            printf("Error creando el worker");
            exit(1);
        }
    }

    srand(time(NULL));
    int numero = rand() % wkrs;

    // leer archivo por chuck lineas
    particulas entrada[N];
    lecturaArchivo(archEntrada, entrada, N);
    // entregar al worker escogido con rand
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < chunks; j++)
        {
            // enviar a worker
            // write(fd_padre_hijo[numero][1], entrada, entrada);
            write(fd_padre_hijo[numero][1], &entrada[i], sizeof(particulas));
            // recibir de worker no estoy seguro del orden en estas 2 xd
            // read(fd_hijo_padre[numero][0], entrada, entrada);
            read(fd_hijo_padre[numero][0], &salida[i], sizeof(particulas));
            // entrada[i];
        }
    }
    */


    //DUDAS:
    // Ordenamos las energías por posición antes de enviarlas?
    // Los valores recibidos por pipes se leen como se lee un archivo?
    // Hay que almacenar de alguna forma los calculos realizados por los hijos para todas las posiciones

}