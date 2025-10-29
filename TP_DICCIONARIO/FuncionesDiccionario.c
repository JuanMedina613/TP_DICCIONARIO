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
        crearLista(aux);
        aux++;
    }
    return TODO_OKEY;
}
///===============================================================================//
int destruir_dic(tDiccionario *pd)
{
    if(!pd || !pd->lista)
        return VACIO;
    tLista* aux = pd->lista;
    while(aux < pd->lista + pd->capacidad)
    {
        vaciarLista(aux);
        aux++;
    }
    free(pd->lista);
    pd->lista = NULL;
    pd->capacidad = 0;
    return TODO_OKEY;
}
///===============================================================================//
size_t hashDiccionario(const char* str)
{
    size_t hash = 1;
    int c;
    while((c = *str))
        hash = hash*33 + c;
    return hash;

}

///===============================================================================//
int cmpClave(const void *v1, const void *v2)
{
    sDato *d1 = (sDato *)v1;
    sDato *d2 = (sDato *)v2;

    return strcmp(d1->clave, d2->clave);
}

///===============================================================================//
void actValorReemplazo(void *actual, void *nuevo)
{
    sDato *act = (sDato *)actual;
    sDato *nue = (sDato *)nuevo;
    act->tam = nue->tam;
    void *aux = malloc(nue->tam);
    if(!aux)
        return;

    memcpy(aux, nue->valor, nue->tam);
    free(act->valor);
    act->valor = aux;
}
///===============================================================================//
void actValorSumar(void *actual, void *nuevo)
{
    sDato *act = (sDato *)actual;

    *(int *)act->valor += 1;

}
///===============================================================================//
int poner_dic(tDiccionario *pd, const void *valor, size_t tamDato, const char *clave, void (*accion)(void *, void *))
{
    sDato nuevo;
    size_t pos;

    if(!valor)
        return VACIO;

    nuevo.tam = tamDato;
    nuevo.clave = (char *)malloc(strlen(clave)+1);
    nuevo.valor = malloc(tamDato);

    if(!nuevo.clave || !nuevo.valor)
        return SIN_MEM;

    strcpy(nuevo.clave, clave);
    memcpy(nuevo.valor, valor, tamDato);

    pos = hashDiccionario(clave) % pd->capacidad;

    if(!listaInsertarActDup((pd->lista + pos), &nuevo, sizeof(nuevo), cmpClave, accion))
    {
        free(nuevo.clave);
        free(nuevo.valor);
        return ERROR1;
    }

    return TODO_OKEY;
}
///===============================================================================//
/*
Poner en 0 la cant
Llamar a crearLista
Asignar capacidad
*/
