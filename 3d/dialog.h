#ifndef DLG
#define DLG

#include "struct.h"

	int GetInt(int *a);
	char *GetStr();
	int dialog(const char *msgs[], int NMsgs);
	int D_Add(Table *ptab);
	int D_ReleaseFind(Table *ptab);
	int D_Find(Table *ptab);
	int D_ReleaseDelete(Table *ptab);
	int D_Delete(Table *ptab);
	int D_Show(Table *ptab);
//	#ifdef LAB_D 
		int D_Load(Table *ptab);
		int D_Save(Table *ptab);
//	#endif

#endif
