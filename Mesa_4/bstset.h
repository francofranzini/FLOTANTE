#ifndef __BSTSET_H__
#define __BSTSET_H__

typedef  struct  bstNodo_ {
  int  dato;
  struct  bstNodo_ *der , *izq;
}bstNodo;

typedef  bstNodo* BSTSet;

void bstset_interseccion_aux(BSTSet raiz1, BSTSet raiz2, BSTSet* interseccion);
BSTSet bstset_interseccion(BSTSet raiz1, BSTSet raiz2);

#endif