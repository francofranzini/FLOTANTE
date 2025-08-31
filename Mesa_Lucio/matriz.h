#ifndef __MATRIZ_H__
#define __MATRIZ_H__

#include "slist/slist.h"

typedef struct _Matriz{
  SList* datos;
  int size;
} _Matriz;

typedef _Matriz* Matriz;

//Matriz es un arreglo dinamico de listas simplemente enlazadas
//donde para cada (i, j) em la matriz de adyacencia, agregamos el 
//numero j a la i-esima lista de Matriz;






#endif