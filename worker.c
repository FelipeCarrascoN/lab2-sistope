#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fworker.h"



const char* id;
int main(int argc, char const *argv[]){
        int N_Celdas = atoi(argv[1]);
        //particulas buffer[N_Celdas];
        int chunk = atoi(argv[4]);
        char* breaker;
        particulas * datos = (particulas *) malloc(sizeof(particulas)*chunk);
        char lectura[chunk][64];
        //particulas salida; // IDEM
        //Se calcula 
        /*
        while(1){
            read(STDIN_FILENO, &entrada, sizeof(particulas));
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
                        calcularEnerg(entrada, buffer, N_Celdas, chunk);
                    }
                }
                //Entregar la lista de particulas al broker
                write(STDOUT_FILENO, &buffer, sizeof(particulas));
            }
        }*/
        //Estoy recibiendo los valores como char
        //suponiendo chunk = 5
        //el formato de entrada de la lectura del archivo sería
        /*
        "3 70"
        "6 30"
        "12 10"
        "14 20"
        "16 30"
        */
        //Por lo que hay que transformarlo a numérico primero.
        
        //Ahora tienen el formato requerido para trabajarse en la fórmula
        /*
        datos = {
            [3,70],
            [6,30],
            [12,10],
            [14,20],
            [16,30]
        }
        */

       //Aquí hay que calcular el coso

        while(1){
            fflush(STDIN_FILENO);
            read(STDIN_FILENO, lectura, sizeof(particulas)*chunk);
            
            for (int i = 0; i < chunk; i++){
                sscanf("%d %lf", datos[i].posicion, datos[i].energia);
            }

            if(strstr(lectura, "FIN") != NULL){
                break;
            }
            else{
                calcularEnerg(datos, lectura, N_Celdas, chunk);
            }
            write(STDOUT_FILENO, datos, sizeof(particulas)*chunk); //aquí hay que cambiar "datos" por la variable que tenga la info calculada
            
        }

        free(datos);

    return 0;

}

