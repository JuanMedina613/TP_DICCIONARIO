#include "FuncionesDiccionario.h"
#include "Lista.h"

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
///================================================================================================================================///
int destruir_dic(tDiccionario *pd)
{
    if(!pd || !pd->lista)
        return VACIO;
    tLista* aux = pd->lista;
    tNodo* nodo = NULL;
    while(aux < pd->lista + pd->capacidad)
    {
        nodo = *aux;
        while(nodo)
        {
            liberar_dato_dic(nodo->info);
            nodo = nodo->sig;
        }
        vaciarLista(aux);
        aux++;
    }
    free(pd->lista);
    pd->lista = NULL;
    pd->capacidad = 0;

    return TODO_OKEY;
}
///================================================================================================================================///
size_t hashDiccionario(const char* str)
{
    size_t hash = 1;
    size_t aux = 0;
    int c;

    while((c = *(str + aux)))
    {
        hash = hash*33 + c;
        aux+=1;
    }

    return hash;

}

///================================================================================================================================///
int cmpClave(const void *v1, const void *v2)
{
    sDato *d1 = (sDato *)v1;
    sDato *d2 = (sDato *)v2;

    return strcmp(d1->clave, d2->clave);
}

///================================================================================================================================///
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
///================================================================================================================================///
void actValorSumar(void *actual, void *nuevo)
{
    sDato *act = (sDato *)actual;

    *(int *)act->valor += 1;

}
///================================================================================================================================///
int poner_dic(tDiccionario *pd, const void *valor, size_t tamDato, const char *clave, void (*accion)(void *, void *))
{
    sDato nuevo;
    size_t pos;

    if(!valor)
        return VACIO;

    //printf("\nTotal de palabras hasta ahora: %zu",contarPalabras(pd));
    nuevo.tam = tamDato;
    nuevo.clave = (char *)malloc(strlen(clave)+1);
    nuevo.valor = malloc(tamDato);

    if(!nuevo.clave || !nuevo.valor)
        return SIN_MEM;

    strcpy(nuevo.clave, clave);
    memcpy(nuevo.valor, valor, tamDato);

    pos = hashDiccionario(clave) % pd->capacidad;
    //printf("\nSe suma %s un total de %d\n", clave, *(int*)valor);
    //printf("\nPoniendo %s", clave);
    if(!listaInsertarActDup((pd->lista + pos), &nuevo, sizeof(nuevo), cmpClave, accion))
    {
        free(nuevo.clave);
        free(nuevo.valor);
        return ERROR1;
    }

    return TODO_OKEY;
}
///================================================================================================================================///

///Función auxiliar para liberar la memoria dinámica dentro de un sDato (la clave y el valor) antes de que el nodo de la lista sea liberado.
void liberar_dato_dic(void *info)
{
    sDato *dato = (sDato *)info;

    if(dato->clave)
        free(dato->clave);
    if(dato->valor)
        free(dato->valor);
}
///================================================================================================================================///
int sacar_dic(tDiccionario *pd, void *destino, size_t cant, const char *claveBusqueda, int (*cmp)(const void *,const void *))
{
    size_t pos;
    tLista *pLista;
    int resultado;

    if(!pd || !pd->lista) // diccionario o lista vacia?
        return VACIO;

    // 1. Calcular la posición (hash)
    pos = hashDiccionario(claveBusqueda) % pd->capacidad;
    pLista = (pd->lista + pos); // Puntero a la lista en el bucket

    // 2. Buscar y sacar el nodo de la lista
    resultado = listaSacarPorContenido(pLista, destino, cant, (const void *)claveBusqueda, cmp, liberar_dato_dic);

    return (resultado == TODO_OK) ? TODO_OKEY : resultado;
}
///================================================================================================================================///
int cmpClaveBusqueda(const void *v1, const void *v2)
{
    // v1 es el sDato* que está dentro del nodo de la lista
    const sDato *dato_nodo = (const sDato *)v1;

    // v2 es el puntero a la cadena de caracteres (claveBusqueda) que pasamos directamente
    const char *clave_busqueda = (const char *)v2;

    return strcmp(dato_nodo->clave, clave_busqueda);
}
///================================================================================================================================///
int recorrer_dic(tDiccionario *pd, void (*accion)(void *))
{
    if (!pd || !pd->lista)
        return VACIO;

    tLista* ini = pd->lista;
    tLista* fin = pd->lista + pd->capacidad;
    while (ini < fin)
    {

        tNodo* nodo = *ini;
        while (nodo != NULL)
        {
            accion(nodo->info);
            nodo = nodo->sig;
        }
        ini++;
    }
    return TODO_OKEY;
}
///================================================================================================================================///
int obtener_dic(const tDiccionario *pd, void *destino, size_t cant, const char *claveaBuscar, int (*cmp)(const void *,const void *))
{
    size_t pos;
    tLista *pLista;

    if(!pd || !pd->lista)
        return VACIO;
    pos = (hashDiccionario(claveaBuscar) % pd->capacidad); // obtengo la pos del vector de lista de la clave a buscar
    pLista = (pd->lista + pos); //Apunto a la lista en donde se encuentra la clave

    if(!listabuscarContenido(pLista,destino,cant,claveaBuscar,cmp))
        return ERROR1;

    return TODO_OK;
}
