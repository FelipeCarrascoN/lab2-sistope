
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "fworker.h"

//worker

void calcularEnerg(particulas entrada[], particulas salida[], int N, int chunk){
    double Ei;
    double Ej_i, Ej;
    double MIN_ENERGIA = pow(10,-3) / N;

    for (int i = 0; i < chunk; i++){
        Ej_i = 0;
        for(int j = 0; j < N; j++){
            Ei = entrada[j].energia;

            Ej = (pow(10,3) * Ei) / (N * sqrt(abs(j - entrada[j].posicion) + 1));

            if(Ej < MIN_ENERGIA){
                Ej = 0;
            }

            Ej_i += Ej;
        }

        salida[i].energia = Ej_i;
    }
}
