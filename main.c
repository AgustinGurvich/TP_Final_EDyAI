#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionesAuxiliares.h"

void llenar_camino_final(AlmacenarCamino* camino, AlmacenarCamino* caminoFinal){
  for(int i = 0; i < camino->cantidadVisitados; i++){
    caminoFinal->listaDeVisitados[i][0] = camino->listaDeVisitados[i][0];
    caminoFinal->listaDeVisitados[i][1] = camino->listaDeVisitados[i][1];
    caminoFinal->listaDeVisitados[i][2] = camino->listaDeVisitados[i][2];
    caminoFinal->costoTotal = camino->costoTotal;
  }
}

void resuelve_tsp(AlmacenarCamino* camino_parcial, AlmacenarCamino* camino_final, int cantidadCiudades, int ciudadActual,
int** matrizAdyacente, int* visitados){
  if(camino_parcial->cantidadVisitados == cantidadCiudades-1 && matrizAdyacente[ciudadActual][0] > 0 &&
     (camino_final->costoTotal == -1 || camino_final->costoTotal > (camino_parcial->costoTotal + matrizAdyacente[ciudadActual][0]))){
    camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][0] = ciudadActual;
    camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][1] = 0;
    camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][2] = matrizAdyacente[ciudadActual][0];
    camino_parcial->costoTotal = camino_parcial->costoTotal + matrizAdyacente[ciudadActual][0];
    camino_parcial->cantidadVisitados++;
    llenar_camino_final(camino_parcial, camino_final);
    camino_parcial->costoTotal = camino_parcial->costoTotal - matrizAdyacente[ciudadActual][0];
    camino_parcial->cantidadVisitados--;
  }
  else{
    for(int ciudadDestino = 0; ciudadDestino<cantidadCiudades; ciudadDestino++){
      if((camino_final->costoTotal == -1 || camino_final->costoTotal > (camino_parcial->costoTotal + matrizAdyacente[ciudadActual][ciudadDestino])
    )&& (matrizAdyacente[ciudadActual][ciudadDestino] > 0) && !(visitados[ciudadDestino])){
        visitados[ciudadDestino] = 1;
        camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][0] = ciudadActual;
        camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][1] = ciudadDestino;
        camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][2] = matrizAdyacente[ciudadActual][ciudadDestino];
        camino_parcial->costoTotal = camino_parcial->costoTotal + matrizAdyacente[ciudadActual][ciudadDestino];
        camino_parcial->cantidadVisitados++;
        resuelve_tsp(camino_parcial, camino_final, cantidadCiudades, ciudadDestino, matrizAdyacente, visitados);
        visitados[ciudadDestino] = 0;
        camino_parcial->cantidadVisitados--;
        camino_parcial->costoTotal = camino_parcial->costoTotal - matrizAdyacente[ciudadActual][ciudadDestino];

      }
    }
  }
}

int main(int argc, char* argv[]){
  if(argc < 3){
    printf("No se completaron los 3 argumentos necesarios en la ejecucion del programa");
    return 0;
  }
  char** ciudades = malloc(sizeof(char*)*50); //TODO: Destruir
  for (int i = 0; i < 50; i++){
    *(ciudades+i) = malloc(sizeof(char)*50);
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL){//Reviso si existe ek archivo de entrada
    printf("Che capo poneme un archivo valido\n");
    return 0;
  }
  int cantidadCiudades = obtenerCiudades(file, ciudades);
  if (cantidadCiudades == 1){//Reviso que sea un caso valido
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
    int* visitados = malloc(sizeof(int)*50);
    for (int i = 0; i < cantidadCiudades; i++){
      visitados[i] = 0;
    }
    visitados[0] = 1;
    //Resuelvo el problema
    resuelve_tsp(camino, caminofinal, cantidadCiudades, 0, matrizAdyacente,visitados);
    FILE* file = fopen(argv[2], "w+");
    for(int i = 0; i < cantidadCiudades; i++){
      fprintf(file,"%s,%s,%d\n" ,ciudades[caminofinal->listaDeVisitados[i][0]],ciudades[caminofinal->listaDeVisitados[i][1]] , caminofinal->listaDeVisitados[i][2]);
    }
    //TODO: escribir en el archivo de salida
    destruir_camino(camino);
    destruir_camino(caminofinal);
    destruir_matriz(matrizAdyacente, cantidadCiudades);
    return 0;
  }
}
