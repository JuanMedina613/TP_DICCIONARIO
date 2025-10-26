#ifndef FUNCIONESDICCIONARIO_H_INCLUDED
#define FUNCIONESDICCIONARIO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

typedef struct
{
    tLista tabla;
    size_t capacidad;
    size_t cant;
}tDiccionario;

void crear_dic(tDiccionario *pd, size_t capacidad);

/*
Poner en 0 la cant
Llamar a crearLista
Asignar capacidad
*/

int poner_dic(tDiccionario *pd, const void *valor, const char *clave);
int obtener_dic(const tDiccionario *pd, const void *valor, const char *clave);
int sacar_dic(tDiccionario *pd, void *destino, size_t cant);
int recorrer_dic(tDiccionario *pd);

void destruir_dic(tDiccionario *pd);
/*
Recorre toda la lista haciendo free
Capacidad y cant pasan a ser 0
*/
#endif // FUNCIONESDICCIONARIO_H_INCLUDED
