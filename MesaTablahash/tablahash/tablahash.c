#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
  GList rebalse;
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
    tabla->elems[idx].rebalse = glist_crear();
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
    glist_destruir(tabla->elems[idx].rebalse, tabla->destr);
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
  else {

    //Se fija si ya esta en el rebalse
    GList aux = tabla->elems[idx].rebalse, prev;
    int asignado = 0;
    while(asignado && aux != NULL){
      if(tabla->comp(aux->data, dato) == 0){
        tabla->destr(aux->data);
        aux->data = tabla->copia(dato);
        asignado = 1;
      }
      else{
        prev = aux;
        aux = aux->next;
      }
    }
    //Si el dato no se reemplazo en el rebalse
    if(!asignado){
      tabla->numElems++;
      tabla->elems[idx].rebalse = glist_agregar_inicio(tabla->elems[idx].rebalse, dato, tabla->copia);
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
  else{
    GList aux = tabla->elems[idx].rebalse;
    int encontrado = 0;
    while(!encontrado && aux != NULL){
      if(tabla->comp(aux->data, dato) == 0){
        dato = aux->data;
        encontrado = 1;
      }
      aux = aux->next;
    }
    if(encontrado)
      return dato;
    else
      return NULL;
  }
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
    // tabla->elems[idx].dato = ;
    if(tabla->elems[idx].rebalse != NULL){
      GList aux = tabla->elems[idx].rebalse;
      tabla->elems[idx].dato = aux->data;
      tabla->elems[idx].rebalse = aux->next;
      free(aux);
    }
    else{
      tabla->elems[idx].dato = NULL;
    }
    return;
  }
  else{
    tabla->elems[idx].rebalse = glist_eliminar(tabla->elems[idx].rebalse, tabla->destr, tabla->comp, dato);
  }
}

void* copia_dummy(void* x){
  return (void*)x;
}
void destr_dummy(void* x){
  (void*)x;
  return;
}

void tablahash_redimensionar(TablaHash tabla){
  TablaHash nt = tablahash_crear(
    tabla->capacidad*2,
    copia_dummy,
    tabla->comp,
    tabla->destr,
    tabla->hash
  );
  tabla->destr = destr_dummy;
  for(int i = 0; i<tabla->capacidad; i++){
    while(tabla->elems[i].dato != NULL){
      tablahash_insertar(nt, tabla->elems[i].dato);
      tablahash_eliminar(tabla, tabla->elems[i].dato);
    }
  }
  free(tabla->elems);
  tabla->elems = nt->elems;
  tabla->destr = nt->destr;
  tabla->numElems = nt->numElems;
  tabla->capacidad = nt->capacidad;
  free(nt);
}
