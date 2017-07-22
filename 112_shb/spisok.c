#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct MyStruct
{
	char rz[10];
	char num1[30];
	char num2[30];
	char num3[30];
	char num4[30];
	char num5[30];
	struct MyStruct *next;
} sMas;

sMas * create_element(char *rz, char *num1, char *num2, char *num3, char *num4, char *num5);
sMas * delete_element(sMas* oldElement);
sMas * add_element(sMas* Element, char *rz, char *num1, char *num2, char *num3, char *num4, char *num5 );
sMas * init_list(void);
int menu(void);
void find(sMas * Element);
void replace(sMas * Element);
int my_is_digit(char * num);
sMas * add(sMas * Element);

int main (void)
{
	sMas * p = init_list();
	sMas * iter;
	int choice;
	
	while (1)
	{
		choice=menu();
		switch(choice)
		{
			case 1: find(p);
				break;
			case 2: replace(p);
				break;
			case 3: p=add(p);
				break;
			case 4: delete_element(p);
				break;
			case 5: exit(1);
				break;
			case 6: exit(1);
		}
	}
	return 0;
}

sMas* create_element(char *rz, char *num1, char *num2, char *num3, char *num4, char *num5)
{
	sMas* newElement = malloc(sizeof(sMas));
	if ( newElement != NULL )
	{
		strncpy(newElement->rz,rz,10);
		strncpy(newElement->num1,num1,30);
		strncpy(newElement->num2,num2,30);
		strncpy(newElement->num3,num3,30);
		strncpy(newElement->num4,num4,30);
		strncpy(newElement->num5,num5,30);
		newElement->next = NULL;
	}
	return newElement;
}

void elemdelete(sMas * previous,sMas * delelem,sMas ** start,sMas ** last)
{
	if (previous) previous->next = delelem->next;
	else *start = delelem->next;
	
	if (delelem==*last && previous) *last=previous;
}

sMas * delete_element(sMas * Element)
{
	char s[20];
	char rz[10];
	int c,i=0;
	sMas * iter;
	sMas * pVar;
	sMas * buf;
	
	printf("\n\t1 Удаление: ");
	printf("\n\t2 Вернуться в главное меню: ");
	printf("\n\t3 Выход: ");
	
	do
	{
		printf("\n\tВведите необходимый пункт меню: ");
		scanf("%s",s);
		c=atoi(s);
	}while(c<0 || c>3);
	
	if ( c==1 )
	{
		printf("\n\t\tВведите номер RZ: ");
		scanf("%s",rz);
		if (strlen(rz)>10||strlen(rz)==0||my_is_digit(rz)==0)
		{
			printf("Неккоректное значение RZ\n");
		}
		else
		{
			buf=NULL;
			for (iter = Element; iter != NULL; iter = iter->next)
			{
				if(strcmp(rz,iter->rz)!=0) 
				{	
					/*if ( NULL != iter->next ){
						delete_element(iter->next);
					}
					free(iter);*/
					//printf("%p\n%p\n",iter,iter->next);
					buf=iter;
				}
				else 
				{
					buf->next=iter->next;
					free(iter);
					//if (iter->next)
					printf("%s\n%s\n%s\n",buf->rz,iter->rz,iter->next->rz);
				}
				/*if (i=0)
				{
					pVar = create_element(iter->rz,iter->num1,iter->num2,iter->num3,iter->num4,iter->num5);
				}
				else
				{
					pVar=add_element(pVar,iter->rz,iter->num1,iter->num2,iter->num3,iter->num4,iter->num5);
				}*/
				//printf("%d\n",i);
				//i++;
			}
			//return pVar;
			return;
		}
	}
	else if ( c==2 )
	{
		return;
	}
	else if ( c==3 )
	{
		exit(1);
	}
}



sMas* add_element(sMas* Element, char *rz, char *num1, char *num2, char *num3, char *num4, char *num5 ) 
{
	sMas* newElement = create_element( rz, num1, num2, num3, num4, num5 );
	if ( NULL != newElement ) 
	{
		Element->next = newElement;
	}
	return newElement;
}

int my_is_digit(char * num)
{
	int i;
	for (i=0; i<strlen(num); i++)
	{
		if (!isdigit(num[i]))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

void find(sMas * Element)
{
	char s[20];
	char rz[10];
	int c;
	int choice;
	char flag[1]="0";
	sMas * iter;
	
	
	printf("\n\t1 Поиск: ");
	printf("\n\t2 Вернуться в главное меню: ");
	printf("\n\t3 Выход: ");

	do
	{
		printf("\n\tВведите необходимый пункт меню: ");
		scanf("%s",s);
		c=atoi(s);
	}while(c<0 || c>3);
	
	if ( c==1 )
	{
		printf("\n\t\tВведите номер RZ: ");
		scanf("%s",rz);
		if (strlen(rz)>10||strlen(rz)==0||my_is_digit(rz)==0)
		{
			printf("Неккоректное значение RZ\n");
		}
		else
		{
			for (iter = Element; iter != NULL; iter = iter->next)
			{	
				if(strcmp(rz,iter->rz)==0) 
				{	
					printf("\n\nRZ: %s, Пожарные: %s, Полиция: %s, Скорая: %s, Газовая служба: %s Антитеррор: %s\n\n",iter->rz,iter->num1,iter->num2,iter->num3,iter->num4,iter->num5);
				}
			}
		}
	}
	else if ( c==2 )
	{
		return;
	}
	else if ( c==3 )
	{
		exit(1);
	}
}

void replace(sMas * Element)
{
	char s[20];
	char rz[10];
	int c;
	char num[30];
	sMas * iter;
	int i=0;
	printf("\n\t1 Введите номер RZ: ");
	printf("\n\t2 Вернуться в главное меню: ");
	printf("\n\t3 Выход: ");
	
	do
	{
		printf("\n\tВведите необходимый пункт меню: ");
		scanf("%s",s);
		c=atoi(s);
	}while(c<0 || c>3);
	
	
	if ( c==1 )
	{
		printf("\n\t\tВведите номер RZ: ");
		scanf("%s",rz);
		if (strlen(rz)>10||strlen(rz)==0||my_is_digit(rz)==0)
		{
			printf("Неккоректное значение RZ\n");
		}
		else
		{
			for (iter = Element; iter != NULL; iter = iter->next)
			{	
				if(strcmp(rz,iter->rz)==0) 
				{	
					printf("\n\t\t1 Изменить телефон пожарной службы: ");	
					printf("\n\t\t2 Изменить телефон милиции: ");	
					printf("\n\t\t3 Изменить телефон скорой: ");	
					printf("\n\t\t4 Изменить телефон газовой службы: ");	
					printf("\n\t\t5 Изменить телефон антитеррор: ");	
					printf("\n\t\t6 Вернуться в главное меню: ");
					printf("\n\t\t7 Выход: ");

					do
					{	
						printf("\n\t\tВведите необходимый пункт меню: ");
						scanf("%s",s);
						c=atoi(s);
					}while(c<0 || c>7);
			
					if ( c==6 )
					{
						return;
					}
					else if ( c==7 )
					{
						exit(1);
					}
					else
					{
						do
						{
							printf("\n\t\tВведите новый номер телефона: ");
							scanf("%s",num);
						}while(strlen(num) > 10);		
				
						if ( my_is_digit(num)==0 )
						{
							printf("\n\t\tНеккоректное значение номера телефона\n");
						}
						else
						{
							switch (c)
							{
								case 1: strncpy(iter->num1,num,30);
								break;
								case 2: strncpy(iter->num2,num,30);
								break;
								case 3: strncpy(iter->num3,num,30);
								break;
								case 4: strncpy(iter->num4,num,30);
								break;
								case 5: strncpy(iter->num5,num,30);
								break;
							}
						}
					}
				}
			}
		}
	}
	else if ( c==2 )
	{
		return;
	}
	else if ( c==3 )
	{
		exit(1);
	}
}

sMas * add(sMas * Element)
{
	char s[20];
	int c;
	char rz[10];
	char num1[30];
	char num2[30];
	char num3[30];
	char num4[30];
	char num5[30];
	sMas * iter;
	int flag=1;
	
	do
	{
		printf("\n\t\t Введите номер роутинговой зоны: ");	
		scanf("%s",rz);
	}while(strlen(rz) > 10);
	
	for (iter = Element; iter != NULL; iter = iter->next)
	{	
		if(strcmp(rz,iter->rz)==0)
		{
			flag=0;
		}
	}

	if (flag)
	{
		do	
		{
			printf("\n\t\t Введите телефон пожарной службы: ");	
			scanf("%s",num1);
		}while( strlen(num1) > 30 );
	
		do
		{
			printf("\n\t\t Введите телефон милиции: ");	
			scanf("%s",num2);
		}while( strlen(num2) > 30 );
	
		do
		{
			printf("\n\t\t Введите телефон скорой: ");	
			scanf("%s",num3);
		}while( strlen(num3) > 30 );
	
		do
		{
			printf("\n\t\t Введите телефон газовой службы: ");	
			scanf("%s",num4);
		}while( strlen(num4) > 30 );
	
		do
		{
			printf("\n\t\t Введите телефон антитеррор: ");	
			scanf("%s",num5);
		}while( strlen(num5) > 30 );
		
		iter=add_element( Element, rz, num1, num2, num3, num4, num5 );

		return iter;
	}
	else
	{
		printf("\n\t\t Такая RZ уже есть в списке\n");
		return;
	}
	
}

sMas * init_list(void)
{
	FILE *fp;

	char ch;

	char *rz;
	char *num01;
	char *num02;
	char *num03;
	char *num04;
	char *num05;
	
	int i=0;
	
	sMas * pVar;
	sMas * pVar2;
	if ((fp=fopen("nwgsm112.shb", "r")) == NULL)
	{
		printf("Can't open shb file.\n");
		exit(1);
	}

	rewind(fp);
	fseek(fp,0,SEEK_END);
	long int size=ftell(fp);
	
	char str[size];
	rewind(fp);
	
	while (!feof(fp))
	{
		ch=getc(fp);
		if (isdigit(ch))
		{	
			fseek(fp,-1,SEEK_CUR);
			fgets(str,size,fp);
			rz=strtok(str, ",");
			num01=strtok(str+strlen(rz)+1,",");
			num02=strtok(str+strlen(rz)+strlen(num01)+2,",");
			num03=strtok(str+strlen(rz)+strlen(num01)+strlen(num02)+3,",");
			num04=strtok(str+strlen(rz)+strlen(num01)+strlen(num02)+strlen(num03)+4,",");
			num05=strtok(str+strlen(rz)+strlen(num01)+strlen(num02)+strlen(num03)+strlen(num04)+5,",");
		
			if (strlen(rz)>10) 
			{
				printf("Can't read file. RZ more then 10 char, %s", rz);
				exit(1);
			}
			else if (strlen(num01)>30)
			{
				printf("Can't read file. FIRE more then 30 char, %s", num01);
				exit(1);
			}
			else if (strlen(num02)>30)
			{
				printf("Can't read file. POLICY more then 30 char, %s", num02);
				exit(1);
			}
			else if (strlen(num03)>30)
			{
				printf("Can't read file. EMERG more then 30 char, %s", num03);
				exit(1);
			}
			else if (strlen(num04)>30)
			{
				printf("Can't read file. GAZ more then 30 char, %s", num04);
				exit(1);
			}
			else if (strlen(num05)>30)
			{
				printf("Can't read file. ANTITERROR more then 30 char, %s", num05);
				exit(1);
			}
			
			if ( i == 0 )
			{
				pVar = create_element(rz,num01,num02,num03,num04,num05);
			}
			else
			{
				pVar = add_element(pVar,rz,num01,num02,num03,num04,num05);
			}
			i++;
		}
	}
	fclose(fp);	
	sMas * iter;
	for (iter = pVar; iter != NULL; iter = iter->next)
	{
		printf("%s\n",iter->rz);
	}				
	return pVar;
}

int menu(void)
{
	char s[20];
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
		if ( strlen(s)>20 || strlen(s)==0 )
		{
			printf("Неккоректное значение\n");
		}
		c=atoi(s);
	} while( c<0 || c>6 );
	return c;
}

