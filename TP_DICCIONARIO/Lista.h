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

int cmp(const void *a , const void* b);
void actSumarDuplicados(const void *v1, void *v2);

void crearLista(tLista *p);
void vaciarLista(tLista *p);

int listaRecorrer(tLista *pLista, void (*accion)(void *, void*), void *p);
int listaBuscarPos(tLista *p, void* dato, int (*cmp)(const void *, const void*));
int listaInsertarActDup(tLista *pLista, const void *dato, size_t tamDato, int (*cmp)(const void *, const void*), void (*accion)(void *, void*));
int listaSacarPorContenido(tLista *pLista, void *destino, size_t tamDestino, const void *dato, int (*cmp)(const void *, const void *), void (*liberarInfo)(void *));

#endif // LISTA_H_INCLUDED
