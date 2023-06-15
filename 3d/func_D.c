#include "table.h"
#include "hash.h"
#include "struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
typedef struct Table{ // тип - просматриваемая таблица-вектор
    FILE *fd; // дескриптор файла, чтобы выполнять операции с файлом
    int msize; // размер вектора
    int csize; // размер таблицы
}Table;


const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};

int GetInt(int *a){
    int n;
    while ((n=scanf("%u", a)) != 1){
        if(n<0){
            return 1;
        }
        //puts("You are wrong. Repeat, please!");
        scanf("%*[^\n]");
        scanf("%*c");
    }
    return 0;
}
char *GetStr(){
    char buf[81] ={0};
    char *res = NULL;
    int len = 0;
    int n;
    do{
        n = scanf("%80[^\n]", buf);
        if (n < 0){
            return NULL;
        }
        else if (n > 0){
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            strncpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else{
            scanf("%*c");
        }
    } while(n > 0);
    if(len > 0){
        res[len] = '\0';
    }
    else{
        res = calloc(1, sizeof(char));
    }
    return res;
}

int dialog(const char *msgs[], int NMsgs){
    char *ermsg = "";
    int rc, n;
    do{
        puts(ermsg);
        ermsg = "You are wrong. Repeat, please!";
        for(int i = 0; i < NMsgs; ++i){
            puts(msgs[i]);
        }
        puts("Make your choice >>");
        n = GetInt(&rc);
        if(n == 1) {
            rc = 0;
        }
    }while(rc >= NMsgs || rc < 0);
    scanf("%*[^\n]");
    scanf("%*c");
    return rc;
}


int get_easy(int N){
    int i, t, k, res;
    for (k=2; k<=N; k++){
        i = 2;
        t = 0;
        while(i < k){
            if (k%i==0){
                t++;
            }
            i++;
        }
        if(t==0){
            res = k;
        }
    }
    return res;
}
int hash1(char *key){
    int res = 1;
    for(int i = 0; i < (int)strlen(key); ++i){
        res = res * (int)key[i];
    }
    return 37*res;
}

int hash2(char *key){
    int r = 0;
    for(int i = 0; i < (int)strlen(key); ++i){
        r = r + (int)key[i];
    }
    return 13 * get_easy(r);
}

int hash(char *key, int i, int m){
    return abs((hash1(key) + i*hash2(key)) % m);
} */


// функция поиска ключа
// возвращает смещение ключа отн-но начала файла(в байтах)
// в случае неудачи -1
int kFind(Table *ptab, char *key, int bull){
    static int ern, n;
    int cnt, pos, k_ofs;
    unsigned short busy, k_len;
    char *kkkey = NULL;
    if(bull == 1){
        pos = hash(key, (int)strlen(key), ptab->msize);
        cnt = 0;
    }
    else{
        pos = (ern + 1) % ptab->msize;
        cnt = n + 1;
    }
    while(cnt < ptab->msize){
        fseek(ptab->fd, 2*sizeof(int) + pos*(2*sizeof(unsigned short) + 2*sizeof(int)), SEEK_SET);
        fread(&busy, sizeof(unsigned  short), 1, ptab->fd);
		if(busy == 1){
        	fseek(ptab->fd,sizeof(unsigned short), SEEK_CUR);
        	fread(&k_ofs, sizeof(int), 1, ptab->fd);
        	fseek(ptab->fd, k_ofs, SEEK_SET);
        	fread(&k_len, sizeof(unsigned short), 1, ptab->fd);
       		kkkey = calloc(k_len+1, sizeof(char));
        	fread(kkkey, sizeof(char), k_len, ptab->fd);
        	kkkey[k_len] = '\0';
        	printf("lalala %s\t", kkkey);
        	if(strcmp(kkkey, key) == 0){
            	ern = pos;
            	n = cnt;
            	//printf("ern %d pos %d\n", ern, pos);
            	free(kkkey);
            	return 2*sizeof(int) + pos*(2*sizeof(unsigned short) + 2*sizeof(int));
        	}
        	free(kkkey);
        	kkkey = NULL;
        }
        pos = (pos + 1) % ptab->msize;
        ++cnt;
    }
    return -1;
}


//поиск по релизу, -1 не нашёл
// возвращает смещение в байтах
int kFind_r(Table *ptab, char *key, int release){
    int cnt = 0, pos, k_ofs;
    unsigned short busy, rel, k_len;
    char *kkkey = NULL;
    pos = hash(key, (int)strlen(key), ptab->msize);
    while(cnt < ptab->msize){
        fseek(ptab->fd, 2*sizeof(int) + pos*(2*sizeof(unsigned short) + 2*sizeof(int)), SEEK_SET);
        fread(&busy, sizeof(unsigned  short), 1, ptab->fd);
        if(busy == 1){
        	fread(&rel, sizeof(unsigned  short), 1, ptab->fd);
        	fread(&k_ofs, sizeof(int), 1, ptab->fd);
        	fseek(ptab->fd, k_ofs, SEEK_SET);
        	fread(&k_len, sizeof(unsigned short), 1, ptab->fd);
        	kkkey = calloc(k_len+1, sizeof(char));
        	fread(kkkey, sizeof(char), k_len, ptab->fd);
        	kkkey[k_len] = '\0';
        	if( rel == (unsigned short)release && strcmp(kkkey, key) == 0){
            	free(kkkey);
            	return 2*sizeof(int) + pos*(2*sizeof(unsigned short) + 2*sizeof(int));
        	}
        	free(kkkey);
        	kkkey = NULL;
        }
        pos = (pos + 1) % ptab->msize;
        ++cnt;
    }
    return -1;
}

// найти последний релиз данного ключа
unsigned short findRel(Table *ptab, char *key){
    int i;
    unsigned short  max1, max2;
    i = kFind(ptab, key, 1);
    if(i < 0) return 1;
    fseek(ptab->fd, i + sizeof(unsigned short), SEEK_SET);
    fread(&max1, sizeof(unsigned short), 1, ptab->fd);
    i = kFind(ptab, key, 0);
    while(i >= 0){
        fseek(ptab->fd, i + sizeof(unsigned short), SEEK_SET);
        fread(&max2, sizeof(unsigned short), 1, ptab->fd);
        if(max2 > max1){
            max1 = max2;
        }
        i = kFind(ptab, key, 0);
    }
    return max1+1;
}


int Add(Table *ptab, char *key, char *info){
    if( ptab->msize == 0){
        free(key);
        free(info);
        return 5;// пустая
    }
    if(ptab->csize == ptab->msize){
        free(key);
        free(info);
        return 2;// переполнение
    }
    int i, cnt = 0;
    unsigned short rel, busy;
    i = hash(key, (int)strlen(key), ptab->msize);
    //printf("hash sum: %d\n", i);
    rel = (unsigned short)findRel(ptab, key);
    fseek(ptab->fd, 2*sizeof(int) + i*(2*sizeof(unsigned short) + 2*sizeof(int)), SEEK_SET);
    fread(&busy, sizeof(unsigned  short), 1, ptab->fd);
    while(busy == 1 && cnt < ptab->msize){
        i = (i+1) % ptab->msize;
        ++cnt;
        fseek(ptab->fd, 2*sizeof(int) + i*(2*sizeof(unsigned short) + 2*sizeof(int)), SEEK_SET);
        fread(&busy, sizeof(unsigned  short), 1, ptab->fd);
    }
    if(cnt < ptab->msize){
        int offset_key, offset_info;
        unsigned short len_k, len_i;
        busy = 1;
        if(rel == 1) {
            fseek(ptab->fd, 0, SEEK_END);// позиционирование на конец файла
            offset_key = ftell(ptab->fd); // запись в таблицу смещения
            len_k = (unsigned short) strlen(key);
            fwrite(&len_k, sizeof(unsigned short), 1, ptab->fd);
            fwrite(key, sizeof(char), len_k, ptab->fd);
        }
        else{
            int pos = kFind_r(ptab, key, 1);
            fseek(ptab->fd, pos + 2*sizeof(unsigned short), SEEK_SET);
            fread(&offset_key, sizeof(int), 1, ptab->fd);
        }
        fseek(ptab->fd, 0, SEEK_END);// позиционирование на конец файла
        offset_info = ftell(ptab->fd); // запись в таблицу смещения
        len_i = (unsigned short)strlen(info);
        fwrite(&len_i, sizeof(unsigned short), 1, ptab->fd);
        fwrite(info, sizeof(char), len_i, ptab->fd);

        fseek(ptab->fd, 2*sizeof(int) + i*(2*sizeof(unsigned short) + 2*sizeof(int)), SEEK_SET);
        fwrite(&busy, sizeof(unsigned short), 1, ptab->fd);
        fwrite(&rel, sizeof(unsigned short), 1, ptab->fd);
        fwrite(&offset_key, sizeof(int), 1, ptab->fd);
        fwrite(&offset_info, sizeof(int), 1, ptab->fd);
    }
    ptab->csize ++;
    return 0;
}


int Delete(Table *ptab, int k){
    unsigned short null = 0;
    if(ptab->csize == 0){
        return 5; //пустая таблица
    }
    if (k == -1) {
        return 3; // нет такого ключа
    }
    fseek(ptab->fd, k, SEEK_SET);
    fwrite(&null, sizeof(unsigned short), 1, ptab->fd);
    fwrite(&null, sizeof(unsigned short), 1, ptab->fd);
    fwrite(&null, sizeof(int), 1, ptab->fd );
    fwrite(&null, sizeof(int), 1, ptab->fd );
    --ptab->csize;
    return 0;
}

int findRelease_s(Table *ptab, char *key, int rel){
    if(ptab->csize <= 0) return 5;
    int k = kFind_r(ptab, key, rel);
    if(k < 0) return 3;
    unsigned short busy, len_i;
    int offs_i;
    fseek(ptab->fd, k, SEEK_SET);
    fread(&busy,sizeof(unsigned short), 1, ptab->fd);
    fseek(ptab->fd, sizeof(int) + sizeof(unsigned short), SEEK_CUR);
    fread(&offs_i,sizeof(int), 1, ptab->fd);
    fseek(ptab->fd, offs_i, SEEK_SET);
    fread(&len_i,sizeof(unsigned short), 1, ptab->fd);
    char *info = calloc(len_i + 1, sizeof(char));
    fread(info,sizeof(char), len_i, ptab->fd);
    info[len_i] = '\0';
    printf("\t   Busy \t  Key \t\t\t  Info \t\t  Release\n");
    printf("_________________________________________________________________________________\n");
    printf("\t  %5hu\t  %14s\t  %14s\t  %5hu\n", busy, key, info, (unsigned short)rel);
    free(info);
    return 0;
}


int findInfo_s(Table *ptab, char *key){
    if(ptab->csize == 0) return 5;
    int k = kFind(ptab, key, 1);
    if(k < 0) return 3;
    printf("\t   Busy \t  Key \t\t\t  Info \t\t  Release\n");
    while (k >= 0) {
        unsigned short busy, release, len_i;
        int offs_i;
        fseek(ptab->fd, k, SEEK_SET);
        fread(&busy,sizeof(unsigned short), 1, ptab->fd);
        fread(&release,sizeof(unsigned short), 1, ptab->fd);
        fread(&offs_i,sizeof(int), 1, ptab->fd);
        fseek(ptab->fd, offs_i, SEEK_SET);
        fread(&len_i,sizeof(unsigned short), 1, ptab->fd);
        char *info = calloc(len_i + 1, sizeof(char));
        fread(info,sizeof(char), len_i, ptab->fd);
        info[len_i] = '\0';
        printf("_________________________________________________________________________________\n");
        printf("\t  %5hu\t  %14s\t  %14s\t  %5hu\n", busy, key, info, release);
        free(info);
        info = NULL;
        k = kFind(ptab, key, 0);
    }
    return 0;
}


int show(Table *ptab){
    if(ptab->msize == 0){
        return 5;
    }
    unsigned short busy, release, len_i, len_k;
    int offs_k, offs_i;
    printf("\t   Busy \t  Key \t\t\t  Info \t\t  Release\n");
    for(int i = 0; i < ptab->msize; ++i){
        char *key = NULL, *info = NULL;
        fseek(ptab->fd, 2*sizeof(int) + i*(2*sizeof(unsigned short) + 2*sizeof(int)), SEEK_SET);
        fread(&busy, sizeof(unsigned short), 1, ptab->fd);
        if(busy == 1){
        	fread(&release, sizeof(unsigned short), 1, ptab->fd);
        	fread(&offs_k, sizeof(int), 1, ptab->fd);
        	fread(&offs_i, sizeof(int), 1, ptab->fd);
        	fseek(ptab->fd, offs_k, SEEK_SET);
        	fread(&len_k, sizeof(unsigned short), 1, ptab->fd);
        	key = calloc(len_k + 1, sizeof(char));
        	fread(key, sizeof(char), len_k, ptab->fd);
        	key[len_k] = '\0';

        	fseek(ptab->fd, offs_i, SEEK_SET);
        	fread(&len_i, sizeof(unsigned short), 1, ptab->fd);
        	info = calloc(len_i + 1, sizeof(char));
        	fread(info, sizeof(char), len_i, ptab->fd);
        	info[len_i] = '\0';
		}
		else{
			key = NULL;
			info = NULL;
			release = 0;
		}
        printf("_________________________________________________________________________________\n");
        printf("\t  %5hu\t  %14s\t  %14s\t  %5hu\n", busy, key, info, release);
        free(key);
        free(info);
    }
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
    unsigned short null = 0;
    ptab->msize = sz;
    ptab->csize = 0;
    ptab->fd = fopen(fname, "w+b");
    if (ptab->fd == NULL){
       // ptab->first = NULL;
        return 0;
    }
// выделяем место для таблицы и сразу заполняем ее нулями
 //   ptab->first = (Item *)calloc(ptab->SZ, sizeof(Item));
// записываем в начало файла размер вектора
    fseek(ptab->fd, 0, SEEK_SET);
    printf("lalalala1 %d\n", ptab->msize);
    fwrite(&ptab->msize, sizeof(int), 1, ptab->fd);
// записываем размер таблицы
	printf("lalalala2 %d\n", ptab->csize);
    fwrite(&ptab->csize, sizeof(int), 1, ptab->fd);
	for(int i = 0; i < ptab->msize; ++i){
		printf("\tAAAA \n");
    	fwrite(&null, sizeof(unsigned short), 1, ptab->fd);
    	fwrite(&null, sizeof(unsigned short), 1, ptab->fd);
    	fwrite(&temp, sizeof(int), 1, ptab->fd);
    	fwrite(&temp, sizeof(int), 1, ptab->fd);
	}
// țзаписываем саму таблицу
   // fwrite(ptab->first, sizeof(Item), ptab->msize, ptab->fd);
    return 1;
}
/*
int D_Load(Table *ptab){
    int sz;
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = GetStr(); // вся строка вводится целиком
    if(fname == NULL)
        return 0; // конец файла
    if (load(ptab, fname) == 0){ // указанный файл не существует надо - создать новый файл и таблицу
        printf("Enter possible vector size: -->");
        if (GetInt(&sz) == 1)
            return 0; // конец файла
        puts("криэйт");
        create(ptab, fname, sz);
    }
    free(fname);
    return 1;
}


//int D_Make(Table *ptab){
//    int k, s, rc;
//    if(ptab->msize != 0){
//        k = -1;
//        rc = 4;
//    }
//    else {
//        puts("Choose size of the table >>");
//        do {
//            k = GetInt(&s);
//        }while(s < 0);
//        if(k == 1){
//            return 0; // обнаружен конец файла
//        }
//        ptab->msize = s;
//        ptab->ks = calloc(s, sizeof(KeySpace));
//        if(ptab->ks == NULL){
//            return 0;
//        }
//        rc = 0;
//    }
//    printf("%s: %d\n", errmsgs[rc], k);
//    return 1;
//}

int D_Add(Table *ptab) {
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int  rc;
    char *errmsg = "You are wrong. Repeat, please!";
    char *m = NULL, *b = NULL;
    printf("Enter key: -->\n");
    m = GetStr();
    if (m == NULL) {
        return 0; // обнаружен конец файла
    }
    printf("Enter info:\n");
    b = GetStr();
    if (b == NULL){
        return 0; // обнаружен конец файла
    }
    rc = Add(ptab, m, b); // вставка элемента в таблицу
    free(b);
    free(m);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_ReleaseFind(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc, rel;
    char *key = NULL;
    puts("Enter key >>");
    key = GetStr();
    if (key == NULL) {
        free(key);
        return 0; // обнаружен конец файла
    }
    puts("Enter release number >>");
    k = GetInt(&rel);
    if (k == 1) {
        return 0; // обнаружен конец файла
    }
    rc = findRelease_s(ptab, key, rel);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Find(Table *ptab) {
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined","Void table"};
    char *key = NULL;
    int rc;
    puts("Enter key: -->");
    key = GetStr();
    if (key == NULL)
        return 0; // конец файла
    rc = findInfo_s(ptab, key);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Save(Table *ptab)
{
    fseek(ptab->fd, sizeof(int), SEEK_SET);
    fwrite(&ptab->csize, sizeof(int), 1, ptab->fd);
    //fwrite(ptab->first, sizeof(Item), ptab->n, ptab->fd);
    fclose(ptab->fd);
    ptab->fd = NULL;
    return 1;
}

int D_ReleaseDelete(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc, k, rel;
    char *key = NULL;
    puts("Enter key: -->");
    key = GetStr();
    if (key == NULL) {
        return 0; // обнаружен конец файла
    }
    puts("Enter release number: -->");
    k = GetInt(&rel);
    if (k == 1) {
        return 0; // обнаружен конец файла
    }
    k = kFind_r(ptab, key, rel);
    rc = Delete(ptab, k);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Delete(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc, k;
    char *key = NULL;
    puts("Enter key: -->");
    key = GetStr();
    if (key == NULL) {
        return 0; // обнаружен конец файла
    }
    k = kFind(ptab, key, 1);
    rc = Delete(ptab, k);
    while(k >= 0) {
        k = kFind(ptab, key, 0);
        Delete(ptab, k);
    }
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Show(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc;
    rc = show(ptab);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int main()
{
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_ReleaseFind,D_Delete, D_ReleaseDelete, D_Show};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find by key", "3. Find by key with release","4. Delete",
                          "5. Delete with release", "6. Show"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    Table table = {NULL, 0, 0 };
    int rc;
    if (D_Load(&table) == 0)// загружаем таблицу, подготавливаем к работе файл данных
        return 1;
    while(rc = dialog(msgs, NMsgs)) // диалог
        if(!fptr[rc](&table))
            break;
    D_Save(&table); // выгружаем таблицу, закрываем файл данных
    printf("That's all. Bye!\n");
    //delTable(&table); // освобождение памяти, заданной таблицей
    return 0;
} */
