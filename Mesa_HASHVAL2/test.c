#include <stdio.h>
#include <stdlib.h>
#include "tablahash/tablahash.h"
#include <assert.h>

int comp(void* x, void* y){
  return (*(int*)x - *(int*)y);
}
void destr(void* x){
  free(x);
}
void* copia(void* x){
  int* y = malloc(sizeof(int));
  *y = *(int*)x;
  return y;
}
unsigned hash(void* x){
  return abs(*(int*)x);
}
int main(){ 

  TablaHash tabla = tablahash_crear(11, copia, comp, destr, hash);

  int* x = malloc(sizeof(int));
  *x = 16;

  tablahash_insertar(tabla, x);
  assert(tablahash_nelems(tabla) == 1);

  *x = 5;
  tablahash_insertar(tabla, x);
  assert(tablahash_nelems(tabla) == 2);
  assert(tablahash_capacidad(tabla) == 22);

  assert(*(int*)(tablahash_buscar(tabla, x)) == 5);
  *x = 16;
  assert(*(int*)(tablahash_buscar(tabla, x)) == 16);

  *x = 8;
  assert(tablahash_buscar(tabla, x) == NULL);


  tablahash_destruir(tabla);

  free(x);

  return 0;
}