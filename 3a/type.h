#ifndef ABC
#define ABC


typedef struct KeySpace{
    char *key; //ключ элемента
    char *par; // родительский ключ(м.б. NULL)
    unsigned int *info; // нуль- терминированная строка
}KeySpace;

typedef struct Table{
    KeySpace *ks; // массив пр-ва ключей
    int msize; // размер вектора
    int csize; //заполеннность таблицы
}Table;



#endif
