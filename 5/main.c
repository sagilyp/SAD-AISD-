  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Vertex{
	char *name;
	unsigned short int sex;
	unsigned short int birthdate;
	unsigned short int deathdate;
	//unsigned short int color; 
	struct Vertex *next;
	struct Vertex *parent;
	struct Vertex *right;
	unsigned short *num;
}Vertex;

typedef struct Graph{
	Vertex *source;
	unsigned short int vertexCount;
}Graph;

const char *errmsgs[] = {"Ok", "Void tree", "No edges", "No vertices", "No path", "Relative is alive", "Relative", "Problems with years"};
const char *sex[] = {"woman","man"};
const char *color[] = {"white", "gray", "black"};

int GetInt(int *a){
    int n;
    while ((n=scanf("%d", a)) != 1){
        if(n<0){
            return 1;
        }
        puts("You are wrong. Repeat, please!");
        scanf("%*[^\n]");
        scanf("%*c");
    }
    return 0;
}

int GetInt_us(unsigned short *a){
    int n;
    while ((n=scanf("%hu", a)) != 1){
        if(n<0){
            return 1;
        }
        puts("You are wrong. Repeat, please!");
        scanf("%*[^\n]");
        scanf("%*c");
    }
    return 0;
}

int GetDbl(double *a){
    int n;
    while ((n=scanf("%lf", a)) != 1){
        if(n<0){
            return 1;
        }
        puts("You are wrong. Repeat, please!");
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

void delVertex(Vertex *garbage);

int addVertex(Graph *graph, char *name, unsigned short sex, unsigned short birth, unsigned short death){
	if(death != 0 && (birth > death || (death-birth) > 140)){
		free(name);
		return 7;
	}	
	Vertex *new = (Vertex *)calloc(1, sizeof(Vertex)), *ptr = graph->source;
	new->num = calloc(1, sizeof(unsigned short));
	new->name = name;
	new->sex = sex;
	new->birthdate = birth;
	new->deathdate = death;
	if(!ptr){
		//puts("чо не так");
		new->num[0] = 0;
		graph->source = new;
		graph->vertexCount = 1;
		return 0;
	}
	while(ptr->next){
		if(strcmp(ptr->name, name) == 0){
			if(ptr->deathdate == death || ptr->birthdate == birth){
				free(new->num);
				delVertex(new);
				return 6;
			}
		}
		ptr = ptr->next;
	}
	ptr->next = new;
	(new->num)[0] = graph->vertexCount;
	graph->vertexCount++;
	return 0;
}

int addEdge(Graph *graph, char *parent, char *heir){
        int flag1 = 0, flag2 = 0;
        if(!graph){
                return 1;
        }
        Vertex *new = (Vertex *)calloc(1, sizeof(Vertex)), *ptr = graph->source, *par, *prev,
        					 *daughter = (Vertex *)calloc(1, sizeof(Vertex));
        while(ptr && (flag1 == 0 || flag2 == 0)){
                if(strcmp(ptr->name, parent) == 0){
						daughter->name = strdup(ptr->name);
						daughter->sex = ptr->sex;
						daughter->birthdate = ptr->birthdate;
						daughter->deathdate = ptr->deathdate;
						daughter->num = ptr->num;
						new->parent = ptr;
					//	printf("\t%hu/n", daughter->num[0]);
                        par = ptr;
                        flag1 = 1;
                }
                if(strcmp(ptr->name, heir) == 0){
                		if(ptr->parent){
                			delVertex(new);
                			delVertex(daughter);
                			return 6;
                		}
                		ptr->parent = par; 
						new->name = strdup(ptr->name);
						new->sex = ptr->sex;
						new->birthdate = ptr->birthdate;
						new->deathdate = ptr->deathdate;
						new->num = ptr->num;
					//	printf("\t%hu/n", new->num[0]);
						prev = ptr;
                       	flag2 = 1;
                }
                ptr = ptr->next;
        }
        if(flag1 == 1 && flag2 == 1){
        	if(daughter->birthdate+15 > new->birthdate){
        		delVertex(new);
        		delVertex(daughter);
        		return 7;
        	}
        	while(par->right)
        		par = par->right;
        	par->right = new;
        	while(prev->right)
        		prev = prev->right;
        	prev->right = daughter;	
        	return 0;	
        }
        delVertex(new);
        delVertex(daughter);
        return 3;
}


int deleteVert(Graph *graph, char *name){
	int flag2 = 0;
	if(!graph){
		return 1;
	}
	Vertex *ptr = graph->source, *prev, *par = NULL;
	char *sur = NULL;
	while(flag2 == 0 && ptr){
		if(strcmp(ptr->name, name) == 0){ //если нашли вершину с данным именем
			//меняем родителей
			if(!par){
				graph->source = ptr->next;
				par = graph->source;
				prev = par;
				while(prev){
					prev->num[0]--;
					prev = prev->next;
				}
			}
			else{
				par->next = ptr->next;
				//par = par->next;
				prev = par->next;
				while(prev){
					prev->num[0]--;
					prev = prev->next;
				}
			}	
			sur = strdup(ptr->name);
			free(ptr->num);
			while(ptr){
				prev = ptr->right;
				//if(prev) deleteVert(graph, prev->name);
				delVertex(ptr);
				ptr = prev;
			}	
			flag2 = 1;
		} /// end
		else
			par = ptr;
		if(par)ptr = par->next;
		else ptr = NULL;
	}
	ptr = graph->source;
	while(sur && ptr){
		prev = ptr;	
		par = ptr->right;
		while(par){
			if(strcmp(par->name, sur) == 0){
				prev->right = par->right;
				delVertex(par);
				par = prev;
			}
			prev = par;
			par = par->right;
		}
		ptr = ptr->next;
	}
	free(sur);
	if(flag2 == 1){
		graph->vertexCount--;
		return 0;
	}	
	return 3; 
}

int delEdge(Graph *graph, char *parent, char *heir){
	        int flag = 0;
	        if(!graph){
	                return 1;
	        }
	        Vertex *ptr = graph->source, *prev, *tmp;
	        while(ptr){
	        		tmp = ptr;
	                while(tmp && strcmp(ptr->name, parent) == 0){
						prev = tmp;
	                	tmp = tmp->right;
	                    if(tmp && strcmp(tmp->name, heir) == 0){
	                            prev->right = tmp->right;
	                            delVertex(tmp);
	                            tmp = prev;
	                    }
	                    flag = 1;
	                }
	                while(tmp && strcmp(ptr->name, heir) == 0){
	                	prev = tmp;
	                	tmp = tmp->right;
	                	if(tmp && strcmp(tmp->name, parent) == 0){
	                		prev->right = tmp->right;
	                		delVertex(tmp);
	                		tmp = prev;
	                	}
	                }
	                ptr = ptr->next;
	        }
	        if(flag == 1)
	                return 0;
	        return 3;
}

int edit(Graph *graph, char *name, unsigned short sex, unsigned short  birth,unsigned short death){
	if(!graph)
		return 1;
	int flag = 0;
	Vertex *ptr = graph->source, *tmp;
	while(ptr){
		if(flag == 0 && strcmp(ptr->name,name) == 0){ // нашли вершину с именем name в главном списке
			ptr->sex = sex;
			ptr->birthdate = birth;
			ptr->deathdate = death;
			flag = 1;
		}
		tmp = ptr->right;
		while(tmp){
			if(strcmp(tmp->name, name) == 0){
				tmp->sex = sex;
				tmp->birthdate = birth;
				tmp->deathdate = death;
			}
			tmp = tmp->right;
		}
		ptr = ptr->next;
	}
	if(flag)
		return 0;
	return 3;	
}

void pop(int *queue, int value){
	int i = 0;
	while(queue[i] != -1){
		i++;
	}
	queue[i] = value;
}

void get(int *queue,int size, int *value){
	*value = queue[0];
	int i = 0;
	while(i != size-1 && queue[i] != -1){
		queue[i] = queue[i+1];
		++i;
	}
	if(i == size-1){
		queue[i] = -1;
	}
}

int bfs(Graph *graph, char *name){
	if(!graph)
		return 1;
	Vertex *ptr = graph->source, *prev;	
	int color[graph->vertexCount], d[graph->vertexCount], pred[graph->vertexCount], queue[graph->vertexCount];  // создаем копии, где будем менять цвета
	int i = 0, moo = -1, j;
	while(i < graph->vertexCount){ //инициализация
		color[i] = 0;
		d[i] = 100000;
		pred[i] = 0;
		queue[i] = -1;
		++i;
	}	
	i = -1;
	while(ptr){ //поиск номера вершины
		if(strcmp(ptr->name, name) == 0){
			i = ptr->num[0]; // номер родителя наследников
			if(ptr->parent) moo = ptr->parent->num[0];
		}
		ptr = ptr->next;
	}
	if(i == -1)
		return 3;	
	color[i] = 1;
	if(moo != -1) color[moo] = 2;
	d[i] = 0;
	pop(queue, i);
	printf("Person ------ \"%s\"\n", name);
	puts("----------------------HEIRS------------------------");
	while(queue[0] != -1){
		int k;
		j = 0;
		Vertex *ptr = graph->source;
		get(queue, graph->vertexCount, &k);
		while(j != k){
			ptr = ptr->next;
			j++;
		}
		ptr = ptr->right;
		while(ptr){
			if(color[ptr->num[0]] == 0){
				d[ptr->num[0]] = d[k] + 1;
				pred[ptr->num[0]] = k;
				color[ptr->num[0]] = 1;
				pop(queue, ptr->num[0]);
				printf("%9s/%5s/%5hu - %5hu --- ", ptr->name, sex[ptr->sex], ptr->birthdate, ptr->deathdate);
			}
			ptr = ptr->right;
		}
		color[k] = 2;
		puts("");
	}
	return 0;	
}

int ExtractMin(int *d, int *s, unsigned short size){
	int met = 100000, res = -1;
	for(int i = 0; i < size; ++i){
		if(met > d[i] && s[i] == -1){
			met = d[i];
			res = i;
		}	
		//if(met > d[i] && s[i] == 1){
		//	met = d[i];
		//	s[i] = i;	
		//}	
	}
	return res;
	
}

int short_path_dijkstra(Graph *graph, char *from, char *to, int *di){
	int s[graph->vertexCount], d[graph->vertexCount]; // pred[graph->vertexCount];
	Vertex *queue[graph->vertexCount], *ptr = graph->source, *u, *pred[graph->vertexCount];
	  // инициализация
	int i, k, res;
	for(i = 0; i < graph->vertexCount; ++i){
		d[i] = 100000;
		pred[i] = NULL;
		s[i] = -1;
		queue[i] = ptr;
		if(strcmp(ptr->name, from) == 0)
			k = i;
		if(strcmp(ptr->name, to) == 0)
			res = i;
		ptr = ptr->next;
	} ///
	d[k] = 0;
	//s[k] = 1;
	i = ExtractMin(d, s, graph->vertexCount);
	while(i != -1){
	//	printf("MIN %d\n", i);
		u = queue[i];
		s[i] = 1;
		u = u->right;
		while(u){
			if(d[u->num[0]] > d[i] + 1){
				d[u->num[0]] = d[i] + 1;
				pred[u->num[0]] = queue[i];
			}
			u = u->right;
		}
		i = ExtractMin(d, s, graph->vertexCount);
	}
	*di = d[res];
	return 0;
}

int geld(Vertex **queue, int *heirs, int moo, int foo){
	if(moo == -1){
		int i = 0;
		Vertex *tmp = queue[0];
		while(tmp){
			heirs[i] = 1;
			tmp = tmp->next;
			i++;
		}
		heirs[foo] = -1;
		return 0;
	}	
	
	Vertex *ptr = queue[foo];
	ptr = ptr->right;
	while(ptr){
		if(ptr->num[0] != moo){
			heirs[ptr->num[0]] = 1;
			geld(queue, heirs, foo, ptr->num[0]);
		}	
		ptr = ptr->right;
	}
	return 0;
}

//алгоритм Флойда-Воршалла 
int distribute(Graph *graph, char *name, double wealth){
	if(!graph){
		return 1;
	} 
	int pred[graph->vertexCount][graph->vertexCount], heirs[graph->vertexCount];
	int d[graph->vertexCount][graph->vertexCount];
	Vertex *ptr = graph->source, *prev, *queue[graph->vertexCount];
	if(!ptr)
		return 1;
	int i, j, k, foo, moo = -1;
	double x = 0, koff;
	//инициализация

	for(i = 0; i < graph->vertexCount; ++i){

		queue[i] = ptr; 
		if(strcmp(ptr->name, name) == 0){
			foo = i;
			if(ptr->parent) moo = ptr->parent->num[0];
			if(ptr->deathdate == 0000){
				return 5;
			}
		}

		heirs[i] = -1;
		
		prev = ptr->right;
		for(j = 0; j < graph->vertexCount; ++j){
			d[i][j] = 100000;
			//	if(moo == -1 && strcmp(prev->name, name) == 0)
				//	moo = i
			//	if(prev->num[0] == j)
				
			if(i == j)
				d[i][j] = 0;
			pred[i][j] = -1;
			
		}
		while(prev){
			d[i][prev->num[0]] = 1;
			prev = prev->right;
		}	
		ptr = ptr->next;
	}
	//
	k = 0;
	while(k < graph->vertexCount){
	
		for(i = 0; i < graph->vertexCount; ++i){
			for(j = i; j < graph->vertexCount; ++j){
				if(d[i][j] > d[i][k] + d[k][j]){
					d[i][j] = d[i][k] + d[k][j];
					d[j][i] = d[i][k] + d[k][j];
					pred[i][j] = k;
					pred[j][i] = k;
				}
			}
		}
		k++;
	}
	//нашли кратчайшие пути
	geld(queue, heirs, moo, foo); // ищем прямых потомков
	// распределяем деньги
	for(i = 0; i < graph->vertexCount; ++i){
		if(heirs[i] == 1){
	//		printf("kaka %d\n", d[foo][i]);
			x = x + wealth/pow(2,d[foo][i]-1);
		}
	}
//	printf(" lalalala %lf\n", x);
	koff = x/wealth;
	x = wealth/koff;
	for(i = 0; i < graph->vertexCount; ++i){
		if(heirs[i] == 1){
			printf("heir: \"%s\" - %s >> %lf (rbl)\n", queue[i]->name, sex[queue[i]->sex], x/(pow(2,d[foo][i]-1)));
		}
	}
	return 0;
}


int dialog(const char *msgs[], int N)
{
    const char *errmsg = "";
    int rc;
    int i, n;

    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeat, please!";

        // вывод списка альтернатив
        for (i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");

        n = GetInt(&rc); // ввод номера альтернативы
        if (n == 1) // конец файла - конец работы
            rc = 0;
    } while (rc < 0 || rc >= N);
    scanf("%*[^\n]");
    scanf("%*c");
    return rc;
}

unsigned short getSex(){
	int num = sizeof(sex)/sizeof(sex[0]);
	unsigned short int i;
	int j;
	do{
		i = 0;
		while(i < num){
			printf("\t%hu - %s\n", i, sex[i]);
			++i;
		}
		j = GetInt_us(&i);
		if(j == 1){
			return 3; // конец файла
		}
	}while(i > 1);
	return i;	
}

//диалоговая функция добавления вершины в граф
int D_AddVertex(Graph *graph){
	int rc = 0;
	char *name;
	unsigned short sex, birth, death;
	puts("Enter the person name >>");
	name = GetStr();
	if(!name)
		return 0;// конец файла
	puts("Enter the sex of the person >>");
	sex = getSex();
	puts("Enter date of birth >>");
	rc = GetInt_us(&birth);
	if(rc == 1)
		return 0;
	puts("Enter date of death >>");
	rc = GetInt_us(&death);
	if(rc == 1)
		return 0;
	rc = addVertex(graph, name, sex, birth, death);	
	printf("%s\n", errmsgs[rc]);
	return 1;
}

//диалоговая функция лобавления ребра
int D_AddEdge(Graph *graph){
	int rc;
	char *heir, *parent;
	puts("Enter the name of the parent:");
	parent = GetStr();
	if(!parent)
		return 0;
	puts("Enter the name of the heir:");
	heir = GetStr();
	if(!heir)
		return 0;
	rc = addEdge(graph, parent, heir);
	free(parent);
	free(heir);
	printf("%s", errmsgs[rc]);
	return 1;		
}

//диалоговая функция удаления вершины
int D_DeleteVertex(Graph *graph){
	int rc;
	char *name;
	puts("Enter the name of vertex:");
	name = GetStr();
	if(!name)
		return 0;
	rc = deleteVert(graph, name);
	free(name);
	printf("%s", errmsgs[rc]);
	return 1;
}

int D_DeleteEdge(Graph *graph){
	int rc;
	char *parent, *heir;
	puts("Enter the name of the parent:");
	parent = GetStr();
	if(!parent)
		return 0;
	puts("Enter the name of the heir");
	heir = GetStr();
	if(!heir)
		return 0;
	rc = delEdge(graph, parent, heir);
	printf("%s\n", errmsgs[rc]);
	free(parent);
	free(heir);
	return 1;		
}

//
//диалоговая функция добавления вершины в граф
int D_EditVertex(Graph *graph){
        int rc;
        char *name;
        unsigned short sex, birth, death;
        puts("Enter the person name to edit >>");
        name = GetStr();
        if(!name)
                return 0;// конец файла
        //puts("Enter the new parent's name");
        //parent = GetStr();
        //if(!parent)
        //        return 0;
        puts("Enter the new sex of the person >>");
        sex = getSex();
        puts("Enter new date of birth >>");
        rc = GetInt_us(&birth);
        if(rc == 1)
                return 0;
        puts("Enter new date of death >>");
        rc = GetInt_us(&death);
        if(rc == 1)
                return 0;
        rc = edit(graph, name, sex, birth, death);
        free(name);
        printf("%s\n", errmsgs[rc]);
}

//диалоговая функция вывода дерева
int D_Show(Graph *graph){
	int rc = 0;
	Vertex *ptr = graph->source;
	if(!ptr)
		rc = 1;
	printf("%hu\n", graph->vertexCount);
	puts("---------------YOUR TREE-------------------");	
	while(ptr){
		Vertex *temp = ptr;
		printf("%9s/%5s/%5hu - %5hu/%hu  ---> ", ptr->name, sex[ptr->sex], ptr->birthdate, ptr->deathdate, (ptr->num)[0]);
		while(temp->right){
			temp = temp->right;
			printf("%9s/%5s/%5hu - %5hu/%hu  ---> ", temp->name, sex[temp->sex], temp->birthdate, temp->deathdate, temp->num[0]);
		}
		puts("");
		ptr = ptr->next;
	}	
	printf("%s\n", errmsgs[rc]);
	return 1;
}

//диалоговая функция обхода в ширину, начиная с некоторого родителя
int D_Bypass(Graph *graph){
	int rc;
	char *name;
	puts("Enter the name of person:");
	name = GetStr();
	if(!name)
		return 0;
	rc = bfs(graph, name);
	free(name);
	printf("%s\n", errmsgs[rc]);	
}

//диалоговая функция поиска кратчайшего пути по алгоритму Дейкстры
int D_ShortestPath(Graph *graph){
	int rc, d = 0 ;
	char *from, *to;
	puts("Enter the name of the parent:");
	from = GetStr();
	if(!from)
		return 0;
	puts("Enter the name of heir:");
	to = GetStr();
	if(!to)
		return 0;
	rc = short_path_dijkstra(graph, from, to, &d);
	printf("RANGE = %d\n", d);
	free(from);
	free(to);
	printf("%s\n", errmsgs[rc]);		 
	
}

int D_DistributeInheritance(Graph *graph){
	int rc;
	double wealth; 
	char *dead;
	puts("Enter name of dead relative");
	dead = GetStr();
	if(!dead)
		return 0;
	puts("Enter the amount of wealth(in roubles)");
	do{
		rc = GetDbl(&wealth);
		if(rc == 1)
			return 0;
	}while(wealth <= 0);			
	rc = distribute(graph, dead, wealth);
	free(dead);
	printf("%s\n", errmsgs[rc]);	
}


void delVertex(Vertex *garbage){
	if(garbage->name) free(garbage->name);
	free(garbage);
}

void delGraph(Graph *graph){
	Vertex *ptr = graph->source;
	while(ptr){
		Vertex *temp = ptr->right, *pat;
		while(temp){
			pat = temp->right;
			delVertex(temp);
			temp = pat;
		}
		pat = ptr;
		ptr = ptr->next;
		free(pat->num);
		delVertex(pat);
	}
	free(graph);
}

int main(){
	int (*fptr[])(Graph *) = {NULL, D_AddVertex, D_AddEdge, D_DeleteVertex, D_DeleteEdge,
							 D_EditVertex, D_Show, D_Bypass, D_ShortestPath, D_DistributeInheritance};
	const char *msgs[] = {"0. Qiut", "1. Add vertex", "2. Add edge","3. Delete vertex", "4. Delete edge",
						 "5. Edit vertex" ,"6. Show", "7. Find heirs", "8. Find shortest path", "9. Disribute wealth"};
	const int NMsgs = sizeof(msgs)/sizeof(msgs[0]);
	Graph *famaly = (Graph*)calloc(1, sizeof(Graph));
	int rc;
	while(rc = dialog(msgs, NMsgs)){
		if(!fptr[rc](famaly))
			break; // обнаружен конец файла
	} 
	printf("That's all. Bye!\n");
	delGraph(famaly);
	return 0;
}


