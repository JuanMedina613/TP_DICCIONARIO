#include "ProcesadorTexto.h"

int main()
{
    //system("chcp 65001 >nul");
    tDiccionario dic;
    crear_dic(&dic, 5);

    menu(&dic);

    return 0;
}
