#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionesAuxiliares.h"

/*
  *Recibe una matriz bidimensional con los nombres de las ciudades, la ciudad a buscar y el maximo de ciudades
  *Devuelve el indice correspondiente a dicha etiqueta en la matriz
*/

int posicion(char** ciudades, char* etiqueta, int cantidadCiudades){
  int i, continuar = 1;
  for (i = 0; i < cantidadCiudades && continuar; i++){
    if (!(strcmp(etiqueta, *(ciudades+i)))){
      continuar = 0;
    }
  }
  return i-1;
}

/*
  *Recibe un archivo y una matriz bidimensional de caracteres
  *Almacena los nombres escritos en la segunda linea del archivo y devuelve la cantidad de valores leidos
*/

int obtenerCiudades(FILE* file, char** ciudades){
  char buff[50];
  fscanf(file, "%s", buff);
  char buffer = fgetc(file);
  int i = 0, cantidadCiudades = 0;
  buffer = fgetc(file);
  while(buffer != '\n'){
    if(buffer != ','){
      buff[i] = buffer;
      i++;
      buffer = fgetc(file);
    }
    else{
      buff[i] = '\0';
      strcpy(*(ciudades+cantidadCiudades), buff);
      cantidadCiudades++;
      i=0;
      buffer = fgetc(file);
      buffer = fgetc(file);
  }
}
buff[i] = '\0';
strcpy(*(ciudades+cantidadCiudades), buff);
cantidadCiudades++;
  return cantidadCiudades;
}

/*
 *Recibe un archivo, una matriz bidimensional con las ciudades y la cantidad de ciudades
 *Crea una matriz de adyacencia con los costos del archivo y la devuelve
*/
int** obtenerCostos(FILE* file, char** ciudades, int cantidadCiudades){
  char buff[50];
  int** matrizAdyacente = malloc(sizeof(int*)*cantidadCiudades);
  for(int i = 0; i < cantidadCiudades; i++){
    matrizAdyacente[i] = malloc(sizeof(int)*cantidadCiudades);
  }
  fscanf(file, "%s", buff);
  char buffer = fgetc(file);//Agarro el \n
  for(int i = 0; i < cantidadCiudades; i++){
    for(int j = 0; j < cantidadCiudades; j++){
      matrizAdyacente[i][j] = 0;
    }
  }
  buffer = fgetc(file);
  int i = 0, origen, destino;
  while(!feof(file)){
    while(buffer!=','){
      buff[i] = buffer;
      i++;
      buffer = fgetc(file);
    }
    buff[i] = '\0';
    origen = posicion(ciudades, buff, cantidadCiudades);
    i = 0;
    buffer = fgetc(file);
    while(buffer!=','){
      buff[i] = buffer;
      i++;
      buffer = fgetc(file);
    }
    buff[i] = '\0';
    destino = posicion(ciudades, buff, cantidadCiudades);
    buffer = fgetc(file);
    i = 0;
    while(buffer != '\n' && !feof(file)){
      buff[i] = buffer;
      i++;
      buffer = fgetc(file);
    }
    buff[i] = '\0';
    i=0;
    buffer = fgetc(file);
    matrizAdyacente[origen][destino] = atoi(buff);
    matrizAdyacente[destino][origen] = atoi(buff);
  }
  return matrizAdyacente;
}

/*
  *Recibe un puntero a una matriz cuadrada bidimensional de enteros y su tamaño
  *Elimina su contenido y el puntero
*/
void destruir_matriz(int** matriz, int largo){
  for(int i = 0; i < largo; i++){
    free (matriz[i]);
  }
  free(matriz);
}

/*
  *Recibe un puntero a una estructura AlmacenarCamino
  *Elimina su contenido y el puntero
*/
void destruir_camino(AlmacenarCamino* camino){
  free(camino->listaDeVisitados[0]);
  free(camino->listaDeVisitados[1]);
  free(camino->listaDeVisitados[2]);
  free(camino);
}

int main(){
  return 0;
}
