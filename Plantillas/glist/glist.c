#include "glist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Devuelve una lista vacía.
 */
GList glist_crear() { 
  return NULL; 
}

/**
 * Destruccion de la lista.
 */
void glist_destruir(GList list, FuncionDestructora destroy) {
  GNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;
    destroy(nodeToDelete->data); 
    free(nodeToDelete);
  }
}

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList list) { 
  return (list == NULL); 
}

/**
 * Agrega un elemento al inicio de la lista.
 */
GList glist_agregar_inicio(GList list, void *data, FuncionCopia copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

/**
 * Busca un elemento en la lista.
 */
int glist_buscar(GList list, void* data, FuncionComparar comp){
  int found = 0;
  for(GNode *node = list; found == 0 && node != NULL; node = node->next)
    if (comp(node->data, data) == 0) found = 1;
  return found;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}




