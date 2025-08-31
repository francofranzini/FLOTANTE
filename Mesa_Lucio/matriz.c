#include "matriz.h"
#include <stdlib.h>
#include <stdio.h>


Matriz matriz_crear(int n){
  Matriz nueva_matriz = malloc(sizeof(Matriz));
  nueva_matriz->datos = malloc(sizeof(SList)*n);
  nueva_matriz->size = n;
  for(int i = 0; i<n; i++){
    nueva_matriz->datos[i] = slist_crear();
  }
  return nueva_matriz;
}

void matriz_insertar(Matriz m, int i, int j){
  //(i, j) representa que i se relaciona con j
  // i --> j. Cada m[i] guarda con quienes se relaciona

  m->datos[i] = slist_agregar_final(m->datos[i], j);
}

void matriz_eliminar(Matriz m, int i, int j){
  m->datos[i] = slist_eliminar(m->datos[i], j);
}

int es_simetrica(Matriz m){
  //Por cada relacion (i, j), buscamos (j, i)
  // es decir, que la lista m->datos[j] contenga i
  int bandera = 1;
  for(int i = 0; bandera && i<m->size; i++){
    SList act = m->datos[i];
    while(bandera && act != NULL){
      int target = act->dato;
      if(!slist_buscar(m->datos[target], i))
        bandera = 0;
      act = act->sig;
    }
  }
  return bandera;
}