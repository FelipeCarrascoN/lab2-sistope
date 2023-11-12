#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct particulas{
    int posicion;
    double energia;
} particulas;

int lecturaArchivo (char* nombre, particulas entrada[], int N);
int indice(int posicion, particulas* entrada, int part);
void calcularEnerg(particulas * entrada, int N, particulas ** salida, int part);
int maximo(particulas * entrada, int N);
int escribirArchivo(int N, particulas * salida, char * nombre);
void imprimirGrafico(int N, particulas * salida);