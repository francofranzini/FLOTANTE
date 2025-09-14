#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tablahash/tablahash.h"

typedef  struct { 
  char *nombre , *direccion , *dni;
  int  edad;
} Persona;

void* persona_copiar(void* p){
  Persona* x = (Persona*)p;
  Persona* np = malloc(sizeof(Persona));
  np->nombre = malloc(sizeof(char)* (strlen(x->nombre)+1));
  np->direccion = malloc(sizeof(char)*(strlen(x->direccion)+1));
  np->dni = malloc(sizeof(char)*(strlen(x->dni)+1));

  strcpy(np->nombre, x->nombre);
  strcpy(np->direccion, x->direccion);
  strcpy(np->dni, x->dni);

  np->edad = x->edad;
  return np;
}
int persona_comparar(void* p1, void* p2){
  return strcmp(((Persona*)p1)->dni, ((Persona*)p2)->dni);
}

void persona_destruir(void* p){
  Persona* p1 = (Persona*)p;
  free(p1->nombre);
  free(p1->direccion);
  free(p1->dni);
  free(p1);
}
unsigned persona_hash(void* p){
  Persona* p1 = (Persona*)p;
  unsigned idx = 1;
  for(int i = 0; i<strlen(p1->dni); i++){
    idx = idx*p1->dni[i] + 31;
  }
  return idx;
}

void persona_imprimir(Persona* p){
  printf("------------------------\n");
  printf("Nombre: %s\n", p->nombre);
  printf("Direcion: %s\n", p->direccion);
  printf("DNI: %s\n", p->dni);
  printf("Edad: %i\n", p->edad);
}

int main(){

  TablaHash tabla = tablahash_crear(10, persona_copiar,persona_comparar,persona_destruir, persona_hash);

  Persona p1 = {"Dante Zanarini", "Corrientes 1700", "46295036", 45};
  Persona p2 = {"Federico Severino Guimpel", "Entre Rios 1500", "34567873", 60};

  tablahash_insertar(tabla, &p1);
  tablahash_insertar(tabla, &p2);

  assert(tablahash_buscar(tabla,&p1) != NULL);
  assert(tablahash_buscar(tabla,&p2) != NULL);
  
  persona_imprimir(tablahash_buscar(tabla, &p1));
  persona_imprimir(tablahash_buscar(tabla, &p2));

  tablahash_eliminar(tabla, &p2);
  assert(tablahash_buscar(tabla,&p2) == NULL);

  tablahash_redimensionar(tabla);

  assert(tablahash_buscar(tabla,&p1) != NULL);
  assert(tablahash_buscar(tabla,&p2) == NULL);
  
  tablahash_destruir(tabla);


  return 0;
}