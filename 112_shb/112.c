#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_record {
    char *rz;
    char *num_01;
    char *num_02;
    char *num_03;
    char *num_04;
    char *num_05;
    struct s_record *next;
} record;

void init_list(void);
int menu(void);
void find(void);
void insert(int num_of_srv, int mas_num);
void replace(void);

int main(void) {
//    int choice;
    init_list();
/*    while (1) {
        choice=menu();
        switch(choice) {
            case 1: find();
                break;
            case 2: replace();
                break;
            case 3: exit(1);
                break;
            case 4: exit(1);
                break;
            case 5: exit(1);
                break;
            case 6: exit(1);
                break;
        }
    }*/
    return 0;
}

record* create_record(char *line) {
    record* rec = malloc(sizeof(record));
    char * token;
    token = strtok(line, ",");
    while(token) {
        if (token != NULL && isdigit(*token)) {
                    printf("NUM:%s\t", token);
            }
            token = strtok(NULL, ",");
        }
}

void delete_words(record* rec) {
    if (NULL != rec->next) {
        delete_words(rec->next);
    }
    free(rec);
}

void init_list(void) {
    FILE *fp;

    char *line;
    char *token;
    size_t len = 0;
    ssize_t read;
    record* rec = NULL;

    char *a;
    char *b;
    char *c;
    char *d;
    char *e;
    char *f;

    int i=0;

    if ((fp=fopen("test.csv", "r")) == NULL) {
        printf("Can't open shb file.\n");
        exit(1);
    }

    fseek(fp,0,SEEK_END);
    long int size=ftell(fp);
    printf("%li\n", size); 
    printf("RECORD: %li\n", sizeof(record));
    printf("RECORD: %li\n", sizeof(a));
    char str[size];
    rewind(fp);
    //p->next=NULL;
    while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, ",");
        i = 0;
        while(token != NULL) {
            if (isdigit(*token)) {
                printf("I:%d\t", i);
                printf("size: %li\n", strlen(token));
                if (rec != NULL) {
                    printf("NUM:%s\t", token);
                }
                i++;
            }
            token = strtok(NULL, ",");
        }
        printf("\n");
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
    }
    free(line);
}

int menu(void)
{
	char s[80];
	int c;
	printf("1. Найти RZ\n");
	printf("2. Заменить RZ\n");
	printf("3. Добавить RZ\n");
	printf("4. Удалить RZ\n");
	printf("5. Создать файл\n");
	printf("6. Выход\n");

	do
	{
		printf("\nВведите номер нужного пункта: ");
		scanf("%s",s);
		if (strlen(s)>80||strlen(s)==0)
		{
			printf("Неккоректное значение\n");
		}
		c=atoi(s);
	}while(c<0 || c>6);
	return c;
}


/*void find(void)
{
	char s[80];
	register int i;

	printf("\n\n\t1.1 Введите номер RZ: ");
	scanf("%s",s);
	if (strlen(s)>80||strlen(s)==0)
	{
		printf("Неккоректное значение RZ\n");
	}
	else
	{
		for (i=0; i<ROW; i++)
		{
			if(strcmp(s,mas[i].rz)==0) 
			{	
			printf("\n\nRZ: %s, Пожарные: %s, Полиция: %s, Скорая: %s, Газовая служба: %s Антитеррор: %s\n\n",mas[i].rz,mas[i].num_01,mas[i].num_02,mas[i].num_03,mas[i].num_04,mas[i].num_05);
			}
		}
	}
}
*/
/*void insert(int num_of_srv, int mas_num)
{
	char s[80];
	bool flag=true;

	strncpy(s,"\0",80);	
	printf("\t\t\tВведите новый номер телефона: ");
	scanf("%s",s);

	if (strlen(s)>30 || strlen(s)==0)
	{
		printf("Неккоректное значение телефона\n");
	}
	int j=0;
	for (j=0; j<strlen(s); j++)
	{
		if (!isdigit(s[j]))
		{
			flag=false;	
		}
	}

	if (flag == true)
	{
		if (num_of_srv == 1)
		{
			strncpy(mas[mas_num].num_01,s,30);
		} else if (num_of_srv == 2)
		{
			strncpy(mas[mas_num].num_02,s,30);
		} else if (num_of_srv == 3)
		{
			strncpy(mas[mas_num].num_03,s,30);
		} else if (num_of_srv == 4)
		{
			strncpy(mas[mas_num].num_04,s,30);
		} else if (num_of_srv == 5)
		{
			strncpy(mas[mas_num].num_05,s,30);
		}
	}
	else
	{
		printf("Номер должен состоять из числовых значений!\n");
	}
	
}
*/

/*void replace(void)
{
	char s[80];
	register int i;
	int f=0;
	int c;

	printf("\n\n\t2.1 Введите номер RZ: ");
	scanf("%s",s);
	
	if (strlen(s)>80||strlen(s)==0)
	{
		printf("Неккоректное значение RZ\n");
	}

	for (i=0; i<ROW; i++)
	{
		if(strcmp(s,mas[i].rz)==0) 
		{
			f=i;
			strncpy(s,"\0",80);	
		}
	}

	if (f!=0)
	{
		printf("\n\t\t1. Изменить телефон пожарной службы\n");	
		printf("\n\t\t2. Изменить телефон милиции\n");	
		printf("\n\t\t3. Изменить телефон скорой\n");	
		printf("\n\t\t4. Изменить телефон газовой службы\n");	
		printf("\n\t\t5. Изменить телефон антитеррор\n");	
		
		do
		{
			printf("\n\t\tВведите необходимый пункт меню: ");
			scanf("%s",s);
			c=atoi(s);
		}while(c<0 || c>5);
		
		switch (c)
		{
			case 1: insert(1,f);
				break;
			case 2: insert(2,f);
				break;
			case 3: insert(3,f);
				break;
			case 4: insert(4,f);
				break;
			case 5: insert(5,f);
				break;
		}
	}
	else
	{
		printf("Нет такой RZ\n");
	}
		
}
*/
