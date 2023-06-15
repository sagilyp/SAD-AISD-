#ifndef TBL
#define TBL

#include "struct.h"

	int kFind(Table *ptab, char *key, int bull);
	int kFind_r(Table *ptab, char *key, int release);
	unsigned short findRel(Table *ptab, char *key);
	int Add(Table *ptab, char *key, char *info);
	int Delete(Table *ptab, int k);
	int findRelease_s(Table *ptab, char *key, int rel);
	int findInfo_s(Table *ptab, char *key);
	int show(Table *ptab);
		

#endif 
