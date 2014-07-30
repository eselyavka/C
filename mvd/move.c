#include <dirent.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#define FROM_DIR "/fromdir/"  
#define TO_DIR "/todir/" 
#define MAXPATHLEN 255 
                       
int file_select(); 
char* my_str_comp(char *str1, char *str2); 
void my_errors(int num); 
int main() 
{ 
        //char *test="test.txt"; 
        //char *test1="asasas"; 
        char *from; 
        char *to; 
        struct dirent **files; 
        int count,i,sp=0; 
        printf("%s%s%s%s%s","Remove file from ",FROM_DIR," to ",TO_DIR,"\n"); 
        count=scandir(FROM_DIR, &files, file_select,alphasort); 
        for (i=1; i<count+1; i++)                                               
        {                                                                       
                from = my_str_comp(FROM_DIR, files[i-1]->d_name); 
                printf("%s ", from); 
                to = my_str_comp(TO_DIR, files[i-1]->d_name); 
                printf("%s ",to);                              
                if (rename(from, to) == 0) 
                {        
                        printf("%s\n", "Success"); 
                } 
                else 
                { 
                        printf("%s %d\n", "Unsuccess errno ", errno); 
                }
		free(from);
	     	free(to);       
        } 
        return 0;                                                          
}                                                                                  
                                                                                   
int file_select(struct dirent *entry)                                              
{                                                                                  
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) 
        {                                                                            
                return 0;                                                            
        }                                                                            
        else                                                                         
        {
                return 1;
        }
}

char* my_str_comp(char *str1, char *str2)
{
        char *str;
        char *pname[MAXPATHLEN];
        if ((strlen(str1)+strlen(str2)) >= sizeof(pname))
        {
              my_errors(0);
              exit(1);
        }
        else
        {
                str = (char *) malloc((strlen(str1) + strlen(str2) +1) *sizeof(char));
                strlcpy(str, str1, sizeof(pname));
                strlcat(str, str2, sizeof(pname));
                return str;
        }
}

void my_errors(int num)
{
        static char *err[] = {
                "Path too long!!!\n",
        };
        printf("%s",err[num]);
}
