#include <stdlib.h>
#include "tablahash.h"
#include <assert.h>
static void* copiar_entero(void* i){
  void* x = malloc(sizeof(int));
  *(int*)x = *(int*)i;
  return x;
}

static int comp_entero(void* i, void*j){
  return (*(int*)i - *(int*)j);
}

static void destr_entero(void* i) { free(i); }

unsigned Hash(void* i){
  return *(int*)i;
}


int main(){
  TablaHash tabla = tablahash_crear(11, copiar_entero, comp_entero, destr_entero, Hash);
  assert(tablahash_nelems(tabla) == 0);

  int* x = malloc(sizeof(int));
  *x = 1;
  tablahash_insertar(tabla, x);
  assert(tablahash_nelems(tabla) == 1);
  
  *x = 12; //modulo da 1 ->colision
  tablahash_insertar(tabla, x);
  assert(tablahash_nelems(tabla) == 2);

  assert(tablahash_capacidad(tabla) == 22);

  assert(tablahash_buscar(tabla, x) != NULL);
  *x = 1;
  assert(tablahash_buscar(tabla, x) != NULL);

  tablahash_destruir(tabla);
  free(x);

  return 0;
}