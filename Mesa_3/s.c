#include "s.h"


struct _Relacion{
  char* persona1;
  char* persona2;
}


typedef struct _S{
  Relacion* relaciones;
}_S;