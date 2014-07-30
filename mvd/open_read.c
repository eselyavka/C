#include <stdio.h>
#include <pcre.h>
#include <string.h>

#define MAX_REC_LEN 1024

char *read_cfg(char *conf_values)
{
	FILE *fp;
	char *str,*data,*value, *test;
	const char *error;
	int fl,pairs,str_num;
	int vectorsize=6, erroffset;
	int vector[vectorsize];
	pcre *re;
	if ((fp = fopen("./mvd.conf", "r")) == NULL )
	{
		printf("%s\n", "Error opening file!!!");
		exit(1);
	}
	fseek(fp, 0L, SEEK_END);
	fl = ftell(fp);
	rewind(fp);
	//printf("%i\n", fl);
	str=malloc(sizeof(fl)+1);	
	if (str == NULL)
	{
		printf("%s\n", "Can't allocate memory for string!!!");
		exit(1);
	}
	re=pcre_compile(conf_values, 0, &error, &erroffset, NULL);
	if (re == NULL)
	{
		printf("%s\n%s\n", "PCRE FAILED!!!",error);
		exit(1);
	}
	str_num=0;
	while(!feof(fp))
	{
		if (fgets(str,fl+1,fp))
		{
			str_num++;
			if ((pairs=pcre_exec(re,NULL,str,strlen(str),0,0,vector,vectorsize)) < 0)
			{
				printf("%s%i\t\n", "Error in str # ", str_num);	
			//	printf("%s\n", str);
			}
			else
			{
				data=strtok(str,"=");
				data=strtok(NULL,"=");
				break;
			}
		}
	}
	free(str);
	fclose(fp);
	//printf("CFG %s", data);
	return data;
}

void chomp(char *s)
{
	char *p;
	while (NULL != s && NULL != (p = strrchr(s, '\n'))){
		*p = '\0';
	}
}

int main(){
	char *running_dir="[Rr][Uu][Nn][Nn][Ii][Nn][Gg]_[Dd][Ii][Rr]=([A-Za-z1-9_/])";
	char *r_dir,*r_dir_tmp;
	r_dir_tmp=read_cfg(running_dir);
	chomp(r_dir_tmp);
	printf("%s\n", r_dir_tmp);
	char lock_file[]="[Ll][Oo][Cc][Kk]_[Ff][Ii][Ll][Ee]=([A-Za-z1-9_/])";
	char *l_file;
	char log_file[]="[Ll][Oo][Gg]_[Ff][Ii][Ll][Ee]=([A-Za-z1-9_/])";
	char *lg_file;
	return 0;
}

