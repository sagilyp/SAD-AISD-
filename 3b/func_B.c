#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "type.h"
//int find(Table ptab, int k)
//{
//    int i = 0;
//    for(; i < ptab.n; ++i)
//        if(ptab.first[i].key == k)
//            return i;
//    return -1;
//}

// функция проверки, есть ли такой ключ вообще
// возвращает смещение ключа отн-но начала файла
// в случае неудачи -1
int k_check(Table *ptab, char *key){
    for(int i = 0; i < ptab->csize; ++i){
        int flag = 0;
        fseek(ptab->fd, (2+i*3)*sizeof(int) , SEEK_SET );
        int offset = 0;
        int cnt;
        char a;
        fread(&offset, sizeof(int), 1, ptab->fd);
        fseek(ptab->fd, offset, SEEK_SET);
        fread(&cnt, sizeof(int), 1, ptab->fd);
        if(cnt == (int)strlen(key)) {
            for (int j = 0; j < cnt; ++j) {
                fread(&a, sizeof(char), 1, ptab->fd);
                if (a == key[j]) {
                    ++flag;
                }
            }
        }
        if(flag == cnt){
            return (2+i*3);
        }
    }
    return -1;
}

// поиск информации по ключу(не родитель)
int findInfo(Table *ptab, char * key, unsigned int *info)
{
    int i = k_check(ptab, key);
    if (i >= 0){
        fseek(ptab->fd, (i+2)*sizeof(int), SEEK_SET);
        fread(info, sizeof(unsigned int), 1, ptab->fd);
        return 1;
    }
    return 0;
}


// функция проверки, есть ли такой ключ, совпадающий с родительским
// возвращает смещение строки отн-но начала
int p_check(Table *ptab, char *par){
   // fseek(ptab->fd, 2*sizeof(int) , SEEK_SET );
   if(par == NULL){
       return  1;
   }
    for(int i = 0; i < ptab->csize; ++i){
        int flag = 0;
        fseek(ptab->fd, (2+i*3)*sizeof(int) , SEEK_SET );
        int offset = 0;
        int cnt = 0;
        char a;
        fread(&offset, sizeof(int), 1, ptab->fd);
        fseek(ptab->fd, offset, SEEK_SET);
        fread(&cnt, sizeof(int), 1, ptab->fd);
        if(cnt == (int)strlen(par)) {
            for (int j = 0; j < cnt; ++j) {
                fread(&a, sizeof(char), 1, ptab->fd);
                if (a == par[j]) {
                    ++flag;
                }
            }
        }
        if(flag == cnt){
            return (2+i*3);
        }
    }
    return 0;
}


// функция поиска ключа по родителям, совпадающего с заданным родителем
// возвращает смещение отн-но начала файла
// для ключа key обычного: если par == NULL, то возвращает
// смещение ключа(не родителя) с таким значением
int p_find(Table *ptab, char *par, int ern){
    static int pos;
    if(ern != 0){
        ern = pos;
    }
    for(int i = ern; i < ptab->csize; ++i){
        int flag = 0;
        fseek(ptab->fd, (3 + i*3)*sizeof(int) , SEEK_SET );
        int offset = 0;
        int cnt;
        char a;
        fread(&offset, sizeof(int), 1, ptab->fd);
        if(offset != -1) {
            fseek(ptab->fd, offset, SEEK_SET);
            fread(&cnt, sizeof(int), 1, ptab->fd);
            if(par != NULL && cnt == (int)strlen(par)) {
                for (int j = 0; j < cnt; ++j) {
                    fread(&a, sizeof(char), 1, ptab->fd);
                    if (a == par[j]) {
                        ++flag;
                    }
                }
            }
        }
        else{
            if(par == NULL){
                cnt = flag;
               // puts("\t\tLOH");
            }    
            else
            	cnt = -1;
        }
        if(flag == cnt){
        	//puts("\t\t\t attention -- eq");
            pos = i + 1;
            return (2 + i*3);
        }
    }
    return -1;
}


// функция сдвига ключей файла вправо; ничего не возвращает
void t_shift_r(Table *ptab, int posit){
	//puts("....shift");
    for(int i = ptab->csize; i >= (posit+1)/3; --i){
        fseek(ptab->fd, (3*i - 1)*sizeof(int), SEEK_SET);
        int off_k1;
        int off_p1;
        int inf1;
        fread(&off_k1, sizeof(int), 1, ptab->fd);
        fread(&off_p1, sizeof(int), 1, ptab->fd);
        fread(&inf1, sizeof(unsigned int), 1, ptab->fd);
        fwrite(&off_k1, sizeof(int), 1, ptab->fd);
        fwrite(&off_p1, sizeof(int), 1, ptab->fd);
        fwrite(&inf1, sizeof(unsigned int), 1, ptab->fd);
    }
}

void t_shift_l(Table *ptab, int posit){
	//puts("....shift l");
    for(int i = (posit+1)/3 + 1; i <= ptab->csize; ++i){
        int off_k1;
        int off_p1;
        int inf1;
        fseek(ptab->fd, (3*i - 1)*sizeof(int), SEEK_SET);
        fread(&off_k1, sizeof(int), 1, ptab->fd);
        fread(&off_p1, sizeof(int), 1, ptab->fd);
        fread(&inf1, sizeof(int), 1, ptab->fd);
        fseek(ptab->fd, (3*(i-1) - 1)*sizeof(int), SEEK_SET);
        fwrite(&off_k1, sizeof(int), 1, ptab->fd);
        fwrite(&off_p1, sizeof(int), 1, ptab->fd);
        fwrite(&inf1, sizeof(int), 1, ptab->fd);
    }
}

//функция перемемны двух строк
// pos1 в pos2, pos2 в pos1
int swap(Table *ptab, int pos1, int pos2){
        if((pos1 > ((ptab->csize - 1) * 3) - 1 )|| (pos2 > ((ptab->csize) * 3) - 1)){
            return -1;
        }
        if(pos1 < 2 || pos2 < 5){
        	return -1;
        }	
        int off_k1, off_k2, off_p1, off_p2;
        unsigned int inf1, inf2;
        fseek(ptab->fd, pos1* sizeof(int), SEEK_SET);
        fread(&off_k1, sizeof(int), 1, ptab->fd);
        fread(&off_p1, sizeof(int), 1, ptab->fd);
        fread(&inf1, sizeof(unsigned int), 1, ptab->fd);
        fseek(ptab->fd, pos2* sizeof(int), SEEK_SET);
        fread(&off_k2, sizeof(int), 1, ptab->fd);
        fread(&off_p2, sizeof(int), 1, ptab->fd);
        fread(&inf2, sizeof(unsigned int), 1, ptab->fd);
        fseek(ptab->fd, pos1* sizeof(int), SEEK_SET);
        fwrite(&off_k2, sizeof(int), 1, ptab->fd);
        fwrite(&off_p2, sizeof(int), 1, ptab->fd);
        fwrite(&inf2, sizeof(unsigned int), 1, ptab->fd);
        fseek(ptab->fd, pos2* sizeof(int), SEEK_SET);
        fwrite(&off_k1, sizeof(int), 1, ptab->fd);
        fwrite(&off_p1, sizeof(int), 1, ptab->fd);
        fwrite(&inf1, sizeof(unsigned int), 1, ptab->fd);
        return 0;
}

void in_head(Table *ptab, int posit){
	int i = posit;
	while(swap(ptab, i-3, i) == 0){
		i -= 3;
	}
}

// функция сравниния строки и строки из файла(при помощи позиции)
// 0 - совпадают  -1 - строка меньше строки из файла   1 - строка больше строки из файла
int cmp(Table *ptab, char *pkey, int pos1){  //pos1 - смещение родителя отн-но начала
        //int flag = 0;
        fseek(ptab->fd, pos1*sizeof(int) , SEEK_SET );
        int offset = 0, res;
        int cnt = 0;
        char a;
        fread(&offset, sizeof(int), 1, ptab->fd);
        if(offset != -1) {
            fseek(ptab->fd, offset, SEEK_SET);
            fread(&cnt, sizeof(int), 1, ptab->fd);
            char *par = calloc(cnt+1, sizeof(char));
            for (int j = 0; j < cnt; ++j) {
                fread(&a, sizeof(char), 1, ptab->fd);
                par[j] = a;
            }
            par[cnt] = '\0';
            res = strcmp(pkey, par);
            free(par);
            return res;
        }
        else{
            if(pkey == NULL)
                return 0;
            else
                return 1;
        }
}


// функция поиска смещения отн-но начала файла для вставки
// т.к. таблица упорядочена по родителям
// ключи с родителем NULL идут в конец; возвращает смещение строки, в которой должна быть данная строка, остальное сдвигается
int f_insert(Table *ptab, char *pkey){
    if(pkey == NULL){
        return 3*ptab->csize + 2;
    }
    for(int i = 0; i < ptab->csize; ++i){
        if(cmp(ptab, pkey, 3 + i*3) <= 0){
            return 2 + i*3;
        }
    }
    return 3*ptab->csize + 2;
}


int add(Table *ptab, char *key, char *par, unsigned int *info)
{
    //printf("posit %d\n", posit);
    if(strcmp(par,"") == 0){
        //free(par);
        par = NULL;
    }
    if(k_check(ptab, key) >= 0)
        return 1; // поиск успешен - дублирование ключей
    if(ptab->csize >= ptab->msize)
        return 2; // переполнение таблицы
    if(ptab->msize == 0)
        return 5; // пустая таблица
    if(p_check(ptab, par) == 0) {
        return 3;//нет ключа ==  родителю
    }
    int posit = f_insert(ptab, par);
    int len_key = (int)strlen(key);
    int null = -1;
    int offset_par;
    int len_par;
    //ptab->first[ptab->n].key = k; // запись в таблицу ключа
    //ptab->first[ptab->n].len = strlen(str) + 1; // запись в таблицу размера информационного блока(включая нуль-байт)
    fseek(ptab->fd, 0, SEEK_END);// позиционирование на конец файла
    int offset_key = ftell(ptab->fd); // запись в таблицу смещения
                                                        // информацию о файле(по отношению к началу файла)
                                                       // запись информации в файл
    //printf("offset k %d\n", offset_key);
    //printf("len k %d\n", len_key);
    fwrite(&len_key, sizeof(int), 1, ptab->fd);
    fwrite(key, sizeof(char), strlen(key), ptab->fd);
    if(par != NULL) {
    	int check = p_check(ptab, par);
    //	printf("\tcheck %d\n ", check);
    	if(check == -1){ //ненаход
        	len_par = (int) strlen(par);
       	 	fseek(ptab->fd, 0, SEEK_END);
        	offset_par = ftell(ptab->fd);
        	fwrite(&len_par, sizeof(int), 1, ptab->fd);
        	fwrite(par, sizeof(char), strlen(par), ptab->fd);
        }
        else{
        	fseek(ptab->fd, (check)*sizeof(int), SEEK_SET);
        	fread(&offset_par, sizeof(int), 1, ptab->fd);
  			
        }	
        t_shift_r(ptab, posit);
        fseek(ptab->fd, posit*sizeof(int), SEEK_SET);
        fwrite(&offset_key, sizeof(int), 1, ptab->fd);
        fwrite(&offset_par, sizeof(int), 1, ptab->fd);
        fwrite(info, sizeof(unsigned int), 1, ptab->fd);
    }
    else{
      //  puts("par is NULL");
        t_shift_r(ptab, posit);
      //  puts("shift is sucessfull");
        fseek(ptab->fd, posit*sizeof(int), SEEK_SET);
        fwrite(&offset_key, sizeof(int), 1, ptab->fd);
     //   printf("null %d\n", null);
     //   printf("info %d\n", info);
        fwrite(&null, sizeof(int), 1, ptab->fd);
        fwrite(info, sizeof(unsigned int), 1, ptab->fd);
    }
   // puts("Succesful add");
    ++(ptab->csize); // изменение размера таблицы
    /* free(key);
    free(par);
    free(info); */
    return 0;
}


//выводит на экран таблицу, составленную из строк, в которых совпадают ключи
// родителей с заданным: исходная таблица не изменяется
int make_par_table(Table *ptab, int pos, char *par){  // pos = есть или нет род ключ совп с зад
    int offset_k;
    int set_k;
    int len;
    char a;
    if(ptab->csize == 0){
        return 5; //пустая таблица
    }
    if(pos == 0){
        return 3; // ключ не найден
    }
    offset_k = p_find(ptab, par, 0); // нашли смещение строки, в которой есть данный род. ключ
    while( offset_k != -1){
        char *key = NULL;
        unsigned int info;
        fseek(ptab->fd, offset_k*sizeof(int), SEEK_SET);
        fread(&set_k, sizeof(int), 1, ptab->fd);
        fseek(ptab->fd, sizeof(int), SEEK_CUR);
        fread(&info, sizeof(unsigned int), 1, ptab->fd);  // получена информация info
        fseek(ptab->fd, set_k, SEEK_SET);
        fread(&len, sizeof(int), 1, ptab->fd);
        key = calloc(len + 1, sizeof(char));
        for(int i = 0; i < len; ++i){
            fread(&a, sizeof(char), 1, ptab->fd);
            key[i] = a;
        }
        key[len] = '\0'; // получен ключ key
     //   printf("offset  %d\n", offset_k);
        printf("\t\t %s\t %s\t %u\t \n", key, par, info);
        free(key);
     //   puts("!!!");
        key = NULL;
        offset_k = p_find(ptab, par, 1);
       // puts("ёкарный бабай");
    }
    return 0;
}


void tab_mark(Table *ptab, int posit){
	int offset, len;
	fseek(ptab->fd, posit*sizeof(int), SEEK_SET);
	fread(&offset, sizeof(int), 1, ptab->fd);
	fseek(ptab->fd, offset, SEEK_SET);
	fread(&len, sizeof(int), 1, ptab->fd);
	if(len > 0){
		len = -len;
		fseek(ptab->fd, offset, SEEK_SET);
		fwrite(&len, sizeof(int), 1, ptab->fd);
	}
}

// функция удаления ключей из файла, все ключи сдвигаются влево отн-но данной позиции
// offset - смещение начала строки(то бишь длины строки) в байтах
// 0 - неудача, 1 - успех
//int rm_str(Table *ptab){
//	int len, offset, len2;
//	fseek(ptab->fd, (ptab->csize*3 +2)*sizeof(int), SEEK_SET);
//	while(feof(ptab->fd) == 0){
//		offset = ftell(ptab->fd);
//		fread(&len, sizeof(int), 1, ptab->fd);
//		if(len < 0){
//			len = -len;
//			fseek(ptab->fd, len*sizeof(char), SEEK_CUR);
//			fread(&len2, sizeof(int), 1, ptab->fd);
//			if(len2 < 0){
//				rm_str(ptab);
//			}
//			char str[len2];
//			fread(str, sizeof(char), len2, ptab->fd);
//			for(int i = 0; i < len2; ++i){
//				fread(&a, sizeof);
//			}	
//		}
//	}
//}

// удалить элемент по ключу key
// перед этим занулить все родительские ключи, в которых он встречается(то бишь поставить -1)
// возвращает коды ошибок: k - смещение в файле от начала отн-но key
int delete(Table *ptab, int k, char *key){
    if(ptab->csize == 0){
        return 5; //пустая таблица
    }
    if (k == -1) {
        return 3; // нет такого ключа
    }
    int offset_k = p_find(ptab, key, 0); // нашли смещение строки, в которой есть данный род. ключ
    int null = -1;
    while( offset_k != -1){
        fseek(ptab->fd, (1+offset_k)*sizeof(int), SEEK_SET);
        fwrite(&null, sizeof(int), 1, ptab->fd);
        in_head(ptab, offset_k); //в начало строку, где нашли искомый род ключ 
        offset_k = p_find(ptab, key, 1);
    }
    //fseek(ptab->fd, k*sizeof(int), SEEK_SET);
    //fwrite(&null, sizeof(int), 3, ptab->fd);
    k = k_check(ptab, key);
    tab_mark(ptab, k);
    t_shift_l(ptab, k);
    (ptab->csize)--;
    return 0;
}


//   Функция загрузки таблицы из файла в оперативную память. Используется один файл, в котором сохраняются
//   данные и таблица. Функция открывает файл на чтение и запись.
//   Файл остаётся открытым в тесение всего сеанса работы
int load (Table *ptab, char *fname){
// открываем существующий файл таблицы на чтение и запись
    ptab->fd = fopen(fname, "r+b");
    if (ptab->fd == NULL)
        return 0;
// файл открыт, можно читать: считываем размер вектора
    fread(&ptab->msize, sizeof(int), 1, ptab->fd);
// выделяем память под таблицу
 //   ptab->first = (Item *)calloc(ptab->msize, sizeof(Item));
// считываем размер таблицы
    fread(&ptab->csize, sizeof(int), 1, ptab->fd);
// считываем всю таблицу
    //fread(ptab->first, sizeof(Item), ptab->csize, ptab->fd);
    return 1;
}

//  функция создания новой таблицы. Создаётся пустая таблица размером sz и новый файл, в котором
//  резервируется место для таблицы. Используется один файл, в котором сохраняются и данные и таблица.
//  Файл остаётся открытым в течение всего сенса работы
int create(Table *ptab, char *fname, int sz){
// должны создать пустую таблицу размером sz, новый файл и зарезервировать в нем место для таблицы
    int temp = 0;
    ptab->msize = sz;
    ptab->csize = 0;
    ptab->fd = fopen(fname, "w+b");
    if(ptab->fd == NULL){
       // ptab->first = NULL;
        return 0;
    }
// выделяем место для таблицы и сразу заполняем ее нулями
 //   ptab->first = (Item *)calloc(ptab->SZ, sizeof(Item));
// записываем в начало файла размер вектора
    fseek(ptab->fd, 0, SEEK_SET);
    fwrite(&ptab->msize, sizeof(int), 1, ptab->fd);
// записываем размер таблицы
    fwrite(&ptab->csize, sizeof(int), 1, ptab->fd);
    for(int i = 0; i < 3*(ptab->msize); ++i){
    	fwrite(&temp, sizeof(int), 1, ptab->fd);
	}
// țзаписываем саму таблицу
   // fwrite(ptab->first, sizeof(Item), ptab->msize, ptab->fd);
    return 1;
}

int show(Table *ptab){
    int k_set, p_set, k_len, p_len;
    unsigned int inf;
    char *key = NULL, *par = NULL;
    char a;
    fseek(ptab->fd, 2*sizeof(int), SEEK_SET);
    for(int i = 0; i < ptab->csize; ++i){
        fread(&k_set, sizeof(int), 1, ptab->fd);
        fseek(ptab->fd, k_set, SEEK_SET);
        fread(&k_len, sizeof(int), 1, ptab->fd);
        key = calloc(k_len + 1, sizeof(char));
        for (int j = 0; j < k_len; ++j) {
            fread(&a, sizeof(char), 1, ptab->fd);
            key[j] = a;
        }
        key[k_len] = '\0';
        fseek(ptab->fd, 3*(i + 1)* sizeof(int), SEEK_SET);
        fread(&p_set, sizeof(int), 1, ptab->fd);
        if(p_set == -1){
            par = NULL;
        }
        else{
            fseek(ptab->fd, p_set, SEEK_SET);
            fread(&p_len, sizeof(int), 1, ptab->fd);
            par = calloc(p_len + 1, sizeof(char));
            for (int j = 0; j < p_len; ++j) {
                fread(&a, sizeof(char), 1, ptab->fd);
                par[j] = a;
            }
            par[p_len] = '\0';
            fseek(ptab->fd, (3*(i + 1) + 1)* sizeof(int), SEEK_SET);
        }
        fread(&inf, sizeof(unsigned int), 1, ptab->fd);
        printf("\t  %s\t  %s\t  %10.u\n", key, par, inf);
        free(key);
        free(par);
    }
    return 0;
}

/* int D_Load(Table *ptab){
    int SZ;
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = GetStr(); // вся строка вводится целиком
    if(fname == NULL)
        return 0; // конец файла
    if (load(ptab, fname) == 0){ // указанный файл не существует надо - создать новый файл и таблицу
        printf("Enter possible vector size: -->");
        if (GetInt(&SZ) == 1)
            return 0; // конец файла
        create(ptab, fname, SZ);
    }
    free(fname);
    return 1;
}

int D_Add(Table *ptab) {
    int k, rc;
    char *errmsg = "You are wrong. Repeat, please!";
    char *m = NULL, *b = NULL;
    unsigned int temp;
    //unsigned int *info = NULL;
    printf("Enter key: -->\n");
    m = GetStr();
    if (m == NULL) {
        return 0; // обнаружен конец файла
    }
    printf("Enter parent's key: -->\n");
    b = GetStr();
    if (b == NULL){
        return 0; // обнаружен конец файла
    }
    printf("Enter info:\n");
    k = GetInt(&temp);
    if (k == 1){
        return 0; // обнаружен конец файла
    }
    int y = f_insert(ptab, b);
    rc = add(ptab, m, b, temp, y); // вставка элемента в таблицу
    free(m); 	
    free(b);	
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}



int D_Find(Table *ptab)
{
    unsigned int info = 0;
    char *key = NULL;
    puts("Enter key: -->");
    key = GetStr();
    if(key == NULL)
        return 0; // конец файла
    findInfo(ptab, key, &info);
    if(info){
        printf("key = %s, info = \"%u\"\n", key, info);
    }
    else{
        printf("Key \"%s\" was not found\n", key);
    }
    free(key);    
    return 1;
}



int D_MPtable(Table *ptab){
    int k, rc;
    char *pkey = NULL;
    if(ptab->csize != 0) {
        puts("Enter par key >>");
        pkey = GetStr();
        if (pkey == NULL) {
            return 0; // обнаружен конец файла
        }
        if(strcmp(pkey,"") == 0){
            free(pkey);
            pkey = NULL;
        }
        k = p_check(ptab, pkey); // есть или нет род ключ совпадающий с заданным(0 или 1)
        rc = make_par_table(ptab, k, pkey);
        free(pkey);
    }
    else{
        k = 0;
        rc = 5; //пустая таблица
    }
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

int D_Delete(Table *ptab){
    int rc, k;
    char *a = NULL;
    puts("Enter key: -->");
    a = GetStr();
    if (a == NULL) {
        return 0; // обнаружен конец файла
    }
    k = k_check(ptab, a);
    rc = delete(ptab, k, a);
    free(a);
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

int D_Show(Table *ptab){
    int rc, k_set, p_set, k_len, p_len;
    unsigned int inf;
    char *key = NULL, *par = NULL;
    char a;
    if(ptab->csize == 0){
        rc = 5;
    }
    fseek(ptab->fd, 2*sizeof(int), SEEK_SET);
    for(int i = 0; i < ptab->csize; ++i){
        fread(&k_set, sizeof(int), 1, ptab->fd);
        if(k_set != -1) {
            fseek(ptab->fd, k_set, SEEK_SET);
            fread(&k_len, sizeof(int), 1, ptab->fd);
            printf("\t k lenth %d\n", k_len);
            printf("\t k off %d\n", k_set);
            key = calloc(k_len + 1, sizeof(char));
            for (int j = 0; j < k_len; ++j) {
                fread(&a, sizeof(char), 1, ptab->fd);
                key[j] = a;
            }
            key[k_len] = '\0';
            fseek(ptab->fd, 3*(i + 1)* sizeof(int), SEEK_SET);
            fread(&p_set, sizeof(int), 1, ptab->fd);
            if(p_set == -1){
                par = NULL;
            }
            else{
                fseek(ptab->fd, p_set, SEEK_SET);
                fread(&p_len, sizeof(int), 1, ptab->fd);
                printf("\t p lenth %d\n", p_len);
                printf("\t p off %d\n", p_set);
                par = calloc(p_len + 1, sizeof(char));
                for (int j = 0; j < p_len; ++j) {
                    fread(&a, sizeof(char), 1, ptab->fd);
                    par[j] = a;
                }
                par[p_len] = '\0';
                fseek(ptab->fd, (3*(i + 1) + 1)* sizeof(int), SEEK_SET);
            }
            fread(&inf, sizeof(unsigned int), 1, ptab->fd);
            printf("\t  %s\t  %s\t  %10.u\n", key, par, inf);
            free(key);
            free(par);
            rc = 0;
        }
        else{
            fseek(ptab->fd, (3*(i+1) + 2)*sizeof(int), SEEK_SET);
            printf("ёклмн опять \n");
            //--i;
        }
    }
    printf("%s\n", errmsgs[rc]);
    return 1;
}


int D_Save(Table *ptab)
{
//	int len, offset;
    fseek(ptab->fd, sizeof(int), SEEK_SET);
    fwrite(&(ptab->csize), sizeof(int), 1, ptab->fd);
    //fseek(ptab->fd, (ptab->csize * 3 + 2)*sizeof(int), SEEK_SET);
    //while(feof(ptab->fd) == 0){
    //	fread(&len, sizeof(int), 1, ptab->fd);
    //	if(len < 0){
    //		len = -len;
    //		fseek(ptab->fd, -1 * sizeof(char), SEEK_CUR);
    //		offset = ftell(ptab->fd);
    //		rm_str(ptab);
    //	}
    //	fseek(ptab->fd, len*sizeof(char), SEEK_CUR);
    //}
    fclose(ptab->fd);
    ptab->fd = NULL;
    return 1;
}

int main()
{
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_MPtable};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find by key", "3. Delete", "4. Show", "5. Find by parent's key" };
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    Table table = {0, 0, NULL};
    int rc;
    if (D_Load(&table) == 0)// загружаем таблицу, подготавливаем к работе файл данных
        return 1;
    while(rc = dialog(msgs, NMsgs)) // диалог
        if(!fptr[rc](&table))
            break;
    D_Save(&table); // выгружаем таблицу, закрываем файл данных
    printf("That's all. Bye!\n");
    return 0;
} */
