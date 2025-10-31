#include "ProcesadorTexto.h"

void sumarValoresDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    size_t * acum = (size_t *)destino;
    char c = *elemento->clave;
    if (miEsAlpha((int)c))
    {
        (*acum) += *(size_t *)(elemento->valor);
    }
}
///================================================================================================================================///
void sumarEspaciosDic(void *dato, void *destino)
{
    sDato *elem = (sDato*)dato;
    size_t * acum = (size_t*)destino;

    if (elem->clave && strcmp(elem->clave, " ") == 0) // en la primer condicion verifico que exista la clave, luego comparo si es un espacio
        (*acum) += *(size_t *)(elem->valor);
}

///================================================================================================================================///
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
///================================================================================================================================///
size_t contarEspacios(tDiccionario *pd)
{
    size_t acum = 0;
    tLista *ini = pd->lista;

    if(!pd || !pd->lista) // verifico que la lista no este vacia
        return VACIO; // si no hay nada, retorno error

    for(; ini < pd->lista + pd->capacidad; ini++)
    {
        listaRecorrer(ini, sumarEspaciosDic, &acum);
    }
    return acum;

}
///================================================================================================================================///
void Listado_contarApariciones_de_Palabras(tDiccionario* pd)
{
    tLista *ini = NULL;
    tLista *fin = NULL;
    tNodo *nodo = NULL;
    sDato *dato = NULL;

    ini = pd->lista;
    fin = pd->lista + pd->capacidad;

    while(ini < fin)
    {
        nodo=*ini;
        while(nodo != NULL)
        {
            dato=(sDato*)nodo->info;
            printf("\n'%15s' Se Repite un Total de %3zu veces", dato->clave, *(size_t*) dato->valor);
            nodo = nodo->sig;
        }

        ini++;
    }
}
///================================================================================================================================///
int contarApariciones_de_una_Palabra(tDiccionario* pd, const char* palabra)
{
    size_t pos = hashDiccionario(palabra) % pd->capacidad;
    tLista* lista = pd->lista + pos;

    // Si la lista esta vacia, la palabra no esta
    if (*lista == NULL)
        return NO_ENCONTRADA;

    // Recorro los nodos de esa lista
    tNodo* nodo = *lista;
    while (nodo != NULL)
    {
        sDato* dato = (sDato*)nodo->info;

        if (strcmp(dato->clave, palabra) == 0)
            return *(size_t*) dato->valor;   // encontré la palabra, devuelvo el valor
        nodo = nodo->sig;  // sigo buscando
    }
    return NO_ENCONTRADA;  // no se encontró la palabra
}
///================================================================================================================================///
size_t contarSignos(tDiccionario *pd)
{
    size_t acum = 0;
    size_t aux;

    if(!pd || !pd->lista)
    {
        return 0; // Se retorna 0 si está vacío
    }

    // Recorrer el vector principal
    for(aux = 0; aux < pd->capacidad; aux++)
    {
        // Llamar a listaRecorrer con la acción que suma solo los signos
        listaRecorrer(pd->lista + aux, sumarSignosDic, &acum);
    }

    return acum;
}
///================================================================================================================================///
void sumarSignosDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    size_t *acum = (size_t *)destino;

    // Asumimos que la clave es un solo caracter (signo, espacio o letra)
    char c = *(char *)elemento->clave;

    // Si la clave NO es una letra Y NO es un espacio, la consideramos un "signo" o caracter especial
    if (!miEsAlpha(c) && c != ' ')
    {
        // Sumar el contador de apariciones de este signo
        (*acum) += *(size_t *)(elemento->valor);
    }
    //printf("\nPalabra %s valor %zu", elemento->clave, *(size_t *)elemento->valor);
}
///================================================================================================================================///
int sumarPalabra(tDiccionario *pd, const char* clave)
{
    size_t contador = 1;
    sDato aux;

    if(!clave)
        return ERROR1;

    //printf("\nLa palabra es: %s", clave);
    if(obtener_dic(pd,&aux,sizeof(sDato),clave,cmpClaveBusqueda) == TODO_OKEY)
        contador = *(size_t *)aux.valor + 1;

    if(poner_dic(pd,&contador,sizeof(size_t),clave) != TODO_OKEY)
    {
        return ERROR1;
    }

    return TODO_OKEY;
}
///================================================================================================================================///
int seleccionarArchivo(tDiccionario*pd)
{
    FILE *pf;
    int nroarch,res;
    char nombreArchivo[20];

    do
    {
        printf("Seleccione numero de archivo(del 1 al 4): ");
        scanf("%d",&nroarch);
        if(nroarch>4 || nroarch<1)
            printf("\nNumero de archivo invalido, ingrese un numero valido(del 1 al 4):\n");
    }while(nroarch>4 || nroarch<1);
    switch(nroarch)
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
///================================================================================================================================///
int cargarArchivoEnDiccionario(tDiccionario* pd,FILE *pf)
{
    char linea[MAXLINE];
    sDato aux;

    aux.tam = sizeof(size_t);
    if(!(aux.clave = (char *)malloc(MAXLINE)) || !(aux.valor = malloc(sizeof(size_t))))
    {
        free(aux.clave);
        free(aux.valor);
        return SIN_MEM;
    }

    *(size_t*)aux.valor = 1;
    //printf("\n============-- TEXTO --============");
    while(fgets(linea,MAXLINE,pf))
    {
       if(!TrozaryGuardarArchivo(linea,&aux,pd))
       {
           free(aux.clave);
           free(aux.valor);
           return ERROR1;
       }
    }
    free(aux.clave);
    free(aux.valor);
    return TODO_OK;
}
///================================================================================================================================///
int TrozaryGuardarArchivo(char *linea,sDato *dato,tDiccionario *pd)
{
    char *dir = NULL;

    //printf("\n%s", linea);

    dir = strchr(linea,'\n');
    if(!dir)
        return ERROR1;

    *dir = '\0';

    while(dir != linea)
    {

        while ((dir - 1) > linea && !miEsAlpha(*(dir - 1)))// si dir-1 no esta fuera de la linea(existe) y dir-1 no es un caracter y no es espacio vacio insertamos el caracter
            {
                dir = dir - 1;
                *(dato->clave) = *dir;
                *(dato->clave + 1) = '\0';
                //printf("\nSe suma %s un total de %d\n", dato->clave, *(size_t*)dato->valor);
                sumarPalabra(pd, dato->clave);

                *dir = '\0'; // una vez guardado el caracter, \0 para seguir trozando
            }
        if(!(dir = strrchr(linea,' ')))
        {
            dir = linea; // En caso de que retorne NULL porque no hay espacios, significa que llegamos a fin de cadena, por ende igualamos
            strcpy(dato->clave,dir); // copio la palabra
        }else
        {
            *(dato->clave) = *dir;
            *(dato->clave + 1) = '\0';
            //printf("\nSe suma %s un total de %d\n", dato->clave, *(size_t*)dato->valor);
            sumarPalabra(pd, dato->clave);
            strcpy(dato->clave,dir + 1); // copio la palabra
        }


        //printf("\nSe suma %s un total de %d\n", dato->clave, *(size_t*)dato->valor);
        sumarPalabra(pd, dato->clave);

        *dir = '\0'; // continuo poniendo \0
        // El if se hace ya que, si estamos parados al principio no se puede hacer dir + 1 ya que nos comeriamos una letra
     }

    return 1;
}
///================================================================================================================================///
void menu(tDiccionario *pd)
{
    int opcion = 0;
    size_t total_palabras;
    size_t total_signos;
    size_t total_espacios;
    size_t aparicionesPalabra;
    char palabra_busqueda[MAXLINE];

    // Paso 1: Ingresar y cargar el archivo
    printf("--- PROCESADOR DE TEXTO V1.0 ---\n");
    if(seleccionarArchivo(pd) != TODO_OKEY)
    {
        printf("\nError fatal: No se pudo cargar el archivo. Saliendo del programa.\n");
        return;
    }

    printf("\nArchivo cargado y diccionario poblado con exito.\n");

    // Bucle del menú
    do
    {
        printf("\n============================================\n");
        printf("               MENU DE ANALISIS\n");
        printf("============================================\n");
        printf("1. Mostrar estadisticas generales (Palabras, Signos, y Espacios)\n");
        printf("2. Mostrar listado de apariciones por palabra (Recorrer Diccionario)\n");
        printf("3. Buscar apariciones de una palabra en particular\n");
        printf("0. Salir\n");
        printf("Ingrese una opcion: ");

        if(scanf("%d", &opcion) != 1)
        {
            opcion = -1; // Forzar un valor inválido si la entrada no es un número
            while(getchar() != '\n'); // Limpiar el buffer de entrada
        }

        system("cls");
        switch(opcion)
        {
            case 1:
                printf("\n--- Estadisticas Generales ---\n");
                total_espacios = contarEspacios(pd);
                total_palabras = contarPalabras(pd);
                total_signos = contarSignos(pd);
                printf("Cantidad total de palabras: %zu\n", total_palabras);
                printf("Cantidad total de espacios: %zu\n", total_espacios);
                printf("Cantidad total de signos y caracteres especiales: %zu\n", total_signos);

                break;

            case 2:
                printf("\n--- Listado de Apariciones (Recorrido del Diccionario) ---\n");
                Listado_contarApariciones_de_Palabras(pd);
                // Debes implementar una función de acción que imprima:
                // void imprimirDato(void *DatoDiccionario, void *p_extra)
                // y luego llamar: recorrer_dic(pd, imprimirDato);

                break;

            case 3:
                printf("\n--- Busqueda de Palabra Especifica ---\n");
                printf("Ingrese la palabra a buscar: ");
                scanf("%s", palabra_busqueda);
                // Aquí usarías obtener_dic para buscar la clave
                // Necesitas una variable 'int apariciones_count = 0;'
                //obtener_dic(pd, &apariciones_count, sizeof(int), palabra_busqueda, cmpClaveBusqueda);
                if((aparicionesPalabra = contarApariciones_de_una_Palabra(pd,palabra_busqueda)) != 0)
                    printf("La palabra \"%s\" aparece %zu veces.\n", palabra_busqueda, aparicionesPalabra);
                else
                    printf("La Palabra \"%s\" No Aparece en el Texto.\n",palabra_busqueda);
                break;

            case 0:
                printf("\nSaliendo del procesador de texto. Destruyendo diccionario...\n");
                destruir_dic(pd);
                break;

            default:
                printf("\nOpcion inválida. Intente de nuevo.\n");
                break;
        }
        printf("\n\n");
        system("pause");
        system("cls");
    }while(opcion != 0);
}
///================================================================================================================================///
int miEsAlpha(int c)
{
    // Letras normales A-Z / a-z
    if (isalpha(c))
        return 1;

    // Vocales acentuadas básicas (á, é, í, ó, ú y sus mayúsculas)
    switch (c)
    {
        case 0xE1: case 0xC1: // á Á
        case 0xE9: case 0xC9: // é É
        case 0xED: case 0xCD: // í Í
        case 0xF3: case 0xD3: // ó Ó
        case 0xFA: case 0xDA: // ú Ú
        case 0xF1: case 0xD1:
            return 1;
        default:
            return 0;
    }
}
