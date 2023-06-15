#ifndef STRCT
#define STRCT

//#ifdef LAB_C

typedef struct KeySpace{
    unsigned short busy;
    char *key; // ключ
    unsigned short release; // номер версии
    char *info; // указатель на информацию
}KeySpace;

typedef struct Table{
    KeySpace *ks; // массив пр-ва ключей
    int msize; // размер вектора
    int csize; //заполеннность таблицы
}Table;

//#endif


#endif
