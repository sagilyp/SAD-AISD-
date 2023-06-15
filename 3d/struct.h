#ifndef STRCT
#define STRCT


//#ifdef LAB_D
#include <stdio.h>
typedef struct Table{ // тип - просматриваемая таблица-вектор
    FILE *fd; // дескриптор файла, чтобы выполнять операции с файлом
    int msize; // размер вектора
    int csize; // размер таблицы
}Table;

//#endif

#endif
