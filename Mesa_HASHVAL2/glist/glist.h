#ifndef __GLIST_H__
#define __GLIST_H__

typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparar)(void *dato1, void *dato2);
typedef int (*Predicado) (void *dato);

typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef struct _GNode *GList;

/**
 * Devuelve una lista vacía.
 */
GList glist_crear();

/**
 * Destruccion de la lista.
 */
void glist_destruir(GList list, FuncionDestructora);

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList list);

/**
 * Agrega un elemento al inicio de la lista.
 */
GList glist_agregar_inicio(GList list, void *data, FuncionCopia);

/**
 * Busca un elemento en la lista.
 */
int glist_buscar(GList list, void* data, FuncionComparar);

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante);


#endif /* __GLIST_H__ */
