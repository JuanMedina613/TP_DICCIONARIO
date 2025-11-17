#include "lista.h"
#include "FuncionesDiccionario.h"
///================================================================================================================================///
void crearLista(tLista *p)
{
    *p = NULL;
}
///================================================================================================================================///
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
///================================================================================================================================///
void vaciarLista(tLista *p)
{
    tNodo *aux = NULL;
    while(*p)
    {
        aux = *p;
        *p = aux->sig;
        free(aux);
    }
}
///================================================================================================================================///
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
///================================================================================================================================///
int listaRecorrer(tLista *pLista, void (*accion)(void *, void*), void *p)
{
    tNodo *actual = *pLista;

    if(!actual)
        return ERR;

    while(actual != NULL)
    {
        accion(actual->info,p);

        actual = actual->sig;
    }

    return TODO_OK;
}

///================================================================================================================================///
///Busca el nodo por comparación, lo desengancha, copia su información a destino y llama a una función para liberar la info.
int listaSacarPorContenido(tLista *pLista, void *destino, size_t tamDestino, const void *dato, int (*cmp)(const void *, const void *), void (*liberarInfo)(void *))
{
    tNodo *act = *pLista;
    tNodo *ant = NULL;

    while(act && cmp(act->info, dato) != 0)
    {
        ant = act;
        act = act->sig;
    }

    if(!act)
        return ERR;
    if(destino)
        memcpy(destino, act->info, MIN(tamDestino, act->tamInfo));

    if(ant)
        ant->sig = act->sig;
    else
        *pLista = act->sig;

    if(liberarInfo)
        liberarInfo(act->info);

    free(act);

    return TODO_OK;
}
///================================================================================================================================///
int listabuscarContenido(tLista *pLista, void *destino,size_t tam, const char *clave,int (*cmp)(const void *, const void *))
{
    tNodo *aux= *pLista;

    while(aux && (cmp(aux->info,clave) != 0))
    {
        aux = aux->sig; // avanzo
    }

    if(!aux)
        return ERR;

    memcpy(destino,aux->info,MIN(tam, aux->tamInfo));
    return TODO_OK;
}
