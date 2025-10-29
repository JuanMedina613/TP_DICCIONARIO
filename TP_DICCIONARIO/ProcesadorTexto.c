#include "ProcesadorTexto.h"

void sumarValoresDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    int * acum = (int *)destino;
    char c = *(char *)elemento->clave;
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    (*acum) += *(int *)(elemento->valor);
}
///===============================================================================//
size_t contarPalabras(tDiccionario *pd)
{
    size_t acum = 0;
    size_t aux;

    if(!pd || !pd->lista)
    {
        return VACIO;
    }
    for(aux=0; aux<pd->capacidad;aux++)
    {
        listaRecorrer(pd->lista + aux, sumarValoresDic, &acum);
    }

    return acum;
}
///===============================================================================//
int contarAparicionesPalabra(tDiccionario* pd)
{
    int cont = 0;
    tLista* fin = pd->lista + pd->capacidad;
    tLista* ini = pd->lista;
    while (ini < fin)
    {
        tNodo* nodo = *ini;              // <-- antes intentabas usar (*ini)->sig directamente
        while (nodo != NULL)            // <-- reemplaza el while(!((*ini)->sig))
        {
            sDato* dato = (sDato*)nodo->info;   // <-- antes usabas ((*ini)->sig->info)
            cont += *(int*)dato->valor;         // casteo y sumo
            nodo = nodo->sig;            // <-- avanzar la lista, esto antes no estaba
        }
        ini++;  // siguiente
    }
    return cont;
}
///===============================================================================//
