#include "lista.h"

void listaCrear(tLista *pLista)
{
    *pLista = NULL;

}

int listaSacar(tLista *pLista, void *destino, size_t cant)
{
    tNodo *nodo = *pLista;

    if(!*pLista)
        return ERR;

    memcpy(destino, nodo->info ,MIN(cant, nodo->tamInfo));
    *pLista = nodo->sig;
    free(nodo);
    return TODO_OK;
}

int listaVacia(const tLista *pLista)
{
    return *pLista == NULL;
}

void listaDestruir(tLista *pLista)
{
    tNodo *elim = *pLista;
    while(*pLista)
    {
        *pLista = elim->sig;
        free(elim);
        elim = *pLista;
    }
}

int listaLlena(const tLista *pLista, size_t cantBytes)
{
    tNodo *nodo = (tNodo *)malloc(sizeof(tNodo) + cantBytes);
    free(nodo);
    if(!nodo)
        return SIN_MEM;
    else
        return TODO_OK;
}

int listaInsertarPri(tLista *pLista, const void* dato, size_t tamDato)
{
    tNodo *nodo = (tNodo *)malloc(sizeof(tNodo)+tamDato);
    if(!nodo)
        return SIN_MEM;

    nodo->info = nodo + 1;

    memcpy(nodo->info, dato, tamDato);
    nodo->tamInfo = tamDato;
    nodo->sig = *pLista;
    *pLista = nodo;

    return TODO_OK;
}

int listaInsertarUlt(tLista *pLista, const void* dato, size_t tamDato)
{
    tNodo *nodo = (tNodo *)malloc(sizeof(tNodo) + tamDato);
    if(!nodo)
        return SIN_MEM;

    while(*pLista)
    {
        pLista = &(*pLista)->sig;
    }

    nodo->info = nodo+1;

    memcpy(nodo->info,dato,tamDato);
    nodo->tamInfo=tamDato;
    nodo->sig = NULL;
    *pLista = nodo;

    return TODO_OK;
}

int listaInsertarPos(tLista *pLista, const void* dato, size_t tamDato, size_t pos)
{
    tNodo *nodo = (tNodo *)malloc(sizeof(tNodo) + tamDato);
    size_t aux = 0;

    if(!nodo)
        return SIN_MEM;
    if(pos < 0)
    {
        free(nodo);
        return ERR;
    }

    while(pos != aux && *pLista)
    {
        aux++;
        pLista = &(*pLista)->sig;
    }

    nodo->tamInfo = tamDato;
    nodo->info = nodo +1;
    memcpy(nodo->info,dato,tamDato);
    nodo->sig = *pLista;
    *pLista = nodo;

    return TODO_OK;
}

int cmpINT(const void *v1, const void*v2)
{
    const int *aux1 = (const int *)v1;
    const int *aux2 = (const int *)v2;

    if (*aux1 < *aux2)
        return -1;
    if (*aux2 < *aux1)
        return 1;
    return 0;

}

int listaInsertarOrdConDup(tLista *pLista, const void* dato, size_t tamDato, int (*cmp)(const void *, const void*))
{
    tNodo *nodoB = *pLista;
    tNodo *ant = NULL;
    tNodo *insertar = (tNodo *)malloc(tamDato + sizeof(tNodo));

    if(!insertar)
        return SIN_MEM;

    insertar->info = insertar+1;
    insertar->tamInfo = tamDato;
    memcpy(insertar->info, dato, tamDato);

    while(nodoB && cmp(dato, nodoB->info)>0)
    {
        ant = nodoB;
        nodoB = nodoB->sig;
    }

    insertar->sig = nodoB;
    if(!ant)
        *pLista = insertar;
    else
        ant->sig = insertar;

    return TODO_OK;
}

int listaInsertarOrdSinDup(tLista *pLista, const void* dato, size_t tamDato, int (*cmp)(const void *, const void*))
{
    tNodo *nodoB = *pLista;
    tNodo *ant = NULL;
    tNodo *insertar = (tNodo *)malloc(tamDato + sizeof(tNodo));

    if(!insertar)
        return SIN_MEM;

    insertar->info = insertar+1;
    insertar->tamInfo = tamDato;
    memcpy(insertar->info, dato, tamDato);

    while(nodoB && cmp(dato, nodoB->info)>0)
    {
        ant = nodoB;
        nodoB = nodoB->sig;
    }

    if(nodoB && cmp(dato, nodoB->info)==0)
    {
        free(insertar);
        return DUPLICADO;
    }

    insertar->sig = nodoB;
    if(!ant)
        *pLista = insertar;
    else
        ant->sig = insertar;

    return TODO_OK;
}

void actSumarDuplicados(const void *v1, void *v2)
{
    const int *d1 = (const int *)v1;
    int *d2 = (int *)v2;
    *d2 += *d1;
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

int listaOrdenarSeleccion(tLista *pLista, int cmp(const void*, const void*))
{
    tNodo *min = *pLista;
    tNodo *act = *pLista;
    tNodo *ant = NULL;
    tNodo *antAux = NULL;
    if(!*pLista)
        return ERR;

    while(*pLista)
    {
        min = *pLista;
        act = *pLista;
        ant = NULL;
        antAux = NULL;

        while(act)
        {
            if(cmp(min->info, act->info) > 0)
            {
                antAux = ant;
                min = act;
            }
            ant = act;
            act = act->sig;
        }

        if(min != *pLista)
        {
            if(antAux)
                antAux->sig = min->sig;

            act = *pLista;
            *pLista = min;
            min->sig = act;
        }

        pLista = &((*pLista)->sig);
    }

    return TODO_OK;
}

