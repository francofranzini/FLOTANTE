#ifndef __ARBOL_H__
#define __ARBOL_H__

typedef  enum  color {Rojo , Azul , Neutro} Color;

typedef  struct  Nodo_ {
  char* cdato;
  Color  colorNodo;
  struct  Nodo_ *der , *izq;
} Nodo;

typedef  Nodo* ArbolBinarioColoreado;

// Retorna  el  color  del  Arbol.
Color  color(ArbolBinarioColoreado  inicio );

// Retorna  la  cantidad  de  hojas  puras.
int  CantidadPuras(ArbolBinarioColoreado  inicio );


#endif