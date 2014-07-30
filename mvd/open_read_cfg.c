#include <stdio.h>
#include <pcre.h>
#include <string.h>
#include <stdlib.h>

int pcre_comp(char *conf_values, char* str)
{
        FILE *fp;
        const char *error;
        int pairs;
        int vectorsize=6, erroffset;
        int vector[vectorsize];
        pcre *re;
        re=pcre_compile(conf_values, 0, &error, &erroffset, NULL);
        if (re == NULL)
        {
        	printf("%s\n%s\n", "PCRE FAILED!!!",error);
                exit(1);
        }
        if ((pairs=pcre_exec(re,NULL,str,strlen(str),0,0,vector,vectorsize)) < 0)
        {
        	printf("%s\t%s\n", "Syntax error in str # ", str);
		return 0;
        }
	else
	{
        	return 1;
	}
}

