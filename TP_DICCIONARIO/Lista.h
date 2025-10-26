#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X,Y) ((X)<=(Y)?(X):(Y))

#define SIN_MEM 0
#define TODO_OK 1
#define DUPLICADO 2
#define ERR 3
typedef struct sNodo
{
    void *info;
    size_t tamInfo;
    struct sNodo *sig;
} tNodo;

typedef tNodo* tLista;

int cmpINT(const void *v1, const void*v2);
void actSumarDuplicados(const void *v1, void *v2);

void listaCrear(tLista *pLista);
void listaDestruir(tLista *pLista);
int listaInsertarPri(tLista *pLista, const void* dato, size_t tamDato);
int listaInsertarUlt(tLista *pLista, const void* dato, size_t tamDato);
int listaSacar(tLista *pLista, void *destino, size_t cant);
int listaVacia(const tLista *pLista);
int listaInsertarPos(tLista *pLista, const void* dato, size_t tamDato, size_t pos);
int listaInsertarOrdConDup(tLista *pLista, const void* dato, size_t tamDato, int (*cmp)(const void *, const void*));
int listaInsertarOrdSinDup(tLista *pLista, const void* dato, size_t tamDato, int (*cmp)(const void *, const void*));
int listaInsertarActDup(tLista *pLista, const void *dato, size_t tamDato, int (*cmp)(const void *, const void*), void (*accion)(const void *, void*));
int listaOrdenarSeleccion(tLista *pLista, int cmp(const void*, const void*));

#endif // LISTA_H_INCLUDED
