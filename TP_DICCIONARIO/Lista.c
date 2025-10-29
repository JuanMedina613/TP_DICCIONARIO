#include "lista.h"

void crearLista(tLista *p)
{
    *p = NULL;
}

int listaInsertarActDup(tLista *pLista, const void *dato, size_t tamDato, int (*cmp)(const void *, const void*), void (*accion)(void *, void*))
{
    tNodo *insertar = (tNodo *)malloc(sizeof(tNodo) + tamDato);
    tNodo *ant = NULL;
    tNodo *act = *pLista;
    void *nDato = malloc(tamDato);

    if(!insertar || !nDato)
    {
        free(nDato);
        free(insertar);
        return SIN_MEM;
    }

    memcpy(nDato, dato, tamDato);

    while(act && cmp(nDato, act->info) > 0)
    {
        ant = act;
        act = act->sig;
    }

    if(act && cmp(nDato, act->info) == 0)
    {
        accion(act->info, nDato);

        free(nDato);
        free(insertar);
        return TODO_OK;
    }

    insertar->info = insertar+1;
    insertar->tamInfo = tamDato;
    insertar->sig = act;
    memcpy(insertar->info, nDato, tamDato);

    (ant!= NULL)?(ant->sig = insertar):(*pLista = insertar);

    free(nDato);
    return TODO_OK;
}

void vaciarLista(tLista *p)
{
    while(*p)
    {
        tNodo *aux = *p;

        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}
int listaBuscarPos(tLista *p, void* dato, int (*cmp)(const void *, const void*))
{
    int pos = 0;
    while(*p)
    {
        if(cmp((*p)->info,dato) == 0) // 0 lo encontro, return pos
            return pos;
        p = &(*p)->sig;
        pos++;
    }
    return -1; // no lo encontro
}

int listaRecorrer(tLista *pLista, void (*accion)(void *, void*), void *p)
{
    tNodo *actual = *pLista;

    if(!actual)
        return ERR;
    while(actual)
    {
        accion(actual->info,p);

        actual = actual->sig;
    }

    return TODO_OK;
}

int cmp(const void *a , const void* b)
{
    return (*(int*)a - *(int*)b); // si devuelve negativo, b es mas grande;si devuelve positivo a es mas grande
}
