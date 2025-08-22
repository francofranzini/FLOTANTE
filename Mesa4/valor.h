#ifndef __VALOR_H__
#define __VALOR_H__
#include "./glist/glist.h"
typedef struct _Casilla{
  int valor;
  int cant_menores;
  _Valor* sig;
} Casilla;

typedef Casilla* Valores;





#endif