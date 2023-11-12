#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//Abstraccion de una particula
typedef struct particulas{
    int posicion;
    double energia;
} particulas;

void calcularEnerg(particulas * entrada, particulas * salida, int N, int chunk);

