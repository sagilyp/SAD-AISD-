#ifndef MATRIX_H                                                                                                                                
#define MATRIX_H                                                                                                                                
typedef struct Line{                                                                                                                          
    int n;                                                                                                                                    
	int *line;                                                                                                                                
}Line;                                                                                                                                        
                                                                                                                                              
typedef struct Matrix{                                                                                                                       
	Line *matr;                                                                                                                              
    int size; 
    int allot;                                                                                                                               
}Matrix;

void end();
int GetInt(int *a);
int input(Matrix *table);
int *sum_up(Matrix table);
void output(char *str, Matrix table);
void print_arr(char *str, int *res, Matrix table);
void erase(Matrix *table);

#endif
