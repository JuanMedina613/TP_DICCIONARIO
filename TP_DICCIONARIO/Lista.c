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

    //printf("\nSe suma %s un total de %d\n", ((sDato *)(dato))->clave, *(size_t*)((sDato*)(dato))->valor);

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
        return ERR; // No se encontró el elemento

    // 1. Copiar la información al destino (si se necesita)
    if(destino)
        memcpy(destino, act->info, MIN(tamDestino, act->tamInfo));

    // 2. Desenganchar el nodo
    if(ant)
        ant->sig = act->sig;
    else
        *pLista = act->sig; // El nodo a sacar es la cabecera

    // 3. Liberar la memoria interna (clave y valor del sDato)
    if(liberarInfo)
        liberarInfo(act->info);

    // 4. Liberar el nodo
    free(act);

    return TODO_OK;
}
///================================================================================================================================///
int listabuscarContenido(tLista *pLista, void *destino,size_t tam, const char *clave,int (*cmp)(const void *, const void *))
{
    tNodo *aux= *pLista;

    while(aux && (cmp(aux->info,clave) != 0)) // mientras aux siga existiendo y de diferente a la clave, que siga avanzando
    {
        aux = aux->sig; // avanzo
    }

    if(!aux)
        return ERR; // el elemento no se encuentra

    memcpy(destino,aux->info,MIN(tam, aux->tamInfo)); // si paso el if, quiere decir que encontro el nodo, por ende lo copia
    return TODO_OK;
}
///================================================================================================================================///
int listaSacarPorPosicion(tLista *pLista, void *destino, size_t tam, unsigned pos)
{
    if( *pLista == NULL )
        return VACIO;

    tNodo *actual = *pLista;
    tNodo *anterior = NULL;
    unsigned i = 0;

    while ( actual != NULL && i < pos)
    {
        anterior = actual;
        actual = actual ->sig;
        i++;
    }
    if(!actual)
        return VACIO;
    memcpy(destino, actual->info, MIN(actual->tamInfo, tam));

    if(anterior)
        anterior->sig = actual->sig;
    else
        *pLista = actual->sig;

    free(actual->info);
    free(actual);

    return TODO_OKEY;
}
