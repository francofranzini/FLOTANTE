#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
  int eliminado;

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

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].dato == NULL) {
    tabla->numElems++;
    tabla->elems[idx].dato = tabla->copia(dato);
    tabla->elems[idx].eliminado = 0;
  }
  else {
    while(tabla->elems[idx].dato != NULL){
      if(tabla->comp(tabla->elems[idx].dato, dato) == 0){
        tabla->destr(tabla->elems[idx].dato);
        tabla->elems[idx].dato = tabla->copia(dato);
        return;
      }
      else
        idx = (idx+1)%tabla->capacidad;
    } 
    //llegamos a tabla->elems[idx] == NULL;
    tabla->numElems++;
    tabla->elems[idx].dato = tabla->copia(dato);
    tabla->elems[idx].eliminado = 0;
  }
  

  if((float)tabla->numElems > (float)tabla->capacidad*0.75) tablahash_redimensionar(tabla);
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void* tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx].dato == NULL)
    return NULL;
  // Retornar el dato de la casilla si hay concidencia.
  else {

    int inicio = idx;
    /*
      Hacemos una iteracion por fuera del bucle para probar la situacion inicial
      y de manera segura seguir en el bucle evitando bucle infinito
    */
    if(tabla->elems[idx].dato != NULL && tabla->comp(tabla->elems[idx].dato, dato) == 0)
      return tabla->elems[idx].dato;
    if(tabla->elems[idx].dato == NULL && tabla->elems[idx].eliminado == 0)
        return NULL; //Encontre una casilla que no encadena

    idx++;

    while(idx != inicio){
      if(tabla->elems[idx].dato != NULL && tabla->comp(tabla->elems[idx].dato, dato) == 0)
        return tabla->elems[idx].dato;
      if(tabla->elems[idx].dato == NULL && tabla->elems[idx].eliminado == 0)
        return NULL; //Encontre una casilla que no encadena

      idx = (idx + 1) % tabla->capacidad;
    }
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
  if (tabla->elems[idx].dato == NULL && tabla->elems[idx].eliminado == 0)
    return;
  // Vaciar la casilla si hay coincidencia.
  else {
    // 
    int inicio = idx;
    if(tabla->elems[idx].dato != NULL && tabla->comp(tabla->elems[idx].dato, dato) == 0){
      tabla->destr(tabla->elems[idx].dato);
      tabla->elems[idx].dato = NULL;
      tabla->elems[idx].eliminado = 1;
      tabla->numElems--;
      return;
    }
    if(tabla->elems[idx].dato == NULL && tabla->elems[idx].eliminado == 0) 
      return;
    idx = (idx + 1)%tabla->capacidad;

    while(idx != inicio){
      if(tabla->elems[idx].dato != NULL && tabla->comp(tabla->elems[idx].dato, dato) == 0){
        tabla->destr(tabla->elems[idx].dato);
        tabla->elems[idx].dato = NULL;
        tabla->elems[idx].eliminado = 1;
        tabla->numElems--;
        return;
      }
      if(tabla->elems[idx].dato == NULL && tabla->elems[idx].eliminado == 0) 
        return;
      idx = (idx+1)%tabla->capacidad;
    }
  }
  
}

void tablahash_redimensionar(TablaHash tabla){
  TablaHash nt = tablahash_crear(
    tabla->capacidad*2,
    tabla->copia,
    tabla->comp,
    tabla->destr,
    tabla->hash
  );
  for(int i = 0; i<tabla->capacidad; i++){
    if(tabla->elems[i].dato != NULL){
      tablahash_insertar(nt, tabla->elems[i].dato);
      // tabla->destr(tabla->elems[i].dato);
      tablahash_eliminar(tabla, tabla->elems[i].dato);
    }
  }
  free(tabla->elems);
  tabla->elems = nt->elems;
  tabla->numElems = nt->numElems;
  tabla->capacidad = nt->capacidad;
  free(nt);
}
