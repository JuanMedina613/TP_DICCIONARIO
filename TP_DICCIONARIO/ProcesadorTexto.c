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
void sumarEspaciosDic(void *dato, void *destino)
{
    sDato *elem = (sDato*)dato;
    int * acum = (int*)destino;

    if (elem->clave && strcmp(elem->clave, " ") == 0) // en la primer condicion verifico que exista la clave, luego comparo si es un espacio
        (*acum) += *(int *)(elem->valor);
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
size_t contarEspacios(tDiccionario *pd)
{
    size_t acum = 0;
    size_t aux;

    if(!pd || !pd->lista) // verifico que la lista no este vacia
        return VACIO; // si no hay nada, retorno error

    for(aux = 0; aux < pd->capacidad; aux++)
    {
        listaRecorrer(pd->lista + aux, sumarEspaciosDic, &acum);
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
///===============================================================================//
void sumarSignosDic(void *DatoDiccionario, void *destino)
{
    sDato *elemento = (sDato *)DatoDiccionario;
    size_t *acum = (size_t *)destino;

    // Asumimos que la clave es un solo caracter (signo, espacio o letra)
    char c = *(char *)elemento->clave;

    // Si la clave NO es una letra Y NO es un espacio, la consideramos un "signo" o caracter especial
    if (!isalpha(c) && c != ' ')
    {
        // Sumar el contador de apariciones de este signo
        (*acum) += *(int *)(elemento->valor);
    }
    //printf("\nPalabra %s valor %zu", elemento->clave, *(int *)elemento->valor);
}
///===============================================================================//
int seleccionarArchivo(tDiccionario*pd)
{
    FILE *pf;
    int nroarch,res;
    char nombreArchivo[20];

    do
    {
        printf("Seleccione numero de archivo(del 1 al 4):\n");
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
///===============================================================================//
int cargarArchivoEnDiccionario(tDiccionario* pd,FILE *pf)
{
    char linea[MAXLINE];
    sDato aux;

    if(!(aux.clave = malloc(MAXLINE)) || !(aux.valor = malloc(sizeof(int))))
    {
        free(aux.clave);
        free(aux.valor);
        return ERROR1;
    }

    *(int*)aux.valor = 1;

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
///===============================================================================//
int TrozaryGuardarArchivo(char *linea,sDato *dato,tDiccionario *pd)
{
    char *dir;
    printf("\n%s", linea);
    dir = strchr(linea,'\n');
    if(!dir)
        return ERROR1;

    *dir = '\0';

    while(dir != linea)
    {

        while ((dir - 1) > linea && !isalpha(*(dir - 1)))// si dir-1 no esta fuera de la linea(existe) y dir-1 no es un caracter y no es espacio vacio insertamos el caracter
            {
                dir = dir - 1;
                *(dato->clave) = *dir;
                *(dato->clave + 1) = '\0';
                dato->tam = sizeof(char);
                poner_dic(pd,dato->valor,sizeof(int),dato->clave,actValorSumar); // guardamos el caracter para no perderlo

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
            dato->tam = sizeof(char);
            poner_dic(pd,dato->valor,sizeof(int),dato->clave,actValorSumar);

            strcpy(dato->clave,dir + 1); // copio la palabra
        }


        dato->tam = strlen(dato->clave);
        poner_dic(pd,dato->valor,dato->tam,dato->clave,actValorSumar); // guardo en diccionario para no perder la palabra

        *dir = '\0'; // continuo poniendo \0
        // El if se hace ya que, si estamos parados al principio no se puede hacer dir + 1 ya que nos comeriamos una letra
     }

    return 1;
}
///===============================================================================//
void menu(tDiccionario *pd)
{
    int opcion = 0;
    size_t total_palabras;
    size_t total_signos;
    char palabra_busqueda[MAXLINE];
    int* apariciones;

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
        printf("1. Mostrar estadisticas generales (Palabras y Signos)\n");
        printf("2. Mostrar listado de apariciones por palabra (Recorrer Diccionario)\n");
        printf("3. Buscar apariciones de una palabra en particular\n");
        printf("0. Salir\n");
        printf("Ingrese una opción: ");

        if(scanf("%d", &opcion) != 1)
        {
            opcion = -1; // Forzar un valor inválido si la entrada no es un número
            while(getchar() != '\n'); // Limpiar el buffer de entrada
        }

        switch(opcion)
        {
            case 1:
                printf("\n--- Estadísticas Generales ---\n");
                // Implementar función para contar espacios si se necesita, sino se puede omitir
                total_palabras = contarPalabras(pd);
                total_signos = contarSignos(pd);

                printf("Cantidad total de palabras: %zu\n", total_palabras);
                printf("Cantidad total de signos y caracteres especiales: %zu\n", total_signos);
                break;

            case 2:
                printf("\n--- Listado de Apariciones (Recorrido del Diccionario) ---\n");
                // Debes implementar una función de acción que imprima:
                // void imprimirDato(void *DatoDiccionario, void *p_extra)
                // y luego llamar: recorrer_dic(pd, imprimirDato);
                printf("Funcion recorrer_dic y la accion de impresion pendientes de implementacion.\n");
                break;

            case 3:
                printf("\n--- Busqueda de Palabra Especifica ---\n");
                printf("Ingrese la palabra a buscar: ");
                scanf("%s", palabra_busqueda);
                // Aquí usarías obtener_dic para buscar la clave
                // Necesitas una variable 'int apariciones_count = 0;'
                // obtener_dic(pd, &apariciones_count, sizeof(int), palabra_busqueda, cmpClaveBusqueda);
                printf("Funcion obtener_dic pendiente de implementacion.\n");
                break;

            case 0:
                printf("\nSaliendo del procesador de texto. Destruyendo diccionario...\n");
                destruir_dic(pd);
                break;

            default:
                printf("\nOpcion inválida. Intente de nuevo.\n");
                break;
        }
    }while(opcion != 0);
}
