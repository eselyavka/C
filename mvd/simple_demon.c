#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <pcre.h>


int MAXPATHLEN=255;

char RUNNING_DIR[255];
char LOCK_FILE[255];
char LOG_FILE[255];
char FROM_DIR[255];
char TO_DIR[255];

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
                //exit(1);
        }
	else
	{
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
                               		//printf("%s%i\t\n", "Error in str # ", str_num);
                        		//printf("%s\n", str);
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
}

int chomp(char *s)
{
	size_t len = strlen(s);
	if (len == 0) {
		return 0;
	}
	else if (s[len-1] == '\n') {
		s[len-1] = '\0';
		return 1;
	}
	else {
		return 0;
	}
}

void set_main_vars()
{
        char *running_dir1="[Rr][Uu][Nn][Nn][Ii][Nn][Gg]_[Dd][Ii][Rr]=([A-Za-z1-9_/])";
        char *r_dir;
	char *lock_file="[Ll][Oo][Cc][Kk]_[Ff][Ii][Ll][Ee]=([A-Za-z1-9_/])";
	char *l_file;
	char *log_file="[Ll][Oo][Gg]_[Ff][Ii][Ll][Ee]=([A-Za-z1-9_/])";
	char *lg_file;
	char *from_dir="[Ff][Rr][Oo][Mm]_[Dd][Ii][Rr]=([A-Za-z1-9_/])";
	char *fr_dir;
	char *to_dir="[Tt][Oo]_[Dd][Ii][Rr]=([A-Za-z1-9_/])";
	char *todir;
	r_dir=NULL;
	l_file=NULL;
	lg_file=NULL;
	fr_dir=NULL;
	todir=NULL;

        r_dir=read_cfg(running_dir1);
	//printf("%s\n",r_dir);
	chomp(r_dir);
	if ((r_dir!=NULL) && (strlen(r_dir) < 255)) 
	{
		strncpy(RUNNING_DIR, r_dir, sizeof(RUNNING_DIR));
		//printf("SET_MAIN_VARS: %s\n",RUNNING_DIR);
	}
	else
	{
		strncpy(RUNNING_DIR,"/tmp",sizeof(RUNNING_DIR));
	}

	l_file=read_cfg(lock_file);
	//printf("%s\n",l_file);
	chomp(l_file);
	if ((l_file!=NULL) && (strlen(l_file) < 255))	
	{
		strncpy(LOCK_FILE,l_file,sizeof(LOCK_FILE));
		//printf("%s\n",LOCK_FILE);
	}
	else
	{
		strncpy(LOCK_FILE,"/tmp/mvd.lock",sizeof(LOCK_FILE));
	}

	lg_file=read_cfg(log_file);
	//printf("%s\n",lg_file);
	chomp(lg_file);
	if (lg_file!=NULL && (strlen(lg_file) < 255))
	{
		strncpy(LOG_FILE,lg_file,sizeof(LOG_FILE));
		//printf("%s\n",LOG_FILE);
	}
	else
	{
		strncpy(LOG_FILE,"/var/log/mvd.log",sizeof(LOG_FILE));
	}

	fr_dir=read_cfg(from_dir);
	//printf("%s",fr_dir);
	chomp(fr_dir);
	if (fr_dir!=NULL && (strlen(fr_dir) < 255))
	{
		strncpy(FROM_DIR,fr_dir,sizeof(FROM_DIR));
		//printf("%s\n",FROM_DIR);
	}
	else
	{
		strncpy(FROM_DIR,"/usr/local/samba/ftp/",sizeof(FROM_DIR));
	}

	todir=read_cfg(to_dir);
	//printf("%s\n",todir);
	chomp(todir);
	if (todir!=NULL && (strlen(todir) < 255))
	{
		strncpy(TO_DIR,todir,sizeof(TO_DIR));
		//printf("%s\n",TO_DIR);
	}
	else
	{
		strncpy(TO_DIR,"/usr/local/ftp/timur/",sizeof(TO_DIR));
	}
}

void log_message(char *filename, char *message)
{
	FILE *logfile;
        logfile=fopen(filename,"a");
        if(!logfile) return;
        fprintf(logfile,"%s\n",message);
        fclose(logfile);
}


void signal_handler(sig)
int sig;
{
	switch(sig) {
	case SIGHUP:
		log_message(LOG_FILE,"hangup signal catched");
		break;
	case SIGTERM:
		log_message(LOG_FILE,"terminate signal catched");
		exit(0);
		break;
	}

}

void my_errors(int num)
{
        static char *err[] = {
                "Path too long!!!\n",
        };
        //printf("%s",err[num]);
	log_message(LOG_FILE,err[num]);
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

void mv_dir()
{
	char *from;
        char *to;
        struct dirent **files;
        int count,i;
        //printf("%s%s%s%s%s","Remove file from ",FROM_DIR," to ",TO_DIR,"\n");
        count=scandir(FROM_DIR, &files, file_select,alphasort);
        for (i=1; i<count+1; i++)
        {
                from = my_str_comp(FROM_DIR, files[i-1]->d_name);
                //printf("%s ", from);
                to = my_str_comp(TO_DIR, files[i-1]->d_name);
                //printf("%s ",to);
                if (rename(from, to) == 0)
                {
                        //printf("%s\n", "Success");
			log_message(LOG_FILE, "Success");
                }
                else
                {
                        //printf("%s %d\n", "Unsuccess errno ", errno);
			log_message(LOG_FILE,"Unsuccess");
                }
                free(from);
                free(to);
        }

}

int list(const char *name, const struct stat *status, int type)
{
	if(type == FTW_NS){  return 0; }

	if(type == FTW_F)
	{ 
		//printf("0%3o\t%s\n", status->st_mode&0777, name); 
		chown(name, 2121, 2121); 
	}

	if(type == FTW_D && strcmp(".", name) != 0)
	{
		//printf("0%3o\t%s/\n", status->st_mode&0777, name); 
		chown(name, 2121, 2121); 
	}

	    return 0;
}

void ch()
{
	
	ftw(TO_DIR, list, 1);
}

void daemonize()
{
	int i,lfp;
	char str[10];
	if(getppid()==1) return; 
	i=fork();
	if (i<0) exit(1); 
	if (i>0) exit(0);
	
	setsid(); 
	for (i=getdtablesize();i>=0;--i) close(i);

	i=open("/dev/ttyp1",O_RDWR); dup(i); dup(i);
	umask(027); 
	chdir(RUNNING_DIR); 
	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
	if (lfp<0) exit(1); 
	if (lockf(lfp,F_TLOCK,0)<0) exit(0); 
	sprintf(str,"%d\n",getpid());
	write(lfp,str,strlen(str)); 
	signal(SIGCHLD,SIG_IGN); 
	signal(SIGTSTP,SIG_IGN); 
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signal_handler);
	signal(SIGTERM,signal_handler);
}



int main()
{
	set_main_vars();
	//printf("%s", RUNNING_DIR);
	//printf("%s", LOCK_FILE);
	//printf("%s", LOG_FILE);
	//printf("%s", FROM_DIR);
	//printf("%s", TO_DIR);

	daemonize();
	while(1)
	{
	     	sleep(1);
		mv_dir();
		ch();
	}
	return 0;
}

