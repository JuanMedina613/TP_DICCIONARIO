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
    for(aux=0; aux<pd->capacidad; aux++)
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
int seleccionarArchivo(tDiccionario*pd)
{   // (tolower(*arch) > 100 || tolower(*arch) < 97)
    FILE *pf;
    int nroarch,res;
    char nombreArchivo[20];

    do
    {
        printf("Seleccione numero de archivo(del 1 al 4):\n");
        scanf("%d",&nroarch);
        if(nroarch>4 || nroarch<1)
            printf("\nNumero de archivo invalido, ingrese un numero valido(del 1 al 4):\n");
    }
    while(nroarch>4 || nroarch<1);
    switch (nroarch)
    {
        case 1: strcpy(nombreArchivo, "1.txt");
        break;
        case 2: strcpy(nombreArchivo, "2.txt");
        break;
        case 3: strcpy(nombreArchivo, "3.txt");
        break;
        case 4: strcpy(nombreArchivo, "4.txt");
        break;
    } /// ESTE SWITCH ES EN CASO DE QUE EL USUARIO PONGA NOMBRE DE ARCHIVO
    pf = fopen(nombreArchivo,"rt");
    if(!pf)
    {
        printf("\nError al abrir el archivo!");
        return ERROR1;
    }
    res = cargarArchivoEnDiccionario(pd,pf);
    if(res == 0)
    {
        printf("\nError al cargar el archivo en el diccionario!");
        fclose(pf);
        return ERROR1;
    }
    fclose(pf);
    return TODO_OK;
}
///===============================================================================//
int cargarArchivoEnDiccionario(tDiccionario* pd,FILE *pf)
{
    char linea[MAXLINE];
    sDato aux;

    aux.valor = 0;

    while(fgets(linea,MAXLINE,pf))
    {
       if(!TrozaryGuardarArchivo(linea,&aux,pd))
            return ERROR1;
    }
    return TODO_OK;
}
///===============================================================================//
int TrozaryGuardarArchivo(char *linea,sDato *dato,tDiccionario *pd)
{
    char *dir;
    if(!(dir = strrchr(linea,'\n')))
            return ERROR1;

    *dir = '\0';

    while(dir != linea)
    {
        while((dir - 1) > linea && (tolower(*(dir - 1 )) < 97 || tolower(*(dir - 1)) > 122) && *(dir - 1)!= ' ') // si dir-1 no esta fuera de la linea(existe) y dir-1 no es un caracter y no es espacio vacio insertamos el caracter
            {
                dir = dir - 1;
                sscanf(dir,"%c",dato->clave);
                dato->tam = sizeof(char);
                dato->valor += 1;
                poner_dic(pd,dato->valor,sizeof(char),dato->clave,actValorSumar); // guardamos el caracter para no perderlo
                *dir = '\0'; // una vez guardado el caracter, \0 para seguir trozando
            }
        if(!(dir = strrchr(linea,' ')))
        {
            dir = linea; // En caso de que retorne NULL porque no hay espacios, significa que llegamos a fin de cadena, por ende igualamos
            strcpy(dato->clave,dir); // copio la palabra
        }else
            strcpy(dato->clave,dir + 1); // copio la palabra

        dato->tam = sizeof(strlen(dato->clave));
        dato->valor += 1;
        poner_dic(pd,dato->valor,dato->tam,dato->clave,actValorSumar); // guardo en diccionario para no perder la palabra
        *dir = '\0'; // continuo poniendo \0
        // El if se hace ya que, si estamos parados al principio no se puede hacer dir + 1 ya que nos comeriamos una letra
     }
    return 1;
}
