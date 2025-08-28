#include "bstset.h"
#include <stdlib.h>
#include <stdio.h>

BSTSet bstset_crear(){
  return NULL;
}

BSTSet bstset_insertar(BSTSet raiz,int dato){
  if(raiz == NULL){
    bstNodo* nuevo_nodo = malloc(sizeof(bstNodo));
    nuevo_nodo->dato = dato;
    nuevo_nodo->der = NULL;
    nuevo_nodo->izq = NULL;
    return nuevo_nodo;

  }
  if(raiz->dato < dato) raiz->der = bstset_insertar(raiz->der, dato);
  if(raiz->dato > dato) raiz->izq = bstset_insertar(raiz->izq, dato);
  return raiz;
}
void imprimeBST(BSTSet raiz){
  if(raiz == NULL) return;
  if(raiz->izq) imprimeBST(raiz->izq);
  printf("%i ", raiz->dato);
  if(raiz->der) imprimeBST(raiz->der);
}


int buscar(BSTSet raiz, int dato){
  if(raiz == NULL ) return 0;
  if(raiz->dato < dato) return buscar(raiz->der, dato);
  if(raiz->dato > dato) return buscar(raiz->izq, dato);
  return 1;
}


void bstset_interseccion_aux(BSTSet raiz1, BSTSet raiz2, BSTSet* interseccion){
  if(raiz1 == NULL) return;
  if(buscar(raiz2, raiz1->dato))
    *interseccion = bstset_insertar(*interseccion, raiz1->dato);
  bstset_interseccion_aux(raiz1->der, raiz2, interseccion);
  bstset_interseccion_aux(raiz1->izq, raiz2, interseccion);
}

BSTSet bstset_interseccion(BSTSet raiz1, BSTSet raiz2){
  if(raiz1 == NULL) return NULL;
  BSTSet interseccion = bstset_crear();
  bstset_interseccion_aux(raiz1, raiz2, &interseccion);
  return interseccion;
}



int bstset_mayor(BSTSet raiz){
  if(raiz->der == NULL) return raiz->dato;
  else return bstset_mayor(raiz->der);
}

int bstset_menor(BSTSet raiz){
  if(raiz->izq == NULL) return raiz->dato;
  else return bstset_menor(raiz->izq);
}

int lejanoBST(BSTSet raiz, int dato){
  int x = bstset_menor(raiz);
  int y = bstset_mayor(raiz);
  return (abs(x - dato) > abs(y-dato) ? x : y);  
}

int main(){

  BSTSet arbol = bstset_crear();
  arbol = bstset_insertar(arbol, 4);
  arbol = bstset_insertar(arbol, 4);
  arbol = bstset_insertar(arbol, 2);
  arbol = bstset_insertar(arbol, 1);
  arbol = bstset_insertar(arbol, 8);

  BSTSet arbol2 = bstset_crear();
  arbol2 = bstset_insertar(arbol2, 1);
  arbol2 = bstset_insertar(arbol2, 3);
  arbol2 = bstset_insertar(arbol2, 4);
  arbol2 = bstset_insertar(arbol2, 5);
  arbol2 = bstset_insertar(arbol2, 6);


  BSTSet arbol_1y2 = bstset_crear();

  arbol_1y2 = bstset_interseccion(arbol, arbol2);

  imprimeBST(arbol_1y2);

  return 0;
}