#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dialog.h"
#include "table.h"
//#include "type.h"
/*int main() {
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_Make, D_MPtable};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find by key", "3. Delete", "4. Show", "5. Make", "6. Find by parent's key" };
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    Table table = {NULL, 0, 0};
    int rc;
    while (rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&table))
            break; // обнаружен конец файла
    }
    printf("That's all. Bye!\n");
    delTable(&table);
    return 0;
}*/

int main() {
    Table table = {NULL, 0, 0};
  //  #ifdef LAB_B
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_MPtable};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find by key", "3. Delete", "4. Show", "5. Find by parent's key" };
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    if (D_Load(&table) == 0)// загружаем таблицу, подготавливаем к работе файл данных
        return 1;
  //  #endif
    int rc;
    while (rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&table))
            break; // обнаружен конец файла
    }
    printf("That's all. Bye!\n");
  //  #ifdef LAB_B
    D_Save(&table); // выгружаем таблицу, закрываем файл данных
   // #endif
    return 0;
}
