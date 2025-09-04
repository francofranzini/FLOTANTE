#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablahash/tablahash.h"
#include <assert.h>
typedef  struct {
  char *nombre, *direccion , *dni;
  int  edad;
} Persona;

void* persona_copiar(Persona* p){
  Persona* np = malloc(sizeof(Persona));
  np->nombre = malloc(sizeof(char)* (strlen(p->nombre)+1));
  strcpy(np->nombre, p->nombre);

  np->direccion = malloc(sizeof(char)*(strlen(p->direccion)+1));
  strcpy(np->direccion, p->direccion);

  np->dni = malloc(sizeof(char)*(strlen(p->dni)+1));
  strcpy(np->dni, p->dni);

  np->edad = p->edad;

  return np;
}
int persona_comparar(Persona* p1, Persona* p2){
  return strcmp(p1->dni, p2->dni);
}
void persona_destruir(Persona* p){
  free(p->nombre);
  free(p->direccion);
  free(p->dni);
  free(p);
}
unsigned persona_hash(Persona* p){
  unsigned idx = 0;
  for(int i = 0; i<strlen(p->dni); i++){
    idx = p->dni[i] + 31*idx;
  }
  return idx;
}

int main(){
  
  int size = 4;
  
  TablaHash tabla = tablahash_crear(
    size,
    (FuncionCopiadora) persona_copiar,
    (FuncionComparadora) persona_comparar,
    (FuncionDestructora) persona_destruir,
    (FuncionHash)persona_hash
  );
  
  Persona* p1 = malloc(sizeof(Persona));
  p1->nombre = malloc(sizeof(char)* strlen("Maria Luisa  "));
  strcpy(p1->nombre, "Maria Luisa");
  p1->direccion = malloc(sizeof(char)* strlen("Sucasa   "));
  strcpy(p1->direccion, "Sucasa");
  p1->dni = malloc(sizeof(char)* (strlen("46295036")+1));
  strcpy(p1->dni, "46295036");
  p1->edad = 66;
  
  Persona *p2 = malloc(sizeof(Persona));
  p2->nombre = malloc(strlen("Lucia Fernandez") + 1);
  strcpy(p2->nombre, "Lucia Fernandez");
  p2->direccion = malloc(strlen("Av. Siempre Viva 742") + 1);
  strcpy(p2->direccion, "Av. Siempre Viva 742");
  p2->dni = malloc(strlen("40123000") + 1);
  strcpy(p2->dni, "40123000");
  p2->edad = 28;
  
  Persona* p0 = malloc(sizeof(Persona));
  p0->nombre = malloc(sizeof(char)* strlen("Camilo Gentile "));
  strcpy(p0->nombre, "Camilo Gentile");
  p0->direccion = malloc(sizeof(char)* strlen("HPR "));
  strcpy(p0->direccion, "HPR");
  p0->dni = malloc(sizeof(char)* (strlen("46295000")+1));
  strcpy(p0->dni, "46295");
  p0->edad = 22;

  Persona *p3 = malloc(sizeof(Persona));
  p3->nombre = malloc(strlen("Martin Lopez") + 1);
  strcpy(p3->nombre, "Martin Lopez");
  p3->direccion = malloc(strlen("Calle Falsa 123") + 1);
  strcpy(p3->direccion, "Calle Falsa 123");
  p3->dni = malloc(strlen("38999000") + 1);
  strcpy(p3->dni, "38999000");
  p3->edad = 35;

  Persona *p4 = malloc(sizeof(Persona));
  p4->nombre = malloc(strlen("Ana Torres") + 1);
  strcpy(p4->nombre, "Ana Torres");
  p4->direccion = malloc(strlen("Belgrano 55") + 1);
  strcpy(p4->direccion, "Belgrano 55");
  p4->dni = malloc(strlen("42111000") + 1);
  strcpy(p4->dni, "42111000");
  p4->edad = 30;

  Persona *p5 = malloc(sizeof(Persona));
  p5->nombre = malloc(strlen("Javier Perez") + 1);
  strcpy(p5->nombre, "Javier Perez");
  p5->direccion = malloc(strlen("San Martin 1000") + 1);
  strcpy(p5->direccion, "San Martin 1000");
  p5->dni = malloc(strlen("37666000") + 1);
  strcpy(p5->dni, "37666000");
  p5->edad = 40;


printf("Hash0: %i\n Hash1: %i\nHash2: %i\nHash3: %i\nHash4: %i\nHash5: %i\n", persona_hash(p0)%size,persona_hash(p1)%size,persona_hash(p2)%size,persona_hash(p3)%size,persona_hash(p4)%size,persona_hash(p5)%size);

  tablahash_insertar(tabla, p0);
  assert(tablahash_nelems(tabla) == 1);
  tablahash_insertar(tabla, p1);
  assert(tablahash_nelems(tabla) == 2);
  tablahash_insertar(tabla, p2);
  assert(tablahash_nelems(tabla) == 3);
  tablahash_insertar(tabla, p3);
  assert(tablahash_nelems(tabla) == 4);
  tablahash_insertar(tabla, p4);
  assert(tablahash_nelems(tabla) == 5);
  tablahash_insertar(tabla, p5);
  assert(tablahash_nelems(tabla) == 6);

  assert(tablahash_buscar(tabla, p5) != NULL);
  
  tablahash_eliminar(tabla, p2);

  assert(tablahash_buscar(tabla, p5) != NULL);

  tablahash_redimensionar(tabla);

  assert(tablahash_capacidad(tabla) == 8);



  tablahash_destruir(tabla);

  persona_destruir(p0);
  persona_destruir(p1);
  persona_destruir(p2);
  persona_destruir(p3);
  persona_destruir(p4);
  persona_destruir(p5);




  return 0;
}