#ifndef AVC
#define AVC

#include "type.h"
/*
//#ifdef LAB_A
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
//#endif

#ifdef LAB_B
typedef struct Table{ // тип - просматриваемая таблица-вектор
    int msize; // размер вектора
    int csize; // размер таблицы
    FILE *fd; // дескриптор файла, чтобы выполнять операции с файлом
}Table;
#endif*/

 int GetInt(int *a);
 char *GetStr();
 int dialog(const char *msgs[], int NMsgs);
 int k_check(Table *ptab, char *key);
 int p_check(Table *ptab, char *pkey);
 int p_find(Table *ptab, char *pkey, int ern);
 void t_shift_r(Table *ptab, int pos);                                                                                                        
 void t_shift_l(Table *ptab, int pos);                                                                                                        
 int f_insert(Table *ptab, char *pkey);
 int add(Table *ptab, char *k, char *pk, unsigned int *info);
 int make_par_table(Table *ptab, int pos, char *pkey);
 int swap(Table *ptab, int pos1, int pos2);
 int delete(Table *ptab, int k, char *key);                                                                                                                  
 int findInfo(Table *ptab, char *key, unsigned int *info);
 int show(Table *ptab);

// #ifdef LAB_A
  void delTable(Table *ptab);
  void delete_tail(Table *ptab, int pos);
  void in_tail(Table *ptab, int pos);
// #endif
           


#endif
