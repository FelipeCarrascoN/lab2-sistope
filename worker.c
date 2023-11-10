#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fworker.h"



const char* id;
int main(int argc, char const *argv[]){
        int N_Celdas = atoi(argv[1]);
        particulas buffer[N_Celdas];
        int chunk = atoi(argv[4]);
        char* breaker;
        particulas entrada; // no estoy seguro del valor de si es particulas o char* o de otro tipo de dato
        particulas salida; // IDEM
        //Se calcula 
        while(1){
            read(STDIN_FILENO, &entrada, buffer);
            //Habria que hacer que el primer valor de entrada en energia cambie a -1 en el broker
            //No supe como sacar este error
            if(entrada[0].energia == -1){
                breaker = "FIN";
            }
            if(strcmp(breaker, "FIN") == 0){
                break;
            }
            else{
                for (int i = 0; i < N_Celdas; i++){
                    if(buffer[i].posicion == i){
                        calcularEnerg(entrada, salida, N_Celdas, chunk);
                    }
                }
                //Entregar la lista de particulas al broker
                write(STDOUT_FILENO, &salida, buffer);
            }
        }
    return 0;

}

