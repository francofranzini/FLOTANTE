#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
  AVL rebalse;
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
  FuncionHash hash;
};

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

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
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].dato = NULL;
    tabla->elems[idx].rebalse = NULL;
  }

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
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
    if (tabla->elems[idx].dato != NULL)
      tabla->destr(tabla->elems[idx].dato);
    if(tabla->elems[idx].rebalse != NULL)
      avl_destruir(tabla->elems[idx].rebalse);
  }
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

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].dato == NULL) {
    tabla->numElems++;
    tabla->elems[idx].dato = tabla->copia(dato);
    tabla->elems[idx].rebalse = avl_crear(tabla->copia, tabla->comp, tabla->destr);
    return;
  }
  // Si el dato se encuentra en la tabla, lo reemplaza
  else if (tabla->comp(tabla->elems[idx].dato, dato) == 0) {
    tabla->destr(tabla->elems[idx].dato);
    tabla->elems[idx].dato = tabla->copia(dato);
    return;
  }
  // si hay colision, se agrega al arbol de rebalse
  else {
    if(avl_buscar(tabla->elems[idx].rebalse, dato) == 1){
      avl_eliminar(tabla->elems[idx].rebalse, dato);
      avl_insertar(tabla->elems[idx].rebalse, dato);
    }
    else{
      avl_insertar(tabla->elems[idx].rebalse, dato);
      tabla->numElems++;
      if(avl_nnodos(tabla->elems[idx].rebalse) > tabla->numElems * 0.15){
        printf("AVL_NNODOS: %i\n", avl_nnodos(tabla->elems[idx].rebalse));
        tablahash_redimensionar(tabla);
      }
    }
    return;
  }
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx].dato == NULL)
    return NULL;
  // Retornar el dato de la casilla si hay concidencia.
  else if (tabla->comp(tabla->elems[idx].dato, dato) == 0)
    return tabla->elems[idx].dato;
  // Retornar NULL en otro caso.
  else
    return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar si la casilla estaba vacia.
  if (tabla->elems[idx].dato == NULL)
    return;
  // Vaciar la casilla si hay coincidencia.
  else if (tabla->comp(tabla->elems[idx].dato, dato) == 0) {
    tabla->numElems--;
    tabla->destr(tabla->elems[idx].dato);
    if(avl_nnodos(tabla->elems[idx].rebalse) > 0){
      void* dato_raiz = avl_raiz(tabla->elems[idx].rebalse);
      tabla->elems[idx].dato = tabla->copia(dato_raiz);
      avl_eliminar(tabla->elems[idx].rebalse, dato_raiz);
    }
    else{
      tabla->elems[idx].dato = NULL;
      avl_destruir(tabla->elems[idx].rebalse);
    }
    return;
  }
  else{
    if(avl_buscar(tabla->elems[idx].rebalse, dato) == 1){
      avl_eliminar(tabla->elems[idx].rebalse, dato);
      tabla->numElems--;
    }
  }
}

void tablahash_redimensionar(TablaHash tabla){
  TablaHash nueva_tabla = tablahash_crear(
    tabla->capacidad*2,
    tabla->copia,
    tabla->comp,
    tabla->destr,
    tabla->hash
  );
  for(int i = 0; i<tabla->capacidad; i++){
    while(tabla->elems[i].dato != NULL){
      tablahash_insertar(nueva_tabla, tabla->elems[i].dato);
      tablahash_eliminar(tabla, tabla->elems[i].dato);
    }  
  }
  free(tabla->elems);
  tabla->capacidad = nueva_tabla->capacidad;
  tabla->numElems = nueva_tabla->numElems;
  tabla->elems = nueva_tabla->elems;
  free(nueva_tabla);
}
