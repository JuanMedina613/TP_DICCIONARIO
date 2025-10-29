#ifndef PROCESADORTEXTO_H_INCLUDED
#define PROCESADORTEXTO_H_INCLUDED
#include "FuncionesDiccionario.h"
#define MAXLINE 1024
#include <string.h>
#include <ctype.h>

size_t contarPalabras(tDiccionario *pd);
//contarEspacios
//contarSignos
int contarAparicionesPalabra(tDiccionario* pd);
size_t contarSignos(tDiccionario *pd);
void sumarSignosDic(void *DatoDiccionario, void *destino);
int seleccionarArchivo(tDiccionario*pd);
int cargarArchivoEnDiccionario(tDiccionario* pd,FILE *pf);
int TrozaryGuardarArchivo(char *linea,sDato *dato,tDiccionario *pd);
void actValorSumar(void *actual, void *nuevo);
//menu

#endif // PROCESADORTEXTO_H_INCLUDED
