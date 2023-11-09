 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
 
 typedef struct particulas{
    int posicion;
    double energia;
} particulas;
 
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

int main(){
    particulas entrada[36];
    int part = lecturaArchivo("test1_36.txt", entrada, 36);
    int i = 0;
    for(i; i<part ; i++){
        printf("particula %d = %d y %lf\n", i, entrada[i].posicion, entrada[i].energia);
    }
    
   // for(int i = 0; i < part ; i++ ){
    //    if (entrada[i].posicion == )
   // }
    
}

    

