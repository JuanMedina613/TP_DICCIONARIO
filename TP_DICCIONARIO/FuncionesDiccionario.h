#ifndef FUNCIONESDICCIONARIO_H_INCLUDED
#define FUNCIONESDICCIONARIO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define ERROR1 0 //Problema de Memoria
#define TODO_OKEY 1
#define VACIO 0
#define NO_ENCONTRADA 0
#define CAPACIDAD_INICIAL 10000 //"A mayor capacidad, mayor velocidad."
typedef struct
{
    tLista *lista;
    size_t capacidad;
    size_t cantColisiones;
}tDiccionario;

typedef struct{
    void* valor;
    size_t tam;
    char* clave;
}sDato;

int crear_dic(tDiccionario *pd, size_t capacidad);
/*
Poner en 0 la cant
Llamar a crearLista capacidad veces
Asignar capacidad
*/

int poner_dic(tDiccionario *pd, const void *valor, size_t tamDato, const char *clave);
/*
Mediante una funcion hash%capacidad, se transforma la clave en una posicion, que es la que define en cual de todas las listas vamos a insertar
*/

int obtener_dic(const tDiccionario *pd, void *destino, size_t cant, const char *claveBusqueda, int (*cmp)(const void *,const void *));
/*
Nos paramos en la lista que cumpla hash%capacidad y usamos buscarLista para encontrar un nodo que en su info tenga una clave igual al hash
Lo que devuelve buscarLista en destino es el valor que está asociado a la clave
*/

int sacar_dic(tDiccionario *pd, void *destino, size_t cant, const char *claveBusqueda, int (*cmp)(const void *,const void *));
/*
Nos paramos en la lista que cumpla hash%capacidad y usamos buscarLista para encontrar un nodo que en su info tenga una clave igual al hash
Lo que devuelve buscarLista (La posición del nodo que tiene esa clave) se envía como posición en sacarListaPos
*/

void liberar_dato_dic(void *info);

int cmpClave(const void *v1, const void *v2); //Recibe 2 sDato

int cmpClaveBusqueda(const void *v1, const void *v2); //Recibe un sDato y un string

void actValorReemplazo(void *actual, void *nuevo);

int recorrer_dic(tDiccionario *pd, void (*accion)(void *));

/*
Llama a la función recorrerLista para cada lista del diccionario
for desde 0 hasta capacidad
recorrerLista(tLista *pd, acccion)
*/

int destruir_dic(tDiccionario *pd,void (*liberar)(void *));
/*
Recorre todas las listas haciendo free y liberando la info del nodo
Capacidad pasa a ser 0
*/

size_t hashDiccionario(const char* str);


#endif // FUNCIONESDICCIONARIO_H_INCLUDED
