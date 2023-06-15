#ifndef ABC
#define ABC


//#ifdef LAB_B
typedef struct Table{ // тип - просматриваемая таблица-вектор
	FILE *fd; // дескриптор файла, чтобы выполнять операции с файлом
    int msize; // размер вектора
    int csize; // размер таблицы
}Table;
//#endif

#endif
