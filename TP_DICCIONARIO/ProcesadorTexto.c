#include "ProcesadorTexto.h"

void sumarValoresDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    size_t * acum = (size_t *)destino;
    char *c = elemento->clave;
    if (isalpha(*c))
    {
        (*acum) += *(size_t *)(elemento->valor);
    }
    else
        if(*c == '-' && isalpha(*(c+1)))
            (*acum) += *(size_t *)(elemento->valor);

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

    if(!pd || !pd->lista) // verifico
        return VACIO;

    for(; ini < pd->lista + pd->capacidad; ini++)
    {
        listaRecorrer(ini, sumarEspaciosDic, &acum);
    }
    return acum;

}
///================================================================================================================================///
size_t contarApariciones_de_una_Palabra(tDiccionario* pd, const char* palabra)
{
    size_t pos = hashDiccionario(palabra) % pd->capacidad;
    tLista* lista = pd->lista + pos;
    tNodo* nodo = *lista;
    sDato* dato;

    if (*lista == NULL) // Si la lista esta vacia, la palabra no esta
        return NO_ENCONTRADA;

    //Recorro los nodos de esa lista
    while (nodo != NULL)
    {
        dato = (sDato*)nodo->info;

        if (strcmp(dato->clave, palabra) == 0)
            return *(size_t*) dato->valor;// encontré la palabra, devuelvo el valor
        nodo = nodo->sig;                 // sigo buscando
    }
    return NO_ENCONTRADA;                 // no se encontró la palabra
}
///================================================================================================================================///
size_t contarSignos(tDiccionario *pd)
{
    size_t acum = 0;
    size_t aux;

    if(!pd || !pd->lista)
    {
        return VACIO; //esta Vacio
    }

    for(aux = 0; aux < pd->capacidad; aux++)                  // Recorrer el vector principal
    {
        listaRecorrer(pd->lista + aux, sumarSignosDic, &acum);//Recorremos las Listas sumando los simbolos
    }
    return acum;
}
///================================================================================================================================///
void sumarSignosDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    size_t *acum = (size_t *)destino;

    char *c = (char *)elemento->clave; //Asumimos que la clave es un solo caracter (signo o espacio)

    if (!isalpha(*c) && *(c+1)=='\0' && *c != ' ')     // Si la clave NO es una letra Y NO es un espacio, la consideramos un "signo" o caracter especial
    {
        (*acum) += *(size_t *)(elemento->valor); // Sumar el contador de apariciones de este signo
    }
}
///================================================================================================================================///
int sumarPalabra(tDiccionario *pd, char* clave)
{
    size_t contador = 1;

    if(!clave)
        return ERROR1;

    if(obtener_dic(pd,&contador,sizeof(size_t),clave,cmpClaveBusqueda) == TODO_OKEY)
        contador += 1;

    if(poner_dic(pd,&contador,sizeof(size_t), clave) != TODO_OKEY)
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
    }
    while(nroarch>4 || nroarch<1);
    switch(nroarch)
    {
    case 1:
        strcpy(nombreArchivo, "1.txt");
        break;
    case 2:
        strcpy(nombreArchivo, "2.txt");
        break;
    case 3:
        strcpy(nombreArchivo, "3.txt");
        break;
    case 4:
        strcpy(nombreArchivo, "4.txt");
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
    char *dir = linea;
    quitarEspeciales(linea);

    while(*dir != '\0' && *dir != '\n')
    {
        if(*dir != '-' && !isalpha(*dir))
        {
            *(dato->clave) = *dir;
            *(dato->clave + 1) = '\0';
            sumarPalabra(pd,dato->clave);
            *dir = '\0';
            if(dir != linea)
            {
                sumarPalabra(pd, linea);
            }
            linea = dir+1;
        }

        dir++;
    }

    if(*dir == '\n')
        *dir = '\0';
    sumarPalabra(pd, linea);
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


    printf("--- PROCESADOR DE TEXTO V1.0 ---\n");
    if(seleccionarArchivo(pd) != TODO_OKEY) //Ingresar y cargar el archivo
    {
        printf("\nError fatal: No se pudo cargar el archivo. Saliendo del programa.\n");
        return;
    }

    printf("\nArchivo cargado y diccionario poblado con exito.\n");
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
            opcion = -1;
            while(getchar() != '\n');
        }

        system("cls");
        switch(opcion)
        {
        case 1:
            printf("\n--- Estadisticas Generales ---\n");
            total_espacios = contarEspacios(pd);
            total_palabras = contarPalabras(pd);
            total_signos = contarSignos(pd);
            printf("Cantidad total de palabras: %u\n", (unsigned int)total_palabras);
            printf("Cantidad total de espacios: %u\n", (unsigned int)total_espacios);
            printf("Cantidad total de signos y caracteres especiales: %u\n", (unsigned int)total_signos);
            printf("Cantidad total de colisiones: %u\n", (unsigned int)pd->cantColisiones);

            break;

        case 2:
            printf("\n--- Listado de Apariciones (Recorrido del Diccionario) ---\n");
            recorrer_dic(pd,imprimirDato);

            break;

        case 3:
            printf("\n--- Busqueda de Palabra Especifica ---\n");
            printf("Ingrese la palabra a buscar: ");
            scanf("%s", palabra_busqueda);

            if((aparicionesPalabra = contarApariciones_de_una_Palabra(pd,quitarEspeciales(palabra_busqueda))) != 0 && (isalpha(*(int*)palabra_busqueda) || *palabra_busqueda == '-' ))
                printf("La palabra \"%s\" aparece %u veces.\n", palabra_busqueda, (unsigned int)aparicionesPalabra);
            else
            {
                if(!isalpha(*(int*)palabra_busqueda))
                    printf("El Simbolo o la Palabra con Simbolo \"%s\" No se Puede Mostrar.\n",palabra_busqueda);
                else
                    printf("La Palabra \"%s\" No Aparece en el Texto.\n",palabra_busqueda);
            }
            break;

        case 0:
            printf("\nSaliendo del procesador de texto. Destruyendo diccionario...\n");
            destruir_dic(pd,liberar_dato_dic);
            break;

        default:
            printf("\nOpcion inválida. Intente de nuevo.\n");
            break;
        }
        printf("\n\n");
        system("pause");
        system("cls");
    }
    while(opcion != 0);
}
//**Funcion de Mostrar**//
void imprimirDato(void *DatoDiccionario)
{
    sDato* aux = (sDato*)DatoDiccionario;
    char* linea = aux->clave;

    if((*(linea + 1)) != '\0' && *linea == '-')
        printf("\n'%15s' esta Palabra se repitio %4d",aux->clave,*(int*)aux->valor);
    if (!isalpha(*(int*)aux->clave))
        return;
    printf("\n'%15s' esta Palabra se repitio %4d",aux->clave,*(int*)aux->valor);
}
///================================================================================================================================///
char *quitarEspeciales(char *palabra)
{
    if (!palabra) return NULL;

    unsigned char *src = (unsigned char *)palabra;
    char *dst = palabra;
    size_t len = strlen(palabra);

    if (len >= 3 && src[0] == 0xEF && src[1] == 0xBB && src[2] == 0xBF)
    {
        memmove(palabra, palabra + 3, len - 2);
        src = (unsigned char *)palabra;
        len -= 3;
    }

    size_t i = 0;
    while (i < len)
    {
        unsigned char c = src[i];

        if ((c == 0xC2 || c == 0xC3) && (i + 1 < len))
        {
            unsigned char n = src[i + 1];

            if (c == 0xC2 && n == 0xBF)
            {
                *dst++ = '?';    /* ¿ */
            }
            else if (c == 0xC2 && n == 0xA1)
            {
                *dst++ = '!';    /* ¡ */
            }
            else if (c == 0xC3 && n == 0xA1)
            {
                *dst++ = 'a';    /* á */
            }
            else if (c == 0xC3 && n == 0x81)
            {
                *dst++ = 'A';    /* Á */
            }
            else if (c == 0xC3 && n == 0xA9)
            {
                *dst++ = 'e';    /* é */
            }
            else if (c == 0xC3 && n == 0x89)
            {
                *dst++ = 'E';    /* É */
            }
            else if (c == 0xC3 && n == 0xAD)
            {
                *dst++ = 'i';    /* í */
            }
            else if (c == 0xC3 && n == 0x8D)
            {
                *dst++ = 'I';    /* Í */
            }
            else if (c == 0xC3 && n == 0xB3)
            {
                *dst++ = 'o';    /* ó */
            }
            else if (c == 0xC3 && n == 0x93)
            {
                *dst++ = 'O';    /* Ó */
            }
            else if (c == 0xC3 && n == 0xBA)
            {
                *dst++ = 'u';    /* ú */
            }
            else if (c == 0xC3 && n == 0x9A)
            {
                *dst++ = 'U';    /* Ú */
            }
            else if (c == 0xC3 && n == 0xBC)
            {
                *dst++ = 'u';    /* ü */
            }
            else if (c == 0xC3 && n == 0x9C)
            {
                *dst++ = 'U';    /* Ü */
            }
            else if (c == 0xC3 && n == 0xB1)
            {
                *dst++ = 'n';    /* ñ */
            }
            else if (c == 0xC3 && n == 0x91)
            {
                *dst++ = 'N';    /* Ñ */
            }
            else if (n >= 0x20 && n <= 0x7E)
            {
                *dst++ = (char)n;
            }
            else
            {
                *dst++ = '?';
            }

            i += 2; /* avanzamos dos bytes */
        }
        else if (c >= 0xC0)
        {
            if (c == 0xE1) *dst++ = 'a'; /* á */
            else if (c == 0xC1) *dst++ = 'A'; /* Á */
            else if (c == 0xE9) *dst++ = 'e'; /* é */
            else if (c == 0xC9) *dst++ = 'E'; /* É */
            else if (c == 0xED) *dst++ = 'i'; /* í */
            else if (c == 0xCD) *dst++ = 'I'; /* Í */
            else if (c == 0xF3) *dst++ = 'o'; /* ó */
            else if (c == 0xD3) *dst++ = 'O'; /* Ó */
            else if (c == 0xFA) *dst++ = 'u'; /* ú */
            else if (c == 0xDA) *dst++ = 'U'; /* Ú */
            else if (c == 0xFC) *dst++ = 'u'; /* ü */
            else if (c == 0xDC) *dst++ = 'U'; /* Ü */
            else if (c == 0xF1) *dst++ = 'n'; /* ñ */
            else if (c == 0xD1) *dst++ = 'N'; /* Ñ */
            else if (c == 0xBF) *dst++ = '?'; /* ¿ */
            else *dst++ = (char)c;

            i += 1;
        }
        else
        {
            *dst++ = (char)c;
            i += 1;
        }
    }

    *dst = '\0';
    return palabra;
}
