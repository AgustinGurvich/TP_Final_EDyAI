#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _AlmacenarCamino{
  int** listaDeVisitados;
  int cantidadVisitados;
  int costoTotal;
} AlmacenarCamino;

int posicion(char** ciudades, char* etiqueta, int cantidadCiudades){
  int i, continuar = 1;
  for (i = 0; i < cantidadCiudades && continuar; i++){
    if (!(strcmp(etiqueta, *(ciudades+i)))){
      continuar = 0;
    }
  }
  return i-1;
}

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
 *Recibe un archivo, una matriz con las ciudades, una matriz de adyacencia y la cantidad de ciudades
 * y llena la matriz de adyacencia con los costos del archivo
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
  int i = 0, origen, destino, costo;
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

void llenar_camino_final(AlmacenarCamino* camino, AlmacenarCamino* caminoFinal){
  char cosa[50];
  for(int i = 0; i < camino->cantidadVisitados; i++){
    caminoFinal->listaDeVisitados[i][0] = camino->listaDeVisitados[i][0];
    caminoFinal->listaDeVisitados[i][1] = camino->listaDeVisitados[i][1];
    caminoFinal->listaDeVisitados[i][2] = camino->listaDeVisitados[i][2];
    caminoFinal->costoTotal = camino->costoTotal;
  }
}

void resuelve_tsp(AlmacenarCamino* camino_parcial, AlmacenarCamino* camino_final, int cantidadCiudades, int ciudadActual,
int** matrizAdyacente, int* visitados){
  char cosa[50];
  if(camino_parcial->cantidadVisitados == cantidadCiudades-1 && matrizAdyacente[ciudadActual][0] > 0 &&
     (camino_final->costoTotal == -1 || camino_final->costoTotal > (camino_parcial->costoTotal + matrizAdyacente[ciudadActual][0]))){
    camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][0] = ciudadActual;
    camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][1] = 0;
    camino_parcial->listaDeVisitados[camino_parcial->cantidadVisitados][2] = camino_parcial->costoTotal + matrizAdyacente[ciudadActual][0];
    camino_parcial->cantidadVisitados++;
    llenar_camino_final(camino_parcial, camino_final);
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

int main(){
  char** ciudades = malloc(sizeof(char*)*50);
  for (int i = 0; i < 50; i++){
    *(ciudades+i) = malloc(sizeof(char)*50);
  }
  char* archivo = malloc(sizeof(char)*1000);
  printf(" Ingrese el nombre completo del archivo\n");
  scanf("%s", archivo);
  FILE* file = fopen(archivo, "r");
  int cantidadCiudades = obtenerCiudades(file, ciudades);
  int** matrizAdyacente = obtenerCostos(file, ciudades, cantidadCiudades);
  AlmacenarCamino* camino = malloc(sizeof(AlmacenarCamino));
  camino->listaDeVisitados = malloc(sizeof(int*)*cantidadCiudades);
  for (int i = 0; i < cantidadCiudades; i++){
    *(camino->listaDeVisitados + i) = malloc(sizeof(int)*3);
  }
  camino->cantidadVisitados = 0;
  camino->costoTotal = 0;
  AlmacenarCamino* caminofinal = malloc(sizeof(AlmacenarCamino));
  caminofinal->listaDeVisitados = malloc(sizeof(int*)*cantidadCiudades);
  for (int i = 0; i < cantidadCiudades; i++){
    *(caminofinal->listaDeVisitados + i) = malloc(sizeof(int)*3);
  }
  caminofinal->costoTotal = -1;
  int* visitados = malloc(sizeof(int)*50);
  for (int i = 0; i < cantidadCiudades; i++){
    visitados[i] = 0;
  }
  visitados[0] = 1;
  resuelve_tsp(camino, caminofinal, cantidadCiudades, 0, matrizAdyacente,visitados);
  int suma = 0;
  for(int i = 0; i < cantidadCiudades; i++){
    printf("%d %d %d\n" ,caminofinal->listaDeVisitados[i][0],caminofinal->listaDeVisitados[i][1] , caminofinal->listaDeVisitados[i][2]);
    suma+=caminofinal->listaDeVisitados[i][2];
  }
  printf("%d\n", suma);

  return 0;
}
