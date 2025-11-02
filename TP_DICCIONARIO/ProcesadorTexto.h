#ifndef PROCESADORTEXTO_H_INCLUDED
#define PROCESADORTEXTO_H_INCLUDED
#include "FuncionesDiccionario.h"
#define MAXLINE 2048
#include <string.h>
#include <ctype.h>

void actValorSumar(void *actual, void *nuevo);
void sumarSignosDic(void *DatoDiccionario, void *destino);
void imprimirDato(void *DatoDiccionario);

size_t contarPalabras(tDiccionario *pd);
size_t contarEspacios(tDiccionario *pd);
size_t contarSignos(tDiccionario *pd);
size_t contarApariciones_de_una_Palabra(tDiccionario* pd, const char* palabra);

int seleccionarArchivo(tDiccionario*pd);
int cargarArchivoEnDiccionario(tDiccionario* pd,FILE *pf);
int TrozaryGuardarArchivo(char *linea,sDato *dato,tDiccionario *pd);

void menu(tDiccionario *pd);

int miEsAlpha(int c);
char *quitarEspeciales(char *palabra);

#endif // PROCESADORTEXTO_H_INCLUDED
