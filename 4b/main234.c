#include <stdio.h>

#include "headers/234.h"
#include "headers/dialog234.h"

int main() {
    int (*fptr[])(Node **) = {NULL, D_Add, D_Find, D_Delete, D_FindPrev, D_Show, D_Bypass, D_Export, D_Timing};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Find the largest previous",
                          "5. Show", "6. Bypass", "7. Export from txt", "8. Timing"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    Node *aspen = NULL;
    int rc;
    while(rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&aspen))
            break; // обнаружен конец файла
    }
    printf("That's all. Bye!\n");
    delTree(&aspen);
    return 0;
}
