#include "arbol.h"
#include <stddef.h>


void color_aux(ArbolBinarioColoreado arbol, int* rojo, int* azul){
  if(arbol == NULL) return;
  if(arbol->colorNodo == Azul) (*azul)++;
  if(arbol->colorNodo == Rojo) (*rojo)++;
  color_aux(arbol->der, rojo, azul);
  color_aux(arbol->izq, rojo, azul);
}

Color color(ArbolBinarioColoreado arbol){
  int azul = 0;
  int rojo = 0;
  color_aux(arbol, &rojo, &azul);
  if(rojo == azul) return Neutro;
  if(rojo > azul) return Rojo;
  else return Azul;
}


int CantidadPuras_aux(ArbolBinarioColoreado inicio, Color linaje){
  if(inicio == NULL) return 0;
  //Al encontrar un nodo que rompa el linaje, descartamos toda su descendencia
  if(inicio->colorNodo != linaje) return 0;
  if(inicio->izq == NULL && inicio->der == NULL) return 1;
  
  return  CantidadPuras_aux(inicio->izq, linaje) + 
          CantidadPuras_aux(inicio->der, linaje);
}
int  CantidadPuras(ArbolBinarioColoreado  inicio ){
  if(inicio == NULL) return 0;
  Color linaje = inicio->colorNodo;
  //Todas las hojas deben ser del mismo linaje, dado por
  //la raiz del arbol
  return CantidadPuras_aux(inicio, linaje);
}