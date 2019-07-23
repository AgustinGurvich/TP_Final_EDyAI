#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  *AlmacenarCamino es una estructura que consta de 3 partes:
    1_ int** listaDeVisitados es una matriz de n submatrices de 3 elementos, donde n es la cantidad de ciudades del archivo. Dicha
      submatriz tiene 3 casillas donde identificamos a la primera como el origen del viaje, la segunda como el destino y la tercera
      como el costo de viajar entre dichas ciudades.
    2_int cantidadVisitados es la cantidad de ciudades visitadas actualmente
    3_int costoTotal es la suma de los costos de los viajes almacenados en listaDeVisitados    
*/

typedef struct _AlmacenarCamino{
  int** listaDeVisitados;
  int cantidadVisitados;
  int costoTotal;
} AlmacenarCamino;

int obtenerCiudades(FILE* file, char** ciudades);

int** obtenerCostos(FILE* file, char** ciudades, int cantidadCiudades);

int posicion(char** ciudades, char* etiqueta, int cantidadCiudades);

void destruir_matriz(int** matriz, int largo);

void destruir_camino(AlmacenarCamino* camino);
