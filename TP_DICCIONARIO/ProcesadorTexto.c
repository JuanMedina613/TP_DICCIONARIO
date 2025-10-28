#include "ProcesadorTexto.h"

void sumarValoresDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    int * acum = (int *)destino;
    if((*(char *)elemento->valor <= 90 && *(char *)elemento->valor >= 65) || (*(char *)elemento->valor <= 122 && *(char *)elemento->valor >= 97))
    (*acum) += *(int *)(elemento->valor);
}

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

