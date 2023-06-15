#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
//#include <graphviz/cgraph.h>

#include "headers/dialog.h"


int main() {
    int (*fptr[])(Node **) = {NULL, D_Add, D_Find, D_Delete, D_MinFind, D_Show, D_Image, D_Bypass, D_Import, D_Timing};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Find the smallest",
                          "5. Show", "6. Image", "7.Bypass", "8. Import from txt", "9. Timing"};
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
