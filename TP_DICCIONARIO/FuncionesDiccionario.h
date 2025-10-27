#ifndef FUNCIONESDICCIONARIO_H_INCLUDED
#define FUNCIONESDICCIONARIO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

typedef struct
{
    tLista *dic_Lista;
    size_t capacidad;
}tDiccionario;

void crear_dic(tDiccionario *pd, size_t capacidad);

/*
Poner en 0 la cant
Llamar a crearLista capacidad veces
Asignar capacidad
*/

int poner_dic(tDiccionario *pd, const void *valor, const char *clave); //Todavía no la podemos hacer

/*
Mediante una funcion hash%capacidad, se transforma la clave en una posicion, que es la que define en cual de todas las listas vamos a insertar
Insertamos en el nodo cuya clave sea el hash, pisando de ser necesario al valor en la posición
Se aumenta en 1 la capacidad
*/

int obtener_dic(const tDiccionario *pd, const void *valor, const char *clave, int (*cmp)(const void *,const void *));//Todavía no la podemos hacer

/*
Nos paramos en la lista que cumpla hash%capacidad y usamos buscarLista para encontrar un nodo que en su info tenga una clave igual al hash
Lo que devuelve buscarLista (La posición del nodo que tiene esa clave) se envía como posición en verListaPos
*/

int sacar_dic(tDiccionario *pd, void *destino, size_t cant, const char *clave, int (*cmp)(const void *,const void *));//Todavía no la podemos hacer

/*
Nos paramos en la lista que cumpla hash%capacidad y usamos buscarLista para encontrar un nodo que en su info tenga una clave igual al hash
Lo que devuelve buscarLista (La posición del nodo que tiene esa clave) se envía como posición en sacarListaPos
Se reduce la capacidad en 1
*/

int recorrer_dic(tDiccionario *pd, void (*accion)(void *, void *));

/*
Llama a la función recorrerLista para cada lista del diccionario
*/

void destruir_dic(tDiccionario *pd);
/*
Recorre todas las listas haciendo free
Capacidad y cant pasan a ser 0
*/

#endif // FUNCIONESDICCIONARIO_H_INCLUDED
