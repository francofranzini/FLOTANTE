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
  int rs;
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
    //Lineas que hay que agregar 
    tabla->elems[idx].rebalse = avl_crear(tabla->copia, tabla->comp, tabla->destr);
    tabla->elems[idx].rs = 0;
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
    return;
  }
  // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
  else if (tabla->comp(tabla->elems[idx].dato, dato) == 0) {
    tabla->destr(tabla->elems[idx].dato);
    tabla->elems[idx].dato = tabla->copia(dato);
    return;
  }
  // ir al arbol de rebalse
  else {
    //Si esta en el arbol de rebalse, eliminamos y agregamos nuevamente
    if(avl_buscar(tabla->elems[idx].rebalse, dato) == 1){
      avl_eliminar(tabla->elems[idx].rebalse, dato);
      avl_insertar(tabla->elems[idx].rebalse, dato);
    }
    else{
      avl_insertar(tabla->elems[idx].rebalse, dato);
      tabla->numElems++;
      tabla->elems[idx].rs++;
    }
    if(tabla->elems[idx].rs > tabla->numElems * 0.15){
      
      tablahash_redimensionar(tabla);
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
    //Eliminacion del dato en casilla
    tabla->numElems--;
    tabla->destr(tabla->elems[idx].dato);
    //Reocupar la casilla con el rebalse
    if(tabla->elems[idx].rs == 0)
      //No hay rebalse para ocupar la casilla
      tabla->elems[idx].dato = NULL;
    else{
      //Asignamos la raiz a la casilla
      void* dato_raiz = avl_raiz(tabla->elems[idx].rebalse);
      tabla->elems[idx].dato = tabla->copia(dato_raiz);
      //Eliminamos el dato del rebalse
      avl_eliminar(tabla->elems[idx].rebalse, dato_raiz);
      tabla->elems[idx].rs--;
    }
    return;
  }
  else if(avl_buscar(tabla->elems[idx].rebalse, dato) == 1){
    //Eliminacion de dato en rebalse
    tabla->numElems--;
    avl_eliminar(tabla->elems[idx].rebalse, dato);
    tabla->elems[idx].rs--;
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
    //Aqui va un while ya que eliminar reocupa la casilla a la vez que vacia el rebalse
    while(tabla->elems[i].dato != NULL){
      tablahash_insertar(nueva_tabla, tabla->elems[i].dato);
      tablahash_eliminar(tabla, tabla->elems[i].dato);
    }
    avl_destruir(tabla->elems[i].rebalse);
  }
  free(tabla->elems);
  tabla->capacidad = nueva_tabla->capacidad;
  tabla->elems = nueva_tabla->elems;
  tabla->numElems = nueva_tabla->numElems;
  free(nueva_tabla);
}
