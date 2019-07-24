#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionesAuxiliares.h"

#define MAX_CIUDADES 30
/*
  *Recibe dos punteros a la estructura AlmacenarCamino
  *Copia en los campos de la segunda estructura los datos de la primera
*/
void llenar_camino_final(AlmacenarCamino* camino, AlmacenarCamino* caminoFinal){
  for(int i = 0; i < camino->cantidadVisitados; i++){
    caminoFinal->listaDeVisitados[i][0] = camino->listaDeVisitados[i][0];
    caminoFinal->listaDeVisitados[i][1] = camino->listaDeVisitados[i][1];
    caminoFinal->listaDeVisitados[i][2] = camino->listaDeVisitados[i][2];
    caminoFinal->costoTotal = camino->costoTotal;
  }
}

/*
  *Recibe dos punteros a una estructura AlmacenarCamino, la cantidad de ciudades del diagrama, la ciudad actual del recorrido
  *la matriz de adyacencia y una matriz de enteros que indica que ciudades fueron visitadas
  *Revisa si encontramos un ciclo posible, caso contrario comienza a elegir las ciudades donde viajar y realizar un recorrido desde allí
*/
void resuelve_tsp(AlmacenarCamino* caminoParcial, AlmacenarCamino* caminoFinal, int cantidadCiudades, int ciudadActual,
int** matrizAdyacente, int* visitados){
  if(caminoParcial->cantidadVisitados == cantidadCiudades-1 && matrizAdyacente[ciudadActual][0] > 0 &&
     (caminoFinal->costoTotal == -1 || caminoFinal->costoTotal > (caminoParcial->costoTotal + matrizAdyacente[ciudadActual][0]))){
    caminoParcial->listaDeVisitados[caminoParcial->cantidadVisitados][0] = ciudadActual;
    caminoParcial->listaDeVisitados[caminoParcial->cantidadVisitados][1] = 0;
    caminoParcial->listaDeVisitados[caminoParcial->cantidadVisitados][2] = matrizAdyacente[ciudadActual][0];
    caminoParcial->costoTotal = caminoParcial->costoTotal + matrizAdyacente[ciudadActual][0];
    caminoParcial->cantidadVisitados++;
    llenar_camino_final(caminoParcial, caminoFinal);
    caminoParcial->costoTotal = caminoParcial->costoTotal - matrizAdyacente[ciudadActual][0];
    caminoParcial->cantidadVisitados--;
  }
  else{
    for(int ciudadDestino = 0; ciudadDestino<cantidadCiudades; ciudadDestino++){
      if((caminoFinal->costoTotal == -1 || caminoFinal->costoTotal > (caminoParcial->costoTotal + matrizAdyacente[ciudadActual][ciudadDestino])
    )&& (matrizAdyacente[ciudadActual][ciudadDestino] > 0) && !(visitados[ciudadDestino])){
        visitados[ciudadDestino] = 1;
        caminoParcial->listaDeVisitados[caminoParcial->cantidadVisitados][0] = ciudadActual;
        caminoParcial->listaDeVisitados[caminoParcial->cantidadVisitados][1] = ciudadDestino;
        caminoParcial->listaDeVisitados[caminoParcial->cantidadVisitados][2] = matrizAdyacente[ciudadActual][ciudadDestino];
        caminoParcial->costoTotal = caminoParcial->costoTotal + matrizAdyacente[ciudadActual][ciudadDestino];
        caminoParcial->cantidadVisitados++;
        resuelve_tsp(caminoParcial, caminoFinal, cantidadCiudades, ciudadDestino, matrizAdyacente, visitados);
        visitados[ciudadDestino] = 0;
        caminoParcial->cantidadVisitados--;
        caminoParcial->costoTotal = caminoParcial->costoTotal - matrizAdyacente[ciudadActual][ciudadDestino];

      }
    }
  }
}

int main(int argc, char* argv[]){
  if(argc < 3){
    printf("No se completaron los 3 argumentos necesarios en la ejecucion del programa");
    return 0;
  }
  char** ciudades = malloc(sizeof(char*)*MAX_CIUDADES); //TODO: Destruir
  for (int i = 0; i < MAX_CIUDADES; i++){
    ciudades[i] = malloc(sizeof(char)*50);
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL){//Reviso si existe ek archivo de entrada
    printf("El archivo de entrada ingresado no existe\n");
    return 0;
  }
  int cantidadCiudades = obtenerCiudades(file, ciudades);
  if (cantidadCiudades < 2){//Reviso que sea un caso valido
    printf("La cantidad de ciudades no es suficiente para generar un camino\n");
    return 0;
  }
  else{
    int** matrizAdyacente = obtenerCostos(file, ciudades, cantidadCiudades); //Creo la matriz adyacente
    fclose(file);
    //Comienzo a crear los caminos
    AlmacenarCamino* camino = malloc(sizeof(AlmacenarCamino));
    camino->listaDeVisitados = malloc(sizeof(int*)*cantidadCiudades);
    for (int i = 0; i < cantidadCiudades; i++){
      camino->listaDeVisitados[i] = malloc(sizeof(int)*3);
    }
    camino->cantidadVisitados = 0;
    camino->costoTotal = 0;
    AlmacenarCamino* caminofinal = malloc(sizeof(AlmacenarCamino));
    caminofinal->listaDeVisitados = malloc(sizeof(int*)*cantidadCiudades);
    for (int i = 0; i < cantidadCiudades; i++){
      caminofinal->listaDeVisitados[i] = malloc(sizeof(int)*3);
    }
    caminofinal->costoTotal = -1;
    //Creo un registro de los visitados
    int* visitados = malloc(sizeof(int)*cantidadCiudades);
    for (int i = 0; i < cantidadCiudades; i++){
      visitados[i] = 0;
    }
    visitados[0] = 1;
    //Resuelvo el problema
    resuelve_tsp(camino, caminofinal, cantidadCiudades, 0, matrizAdyacente,visitados);
    file = fopen(argv[2], "w+");
    for(int i = 0; i < cantidadCiudades; i++){
      fprintf(file,"%s,%s,%d\n" ,ciudades[caminofinal->listaDeVisitados[i][0]],ciudades[caminofinal->listaDeVisitados[i][1]] , caminofinal->listaDeVisitados[i][2]);
    }
    fclose(file);
    free(visitados);
    destruir_camino(camino, cantidadCiudades);
    destruir_camino(caminofinal, cantidadCiudades);
    destruir_matriz(matrizAdyacente, cantidadCiudades);
    destruir_ciudades(ciudades, MAX_CIUDADES);
    return 0;
  }
}
