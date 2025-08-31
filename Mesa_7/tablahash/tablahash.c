#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash1, hash2;
  int cantidades[27];
};

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash1, FuncionHash hash2) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash1 = hash1;
  tabla->hash2 = hash2;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].dato = NULL;
  }
  for(int i = 0; i<27; i++)
    tabla->cantidades[i] = 0;

  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].dato != NULL)
      tabla->destr(tabla->elems[idx].dato);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {


  unsigned idx1 = tabla->hash1(dato) % tabla->capacidad;
  unsigned idx2 =  1 + (tabla->hash2(dato)% (tabla->capacidad - 1));
  int j = 0;

  while(j < tabla->capacidad){
    idx1 = (idx1 + idx2 * j) % tabla->capacidad;
    if(tabla->elems[idx1].dato == NULL){
      tabla->elems[idx1].dato = tabla->copia(dato);
      tabla->numElems++;
      char c = *((char*)dato);
      if(c >= 'a' && c <= 'z')
        (tabla->cantidades[c - 'a'])++;
      else
        (tabla->cantidades[c - 'A'])++;
      return;
    }
    j++;
  }
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  unsigned idx1 = tabla->hash1(dato) % tabla->capacidad;
  unsigned idx2 = 1 + (tabla->hash2(dato)% (tabla->capacidad - 1));
  int j = 0;

  while(j < tabla->capacidad){
    idx1 = (idx1 + idx2 * j) % tabla->capacidad;
    if(tabla->comp(tabla->elems[idx1].dato, dato)){
      return tabla->elems[idx1].dato;
    }
    j++;
  }
  return NULL;//No se encuentra el dato
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  unsigned idx1 = tabla->hash1(dato) % tabla->capacidad;
  unsigned idx2 = 1 + (tabla->hash2(dato)% (tabla->capacidad - 1));
  int j = 0;

  while(j < tabla->capacidad){
    idx1 = (idx1 + idx2 * j) % tabla->capacidad;
    if(tabla->comp(tabla->elems[idx1].dato,dato)){
      tabla->destr(tabla->elems[idx1].dato);
      tabla->numElems--;
      char c = *((char*)dato);
      if(c >= 'a' && c <= 'z')
        (tabla->cantidades[c - 'a'])--;
      else
        (tabla->cantidades[c - 'A'])--;
      return;
    }
    j++;
  }
}
