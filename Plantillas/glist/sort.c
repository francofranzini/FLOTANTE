#include "glist.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


void dividir(GList lista, GList* a, GList* b){
  GList slow, fast;
  slow = lista;
  fast = lista->next;
  while(fast){
    fast = fast->next;
    if(fast){
      slow = slow->next;
      fast = fast->next;
    }
  }
  *a = lista;
  *b = slow->next;
  slow->next = NULL;
}

GList merge(GList a, GList b, FuncionComparar c){
  if(!a) return b;
  if(!b) return a;
  if(c(a->data, b->data) < 0){
    a->next = merge(a->next, b, c);
    return a;
  }
  else{
    b->next = merge(a, b->next, c);
    return b;
  }
}

void mergesort(GList* lista, FuncionComparar c){
  if(!(*lista) || (*lista)->next == NULL)
    return;
  GList a,b;

  dividir(*lista, &a, &b);

  mergesort(&a, c);
  mergesort(&b, c);

  *lista = merge(a, b, c);
}

void imprimir(void* dato){
  printf("%i ", *((int*)dato));
}
void* copia(void* x){
  int* y = malloc(sizeof(int));
  *y = *((int*)x);
  return y;
}

int comp(void* x, void* y){
  return *(int*)x - *(int*)y;
}
int main(){

  GList lista = glist_crear();
  int x = 5;
  lista = glist_agregar_inicio(lista, &x, copia);
  x = 7;
  lista = glist_agregar_inicio(lista, &x, copia);
  x = 2;
  lista = glist_agregar_inicio(lista, &x, copia);
  x = 4;
  lista = glist_agregar_inicio(lista, &x, copia);
  x = 1;
  lista = glist_agregar_inicio(lista, &x, copia);

  printf("Lista original: ");
  glist_recorrer(lista, (FuncionVisitante)imprimir);

  mergesort(&lista, comp);
  puts("");
  printf("Lista ordenada: ");
  glist_recorrer(lista, (FuncionVisitante)imprimir);


  return 0;
}
