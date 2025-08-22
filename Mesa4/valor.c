#include "valor.h"
#define INF 1000000001;
Valores valores_crear(){ return NULL;}

void ajustar_cant_menores_sumar(Valores valores, int n, int t){
  if(valores == NULL) return;
  if(valores->valor > n){
    if(t == 1) valores->cant_menores++;
    if(t == 0) valores->cant_menores--;
  } 
  ajustar_cant_menores(valores->sig, n, t);
}


Valores insertar_en_posicion(Valores valores, int val, int pos, int menores){
  if(valores == NULL) return NULL;
  if(pos != 0){
    if(valores->valor < val) valores->sig = insertar_en_posicion(valores->sig, val, pos-1, menores + 1);
    else valores->sig = insertar_en_posicion(valores->sig, val, pos-1, menores);
    return valores;
  } 
  else{
    Casilla* nueva_casilla = malloc(sizeof(Casilla));
    nueva_casilla->valor = val;
    nueva_casilla->cant_menores = menores;
    ajustar_cant_menores(valores->sig, val, 1);
    nueva_casilla->sig = valores->sig;
    return nueva_casilla;
  }
}

Valores borrar_en_posicion(Valores valores, int pos){
  if(pos != 0) valores->sig = borrar_en_posicion(valores->sig, pos-1);
  else{
    Casilla* casilla_eliminar = valores;
    ajustar_cant_menores(valores->sig, valores->valor, 0);
    valores = valores->sig;
    free(casilla_eliminar);
  }
  return valores;
}

Valores buscar_por_CantMenores(Valores valores, int n){
  if(valores == NULL) return;
  if(valores->cant_menores == n){
    Casilla* nueva_casilla = malloc(sizeof(Casilla));
    nueva_casilla->valor = valores->valor;
    nueva_casilla->cant_menores = valores->valor;
    nueva_casilla->sig = buscar_por_CantMenores(valores->sig, n);
    return nueva_casilla;
  }
  else return buscar_por_CantMenores(valores->sig, n);
}

int menor(Valores valores, int menor){
  int m = -INF;
  Valores aux = valores;
  while(aux != NULL){
    if(aux->valor > menor && (m == -INF || aux->valor < m)) m = aux->valor;
  }
  if(m == -INF) return -INF;
  else return m;
}


int valores_cantidad(Valores valores){
  if(valores == NULL) return 0;
  else return 1 + valores_cantidad(valores->sig);
}

Valores ordenar_por_cantidadMenores(Valores valores){
  if(valores == NULL) return;
  Valores ordenados = valores_crear(), aux = valores_crear();
  int i = 0;
  int n = valores_cantidad(valores);
  int a[n];
  int m = menor(valores, -INF);
  while(m != -INF){
    a[i++] = m;
    m = menor(valores, m); // busca el menor numero mayor que m
  }//(O(n^2))
  //[a, ..., a+i] cantidades ordenadas de cantMenores

  for(int j = i; j>=0; j--){
    aux->sig = ordenados;
    aux = buscar_por_CantMenores(valores, a[j]);
    ordenados = aux;
  }
  //(O(n^2))

  return ordenados;
}

