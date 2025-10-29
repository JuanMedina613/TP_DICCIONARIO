#ifndef FUNCIONESDICCIONARIO_H_INCLUDED
#define FUNCIONESDICCIONARIO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define ERROR1 0 //Problema relacionado a la Memoria
#define TODO_OKEY 1 // TODO_OKEY JAJA
#define VACIO 1
typedef struct
{
    tLista *lista;
    size_t capacidad;
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

int poner_dic(tDiccionario *pd, const void *valor, size_t tamDato, const char *clave, void (*accion)(void *, void *));

/*
Mediante una funcion hash%capacidad, se transforma la clave en una posicion, que es la que define en cual de todas las listas vamos a insertar
Utilizamos la funcion accion para definir si reemplazamos el valor por el nuevo, o para sumar al contador de apariciones.

*/

int obtener_dic(const tDiccionario *pd, void *destino, size_t cant, const char *claveBusqueda, int (*cmp)(const void *,const void *));

/*
Nos paramos en la lista que cumpla hash%capacidad y usamos buscarLista para encontrar un nodo que en su info tenga una clave igual al hash
Lo que devuelve buscarLista (La posición del nodo que tiene esa clave) se envía como posición en verListaPos
*/

int sacar_dic(tDiccionario *pd, void *destino, size_t cant, const char *claveBusqueda, int (*cmp)(const void *,const void *));

/*
Nos paramos en la lista que cumpla hash%capacidad y usamos buscarLista para encontrar un nodo que en su info tenga una clave igual al hash
Lo que devuelve buscarLista (La posición del nodo que tiene esa clave) se envía como posición en sacarListaPos
Se reduce la capacidad en 1
*/

int recorrer_dic(tDiccionario *pd, void (*accion)(void *, void *));

/*
Llama a la función recorrerLista para cada lista del diccionario
for desde 0 hasta capacidad
recorrerLista(tLista *pd, acccion)
*/

int destruir_dic(tDiccionario *pd);
/*
Recorre todas las listas haciendo free
Capacidad y cant pasan a ser 0
*/

size_t hashDiccionario(const char* str);


#endif // FUNCIONESDICCIONARIO_H_INCLUDED
