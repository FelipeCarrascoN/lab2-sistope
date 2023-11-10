#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

// Entradas: argc y argv
// Salidas: int
// Descripción: Valida parametros de entrada e invoca a broker con execv
int main(int argc, char *argv[]){
    // Tienen que ser 5 o 6 argumentos (-b)
    if(argc < 5 || argc > 6){
        printf("[Lab2] Argumentos insuficientes.\n");
        exit(1);
    }

    // Getopt
    int opt, chunks, wkrs, N;
    int flagD = 0;
    char * archEntrada, * archSalida;

    while((opt = getopt(argc, argv, "N:P:i:o:c:D")) != -1){
        switch(opt){
            case 'N':
                N = atoi(optarg);
                break;
            
            case 'P':
                wkrs = atoi(optarg);
                break;
            
            case 'i':
                archEntrada = optarg;
                break;
            
            case 'o':
                archSalida = optarg;
                break;
            
            case 'c':
                chunks = atoi(optarg);
                break;

            case 'D':
                flagD = 1;
                break;
            
            default:
                return 1;
                
        }
    }

    if(chunks <= 0 || wkrs <= 0){
        printf("Valores de chunk y workers invalidos.\n");
        exit(1);
    }

    char N_str[10];
    sprintf(N_str, "%d", N);

    char c_str[10];
    sprintf(c_str, "%d", chunks);

    // Convertir a char* la cantidad de workers
    char w_str[10];
    sprintf(w_str, "%d", wkrs);
    
    // Crear Broker
    int pid = fork();
    if(pid == 0){
        
        // Crear lista de argumentos
        char* argumentos[] = {
            "./broker",
            N_str,
            w_str, 
            archEntrada, 
            archSalida,
            c_str, 
            flagD ? "1" : "0",
            NULL};

        // Ejecutar broker
        execv(argumentos[0], argumentos);

        printf("No se ha podido ejecutar el worker\n");
        exit(1);
    }
}