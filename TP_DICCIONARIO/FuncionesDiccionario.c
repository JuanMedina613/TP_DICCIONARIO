#include "FuncionesDiccionario.h"
#include "Lista.h"

int crear_dic(tDiccionario *pd, size_t capacidad)
{
    pd->lista = malloc(capacidad * sizeof(tLista));
    if(!(pd->lista))
            return ERROR1;
    pd->capacidad = capacidad;
    tLista *aux = pd->lista;
    pd->cantColisiones = 0;

    while(aux < pd->lista + capacidad)
    {
        crearLista(aux);
        aux++;
    }
    return TODO_OKEY;
}
///================================================================================================================================///
int poner_dic(tDiccionario *pd, const void *valor, size_t tamDato, const char *clave)
{
    sDato nuevo;
    size_t pos;

    if(!valor)
        return VACIO;

    nuevo.tam = tamDato;
    nuevo.clave = (char *)malloc(strlen(clave)+1);
    nuevo.valor = malloc(tamDato);

    if(!nuevo.clave || !nuevo.valor)
    {
        free(nuevo.clave);
        free(nuevo.valor);
        return SIN_MEM;
    }

    strcpy(nuevo.clave, clave);
    memcpy(nuevo.valor, valor, tamDato);

    pos = hashDiccionario(clave) % pd->capacidad;

    if(*(pd->lista + pos) != NULL)
        pd->cantColisiones++;

    if(!listaInsertarActDup((pd->lista + pos), &nuevo, sizeof(nuevo), cmpClave, actValorReemplazo))
    {
        free(nuevo.clave);
        free(nuevo.valor);
        return ERROR1;
    }
    return TODO_OKEY;
}
///================================================================================================================================///
int obtener_dic(const tDiccionario *pd, void *destino, size_t cant, const char *claveaBuscar, int (*cmp)(const void *,const void *))
{
    size_t pos;
    sDato aux;
    tLista *pLista;

    if(!pd || !pd->lista)
        return VACIO;
    pos = (hashDiccionario(claveaBuscar) % pd->capacidad); //Obtengo la posicion del vector de lista de la clave a buscar
    pLista = (pd->lista + pos);                            //Apunto a la lista en donde se encuentra la clave

    if(listabuscarContenido(pLista,&aux,sizeof(sDato),claveaBuscar,cmp) != TODO_OK)
        return ERROR1;

    memcpy(destino, aux.valor, MIN(cant, aux.tam));

    return TODO_OK;
}
///================================================================================================================================///
int sacar_dic(tDiccionario *pd, void *destino, size_t cant, const char *claveBusqueda, int (*cmp)(const void *,const void *))
{
    size_t pos;
    tLista *pLista;
    int resultado;

    if(!pd || !pd->lista)
        return VACIO;


    pos = hashDiccionario(claveBusqueda) % pd->capacidad;//Calculo de la lista en la que hay que ir a buscar
    pLista = (pd->lista + pos);                          //Puntero en la lista en la que hay que ir a buscar

    resultado = listaSacarPorContenido(pLista, destino, cant, (const void *)claveBusqueda, cmp, liberar_dato_dic);//Buscamos y sacamos

    return resultado;
}
///================================================================================================================================///
void liberar_dato_dic(void *info)
{
    sDato *dato = (sDato *)info;

    if(dato->clave)
        free(dato->clave);
    if(dato->valor)
        free(dato->valor);
}
///================================================================================================================================///
int cmpClave(const void *v1, const void *v2)
{
    sDato *d1 = (sDato *)v1;
    sDato *d2 = (sDato *)v2;

    return strcmp(d1->clave, d2->clave);
}
///================================================================================================================================///
int cmpClaveBusqueda(const void *v1, const void *v2)
{

    const sDato *dato_nodo = (const sDato *)v1;   //v1 es el sDato* que está dentro del nodo de la lista
    const char *clave_busqueda = (const char *)v2;//v2 es el puntero a la cadena de caracteres (claveBusqueda) que pasamos directamente

    return strcmp(dato_nodo->clave, clave_busqueda);
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
int recorrer_dic(tDiccionario *pd, void (*accion)(void *))
{
    tNodo* nodo;

    if (!pd || !pd->lista)
        return VACIO;

    tLista* ini = pd->lista;
    tLista* fin = pd->lista + pd->capacidad;

    while (ini < fin)
    {

        nodo = *ini;
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
int destruir_dic(tDiccionario *pd,void (*liberar)(void *))
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
            liberar(nodo->info);
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
