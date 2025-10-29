#include "ProcesadorTexto.h"

int main()
{
    tDiccionario dic;
    crear_dic(&dic, 5);

    menu(&dic);

    return 0;
}
