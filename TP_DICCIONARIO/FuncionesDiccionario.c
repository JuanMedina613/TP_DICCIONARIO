#include "FuncionesDiccionario.h"

int crear_dic(tDiccionario *pd, size_t capacidad)
{
    pd->lista = malloc(capacidad * sizeof(tLista));
    if(!(pd->lista))
            return ERROR1;
    pd->capacidad = capacidad;
    tLista *aux = pd->lista;
    while(aux < pd->lista + capacidad)
    {
        listaCrear(aux);
        aux++;
    }
    return TODO_OKEY;
}
//====================================================//
int destruir_dic(tDiccionario *pd)
{
    if(!pd || !pd->lista)
        return VACIO;
    tLista* aux = pd->lista;
    while(aux < pd->lista + pd->capacidad)
    {
        listaDestruir(aux);
        aux++;
    }
    free(pd->lista);
    pd->lista = NULL;
    pd->capacidad = 0;
    return TODO_OKEY;
}
//====================================================//
size_t hashDiccionario(const char* str)
{
    size_t hash = 1;
    int c;
    while((c = *str))
        hash = hash*33 + c;
    return hash;

}


/*
Poner en 0 la cant
Llamar a crearLista
Asignar capacidad
*/
