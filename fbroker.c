#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fbroker.h"

#define buffer 255

int lecturaArchivo(char * nombre, particulas entrada[], int N){
    FILE * file = NULL;
    file = fopen(nombre, "r");
    //En caso de que haya un error en el archivo
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        exit(1);
    }
    //Se define la variable part
    int part;
    fscanf(file, "%d", &part);

    for(int i=0;i<part;i++){
        fscanf(file, "%d %lf", &entrada[i].posicion, &entrada[i].energia);
    }
    fclose(file);
    //Se retorna la cantidad de particulas existentes
    return part;

}

int indice(int posicion, particulas * entrada, int part){
    int i;

    for(i=0;i<part;i++){
        if(entrada[i].posicion == posicion){

            return i;
        }
    }

    return -1;
}

void ordenarEnergiaActual(particulas * entrada, int N, particulas ** salida, int part){
    int indiceS;
    particulas * aux = (particulas*) malloc (sizeof(particulas)*N);
    for(int i=0;i<N;i++){
        indiceS = indice(i, entrada, part);
        
        if (indiceS!=-1){
            
            aux[i].energia = entrada[indiceS].energia;
            aux[i].posicion = i;
            
        }
    }
    *salida = aux;
}

int maximo(particulas * entrada, int N){
    int max = 0;
    for (int i = 0; i<N ; i++){
        if(entrada[max].energia < entrada[i].energia){
            max = i;
        }
    }

    return max;
}

int escribirArchivo(int N, particulas * salida, char * nombre){
    FILE * file = NULL;
    file = fopen(nombre, "w");
    //En caso de que haya un error en el archivo
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    int max = maximo(salida, N);
    /*printf("maximo= %d\n", max);
    printf("Escribo= %d %lf\n", max, salida[max].energia);*/
    
    fprintf(file, "%d %lf\n", max, salida[max].energia);

    for(int i=0;i<N;i++){
        fprintf(file, "%d %lf\n", i, salida[i].energia);
        //printf("Escribiendo= %d %lf\n", i, salida[i].energia);
    }

    fclose(file);
    return 0;
}

void imprimirGrafico(int N, particulas * salida){
    //quiero imprimir por pantalla una representación cada 100 unidades en salida[i].energia por cada partícula
    for(int i=0;i<N;i++){
        int part = salida[i].energia/100;
        printf("%d\t%lf|",i,salida[i].energia);
        for(int j=0;j<part;j++){
            printf("%c",'o');
        }
        printf("\n");
    }
}

// Entradas: Descriptor de archivo del pipe de un worker y el contenido a mandar
// Salidas: void
// Descripción: Manda un mensaje a un worker
void mandarMensaje(int fdHijo, char* contenido){
    write(fdHijo, contenido, buffer);
}

// Entradas: Descriptor de archivo del pipe de un worker y la dirección de memoria donde se guarda
// Salidas: void
// Descripción: Lee un mensaje de un worker
void leerMensaje(int fdHijo, char* buff){
    read(fdHijo, buff, buffer);
}