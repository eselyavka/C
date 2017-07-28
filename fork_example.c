#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define MAX 25            /* maximum number of letters communicated */

float my_sum(char *a, char *b)
{
        
	//printf("%s",a);
	float tmp1=strtof(a,&a);
	float tmp2=strtof(b,&b);
        float value=tmp1 + tmp2;
        printf("%10.2f\n",value);
        return value;
}


void 
read_from_pipe (int file)
{
  FILE *stream;
  int c;
  stream = fdopen (file, "r");
  while ((c = fgetc (stream)) != EOF)
    putchar (c);
  fclose (stream);
}


int main (void)
{  int fd[2];             /* provide file descriptor pointer array for pipe */
                          /* within pipe:
                                 fd[0] will be input end
                                 fd[1] will be output end */
   pid_t pid;

   char line[MAX];        /* character array (string) for reading */
     
   if (pipe (fd) < 0)     /* create pipe and check for an error */
     { perror("pipe error");
       exit (1);
     }

   if ((pid = fork()) < 0)  /* apply fork and check for error */
     { perror ("error in fork");  
       exit (1);
     }

   if (0 == pid)             
     { /* processing for child */
       printf ("The child process is active.\n");
       close (fd[1]);       /* close output end, leaving input open */
       read(fd[0], line, MAX);
       printf ("The string received is '%s'\n", line);
     }  
   else 
     { /* processing for parent */
       printf ("The parent process is active.\n");
       close (fd[0]);       /* close input end, leaving output open */
       scanf("%s",line);
       while (line)
       {
       		write (fd[1], line, 23);
       }
                            /* print string and indicate byte length */
     }

   exit (0);                 /* quit by reporting no error */
}
