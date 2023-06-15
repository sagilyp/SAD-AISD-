#include "table.h"
#include "dialog.h"
#include "struct.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
	Table table = {NULL, 0, 0};
    //#ifdef LAB_D                      
    	int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_ReleaseFind,D_Delete, D_ReleaseDelete, D_Show};
        const char *msgs[] = {"0. Quit", "1. Add", "2. Find by key", "3. Find by key with release","4. Delete",
                                                    "5. Delete with release", "6. Show"};  
        if (D_Load(&table) == 0)// загружаем таблицу, подготавливаем к работе файл данных
        	return 1;                                                                
	//#endif
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int rc;
    while (rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&table))
            break; // обнаружен конец файла
    }
    printf("That's all. Bye!\n");
    //#ifdef LAB_D
    	D_Save(&table); // выгружаем таблицу, закрываем файл данных
    //#endif	
    return 0;
}
