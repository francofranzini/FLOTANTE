#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>

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
    tabla->elems[idx].eliminado = 0;
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
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].dato != NULL)
      tabla->destr(tabla->elems[idx].dato);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}


int tablahash_buscar_casilla(TablaHash tabla, int inicio, void* dato){
  int idx = inicio;
  while(1){
    //el dato no esta en la tabla
    if(tabla->elems[idx].dato == NULL && tabla->elems[idx].eliminado == 0)
      return idx;
    if(tabla->elems[idx].dato != NULL && tabla->comp(tabla->elems[idx].dato, dato) == 0)
      return idx;

    idx = (idx+1)%tabla->capacidad;

    if(idx == inicio)
      return -1;
  }
}



/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {

  if((float)tabla->numElems > (float)tabla->capacidad*0.75)
    tablahash_redimensionar(tabla);
  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].dato == NULL) {
    tabla->numElems++;
    tabla->elems[idx].dato = tabla->copia(dato);
    tabla->elems[idx].eliminado = 0;
    return;
  }
  // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
  else if (tabla->comp(tabla->elems[idx].dato, dato) == 0) {
    tabla->destr(tabla->elems[idx].dato);
    tabla->elems[idx].dato = tabla->copia(dato);
    return;
  }
  else {
    while(tabla->elems[idx].dato != NULL)
      idx = (idx + 1)%tabla->capacidad;
    tabla->elems[idx].dato = tabla->copia(dato);
    tabla->numElems++;
    tabla->elems[idx].eliminado = 0;
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
  unsigned inicio = idx;
  idx = tablahash_buscar_casilla(tabla, inicio, dato);
  if(idx == -1)
    return NULL;
  else return tabla->elems[idx].dato;

}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  unsigned inicio = idx;
  //Recorremos la tabla en busca del dato, pero no paramos al encontrar NULL porque puede ser el resultado
  //de una eliminacion, corriendo el riesgo de entrar en bucle. Para evitar esto, marcamos donde comenzamos
  //en caso de "dar la vuelta"

  idx = tablahash_buscar_casilla(tabla, inicio, dato);
  if(idx == -1)
    return;
  else{
    tabla->destr(tabla->elems[idx].dato);
    tabla->elems[idx].dato = NULL;
    tabla->elems[idx].eliminado = 1;
    tabla->numElems--;
    return;
  }
}

void* copia_dummy(void* dato){
  return dato;
}
void destr_dummy(void* dato){
  (void*)dato;
}
void tablahash_redimensionar(TablaHash tabla){
  TablaHash nueva_tabla = tablahash_crear(
    tabla->capacidad*2,
    copia_dummy,
    tabla->comp,
    tabla->destr,
    tabla->hash
  );
  tabla->destr = destr_dummy;
  for(int i = 0; i<tabla->capacidad; i++){
    if(tabla->elems[i].dato != NULL){
      tablahash_insertar(nueva_tabla, tabla->elems[i].dato);
      tablahash_eliminar(tabla, tabla->elems[i].dato);
    }
  }
  free(tabla->elems);
  tabla->capacidad = nueva_tabla->capacidad;
  tabla->elems = nueva_tabla->elems;
  tabla->destr = nueva_tabla->destr;
  tabla->numElems = nueva_tabla->numElems;
  free(nueva_tabla);

}
