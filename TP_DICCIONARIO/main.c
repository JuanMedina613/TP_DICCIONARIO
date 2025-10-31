#include "ProcesadorTexto.h"

int main()
{
    //system("chcp 65001 >nul");
    tDiccionario dic;
    crear_dic(&dic, CAPACIDAD_INICIAL);

    menu(&dic);

    return 0;
}
