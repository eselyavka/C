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
#include <ctype.h>
#include <syslog.h>
#include <time.h>
#include "ini.h"
#include <stdlib.h>

int MAXPATHLEN=1024;

int MAXUSERS=100;

char RUNNING_DIR[255];
char LOCK_FILE[255];
char LOG_FILE[255];
char DB_LOG_FILE[255];

typedef struct
{
    const char* rdir;
    const char* lockfile;
    const char* logfile;
    const char* dblogfile;
    const char* title;
    const char* from;
    const char* to;
    const char* email;
} config_section [100];

typedef struct 
{
	config_section sections;
} configuration;


char* my_time(char* outtime)
{
	char* timestr[100];
	outtime = (char *) calloc(strlen(timestr)+1, sizeof(char));
	struct tm *ptr;
	time_t lt;
	lt=time(NULL);
	ptr=localtime(&lt);
	strftime(timestr,99,"%Y-%m-%d %H:%M:%S",ptr);
	strcpy(outtime, timestr);
	return outtime;
}

static int handler(void* user, const char* section, const char* name, const char* value)
{
	configuration* pconfig = (configuration*)user;
	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("general", "runnig_dir"))
	{
		pconfig->sections[0].rdir = strdup(value);
	}
	else if (MATCH("general", "lock_file"))
	{
		pconfig->sections[0].lockfile = strdup(value);
	}
	else if (MATCH("general", "log_file"))
	{
		pconfig->sections[0].logfile = strdup(value);
	}	
	else if (MATCH("db", "db_log_file"))
	{
		pconfig->sections[0].dblogfile = strdup(value);
	}
	else if (strncmp(section, "user", 4) == 0)
	{
		int section_num = atoi(section+4)-1;
		if (section_num>=0 && section_num<MAXUSERS)
		{
			if (strcmp(name, "title") == 0)
			{
				pconfig->sections[section_num].title = strdup(value);
			}	
			else if (strcmp(name, "from_dir") == 0)
			{
				pconfig->sections[section_num].from = strdup(value);
			}
			else if (strcmp(name, "to_dir") == 0)
			{
				pconfig->sections[section_num].to = strdup(value);
			}
			else if (strcmp(name, "email") == 0)
			{
				pconfig->sections[section_num].email = strdup(value);
			}
		}
		else
		{
			syslog(LOG_WARNING, "Incorrect user or too much users, MAX is 100 or section num bigger then 100 or section isn't numeric\n");
			exit(1);
		}
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


void signal_handler(int sig)
{
	char *li;
	char *out;
	li = (char*) calloc(strlen(my_time(out))+strlen("\thangup signal catched")+strlen("\tterminate signal catched")+1,sizeof(char));
	strcpy(li,my_time(out));
	switch(sig) {
	case SIGHUP:
		strcat(li, "\thangup signal catched");
		log_message(LOG_FILE, li);
		free(li);
		break;
	case SIGTERM:
		strcat(li, "\tterminate signal catched");
		log_message(LOG_FILE, li);
		free(li);
		exit(0);
		break;
	}

}

void my_errors(int num)
{
	char *out;
	char *errstr;
        static char *err[] = {
                "Path too too long!",
        };
	errstr = (char *) calloc(strlen(err[num])+strlen("\t")+strlen(my_time(out))+1, sizeof(char));
	strcpy(errstr, my_time(out));
	strcat(errstr, "\t");
	strcat(errstr, err[num]);
	log_message(LOG_FILE,errstr);
	free(errstr);
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
        if ((strlen(str1)+strlen(str2)) >= MAXPATHLEN)
        {
              my_errors(0);
              exit(1);
        }
        else
        {
                str = (char *) malloc((strlen(str1) + strlen("/") + strlen(str2) +1) *sizeof(char));
		
                strcpy(str, str1);
		strcat(str, "/");
                strcat(str, str2);
                return str;
        }
}

void mv_dir(const char* NAME, const char* EMAIL, const char* FROM_DIR, const char* TO_DIR)
{
	char *from;
        char *to;
        struct dirent **files;
        int count,i;
	char *q1, *q2, *q3;
	char *insert="INSERT INTO file(date_move,fname,customer_name,customer_email,from_dir,to_dir,send_alert,count) VALUES ";
	char *select="SELECT * FROM file WHERE fname LIKE ";
	char *update="UPDATE file SET date_move='";
	char *buf;
	char *li;
	char *buftm;
	char *out;
	char *db_log;
        //printf("%s%s%s%s%s","Remove file from ",FROM_DIR," to ",TO_DIR,"\n");
        count=scandir(FROM_DIR, &files, file_select,alphasort);
        for (i=1; i<count+1; i++)
        {
                
		from = my_str_comp(FROM_DIR, files[i-1]->d_name);
                //printf("%s ", from);
                to = my_str_comp(TO_DIR, files[i-1]->d_name);
		buftm=(char *) calloc(strlen(my_time(out))+1,sizeof(char));
		strcpy(buftm,my_time(out));
		q1=(char *) calloc(strlen(insert)+strlen("('")+strlen(buftm)+strlen("', '")+strlen(files[i-1]->d_name)+strlen("', '")+strlen(NAME)+strlen("', '")+strlen(EMAIL)+strlen("', '")+strlen(FROM_DIR)+strlen("', '")+strlen(TO_DIR)+strlen("', '0',0)")+1,sizeof(char));
		strcpy(q1,insert);
		strcat(q1,"('");
		strcat(q1,buftm);
		strcat(q1,"', '");
		strcat(q1,files[i-1]->d_name);
		strcat(q1,"', '");
		strcat(q1,NAME);
		strcat(q1,"', '");
		strcat(q1,EMAIL);
		strcat(q1,"', '");
		strcat(q1,FROM_DIR);
		strcat(q1,"', '");
		strcat(q1,TO_DIR);
		strcat(q1,"', '0',0)");
		q2 =(char *) calloc(strlen(select)+strlen("'")+strlen(files[i-1]->d_name)+strlen("' AND customer_name like '")+strlen(NAME)+strlen("'")+1, sizeof(char));
		strcpy(q2, select);
		strcat(q2, "'");
		strcat(q2, files[i-1]->d_name);
		strcat(q2, "' AND customer_name like '");
		strcat(q2, NAME);
		strcat(q2, "'");
		q3 =(char *) calloc(strlen(update)+strlen(buftm)+strlen("', send_alert=0, count=count+1 WHERE fname like '")+strlen(files[i-1]->d_name)+strlen("' AND customer_name like '")+strlen(NAME)+strlen("'")+1, sizeof(char));
		strcpy(q3, update);
		strcat(q3, buftm);
		strcat(q3, "', send_alert=0, count=count+1 WHERE fname like '");
		strcat(q3, files[i-1]->d_name);
		strcat(q3, "' AND customer_name like '");
		strcat(q3, NAME);
		strcat(q3, "'");

		//printf("%s\n",q3);
		//log_message(LOG_FILE,q1);
                //printf("%s ",to);
                if (rename(from, to) == 0)
                {
			li=(char*) calloc(strlen(buftm)+strlen("\tSuccess move file ")+strlen(from)+strlen("\tto\t")+strlen(to)+1, sizeof(char));
			strcpy(li, buftm);
			strcat(li,"\tSuccess move file ");
			strcat(li, from);
			strcat(li,"\tto\t");
			strcat(li, to);
			log_message(LOG_FILE, li);
                }
                else
                {
			li=(char*) calloc(strlen(buftm)+strlen("\tUnsuccess move file ")+strlen(from)+strlen("\tto\t")+strlen(to)+1, sizeof(char));
			strcpy(li, buftm);
			strcat(li,"\tUnsuccess move file ");
			strcat(li, from);
			strcat(li,"\tto\t");
			strcat(li, to);
			log_message(LOG_FILE, li);
                }
		
		if (imysql(DB_LOG_FILE, buftm, q2, "s"))
		{
			if(imysql(DB_LOG_FILE, buftm, q3, "u") == NULL)
			{
				db_log=(char *) calloc(strlen(buftm)+strlen("\t")+strlen("Data in db for file ")+strlen(files[i-1]->d_name)+strlen(" successfully updated")+1, sizeof(char));
				strcpy(db_log, buftm);
				strcat(db_log, "\t");
				strcat(db_log, "Data in db for file ");
				strcat(db_log, files[i-1]->d_name);
				strcat(db_log, " successfully updated");
				log_message(LOG_FILE, db_log);
			}
			else
			{
				db_log=(char *) calloc(strlen(buftm)+strlen("\t")+strlen("Error: Data in db for file ")+strlen(files[i-1]->d_name)+strlen(" unsuccessfully updated")+1, sizeof(char));
				strcpy(db_log, buftm);
				strcat(db_log, "\t");
				strcat(db_log, "Error: Data in db for file ");
				strcat(db_log, files[i-1]->d_name);
				strcat(db_log, " unsuccessfully updated");
				log_message(LOG_FILE, db_log);

			}
		}
		else
		{
			if (imysql(DB_LOG_FILE, buftm, q1, "i") == NULL)
			{
				db_log=(char *) calloc(strlen(buftm)+strlen("\t")+strlen("successfully insert data in DB")+1, sizeof(char));	
				strcpy(db_log, buftm);
				strcat(db_log, "\t");
				strcat(db_log, "successfully insert data in DB");
				log_message(LOG_FILE, db_log);
			}
			else
			{
				db_log=(char *) calloc(strlen(buftm)+strlen("\t")+strlen("Error: can't insert data in DB, see db_log_file")+1, sizeof(char));	
				strcpy(db_log, buftm);
				strcat(db_log, "\t");
				strcat(db_log, "Error: can't insert data in DB, see db_log_file");
				log_message(LOG_FILE, db_log);
			}
		}

                free(from);
                free(to);
		free(buftm);
		free(db_log);
		free(li);
		free(q1);
		free(q2);
		free(q3);
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

void ch(const char* TO_DIR)
{
	
	ftw(TO_DIR, list, 1);
}

void daemonize(char* RUNNING_DIR, char* LOCK_FILE)
{
	int i,lfp;
	char str[10];
	if(getppid()==1) return; 
	i=fork();
	if (i<0) exit(1); 
	if (i>0) exit(0);
	
	setsid(); 
	for (i=getdtablesize();i>=0;--i) close(i);

	i=open("/dev/ttyp0",O_RDWR); dup(i); dup(i);
	umask(027); 
	chdir(RUNNING_DIR); 
	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
	if (lfp<0)
	{
		syslog(LOG_ERR, "Can't create lock file: %s", LOCK_FILE);
		exit(1); 
	}
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

int file_exists(const char* filename)
{
	FILE* pfile;
	pfile = fopen(filename, "r");
	
	if (pfile != NULL)
	{
		fclose(pfile);
	       	return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
        char *running_dir="[A-Za-z1-9_/]";
	char *lock_file="[A-Za-z1-9_/\.]";
	char *log_file="[A-Za-z1-9_/]";
	char *db_log="[A-Za-z1-9_/]";

	char *title="[A-Za-z1-9_/]";
	char *from_dir="[A-Za-z1-9_/]";
	char *to_dir="[A-Za-z1-9_/]";
	char *email="[A-Za-z1-9_/]";

	char *bufstr;
	char *out;

	FILE* pfile;

	char *inform_message_good[]={
		"Use as runnig dir: ",
		"Use as lock file: ",
		"Use as source dir: ",
		"Use as target dir: ",
		"Use as log file: ",
		"Use as db log file: ",	
	};

	char *inform_message_bad[]={
		"No such directory of directory consists of impossible characters. Please use [A-Za-z1-9_/]!. Daemon run in default directory /tmp. Error dir: ",
		"No such file of file consists of impossible characters. Please use [A-Za-z1-9_/]!. Daemon run with default lock file /var/run/mvd.pid. Error lock file: ",	
		"No such directory of directory consists of impossible characters. Please use [A-Za-z1-9_/]!. Daemon can't run . Error source dir: ",
		"No such directory of directory consists of impossible characters. Please use [A-Za-z1-9_/]!. Daemon can't run . Error target dir: ",
		"No such file of file consists of impossible characters. Please use [A-Za-z1-9_/]!. Daemon can't run. Error log file: ",
		"No such file of file consists of impossible characters. Please use [A-Za-z1-9_/]!. Daemon can't run. Error db log file: ",
	};
	struct stat isdir;
        configuration config;
	int k;
	for (k=0; k<MAXUSERS; k++)
	{
		config.sections[k].rdir=NULL;
		config.sections[k].lockfile=NULL;
		config.sections[k].logfile=NULL;
		config.sections[k].dblogfile=NULL;
		config.sections[k].title=NULL;
		config.sections[k].from=NULL;
		config.sections[k].to=NULL;
		config.sections[k].email=NULL;

	}
	int j;
	if (argc > 1)
	{
                for (j=0; j<argc; j++)
		{
			if (strcmp(argv[j],"-c") == 0)
			{
				if(file_exists(argv[j+1]))
				{
					if (ini_parse(argv[j+1], handler, &config) < 0)
					{
						syslog(LOG_ALERT,"%s%s\n", "Can't load ",argv[j+1]);
						exit(1);
					}
				}
				else
				{
					syslog(LOG_ALERT,"%s%s\n", "Can't load ",argv[j+1]);
					exit(1);
				}
			}
			if (strcmp(argv[j], "-h") == 0)
			{
				printf("Usage help for %s\n", argv[0]);
				printf("Use -c for start daemon with custom config file, default file is /usr/local/etc/mvd/mvd.conf\n");
				printf("Use -h to see this help\n");
				exit(0);
			}
		}	
	}
	else
	{
		if (ini_parse("/usr/local/etc/mvd/mvd.conf", handler, &config) < 0)
		{
			syslog(LOG_ALERT, "Can't load 'mvd.conf'\n");
			exit(1);
		}

	}
	
	if (pcre_comp(log_file, config.sections[0].logfile) && file_exists(config.sections[0].logfile))
	{
		syslog(LOG_NOTICE, "%s%s\n",inform_message_good[4],config.sections[0].logfile);
		strncpy(LOG_FILE,config.sections[0].logfile,sizeof(LOG_FILE));
	}
	else
	{
		syslog(LOG_ERR, "%s%s\n",inform_message_bad[4],config.sections[0].logfile);
		exit(1);
	}	
	
	if (pcre_comp(db_log, config.sections[0].dblogfile) && file_exists(config.sections[0].dblogfile))
	{
		syslog(LOG_NOTICE, "%s%s\n",inform_message_good[5],config.sections[0].dblogfile);
		strncpy(DB_LOG_FILE,config.sections[0].dblogfile,sizeof(DB_LOG_FILE));
	}
	else
	{
		syslog(LOG_ERR, "%s%s\n",inform_message_bad[5],config.sections[0].dblogfile);
		exit(1);
	}	

	if (pcre_comp(running_dir, config.sections[0].rdir) && stat(config.sections[0].rdir, &isdir) == 0)
	{
		bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_good[0])+strlen(config.sections[0].rdir)+1,sizeof(char));
		strcpy(bufstr,my_time(out));
		strcat(bufstr,"\t");
		strcat(bufstr, inform_message_good[0]);
		strcat(bufstr, config.sections[0].rdir);
		log_message(LOG_FILE, bufstr);
		syslog(LOG_NOTICE, "%s%s\n",inform_message_good[0],config.sections[0].rdir);
		free(bufstr);
	}
	else
	{
		bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_bad[0])+strlen(config.sections[0].rdir)+1,sizeof(char));
		strcpy(bufstr, my_time(out));
		strcat(bufstr, "\t");
		strcat(bufstr, inform_message_bad[0]);
		strcat(bufstr, config.sections[0].rdir);
		syslog(LOG_WARNING, "%s%s\n",inform_message_bad[0],config.sections[0].rdir);
		log_message(LOG_FILE, bufstr);
		strncpy(RUNNING_DIR, "/tmp", sizeof(RUNNING_DIR));
		free(bufstr);
	}
	
	if (pcre_comp(lock_file, config.sections[0].lockfile) && file_exists(config.sections[0].lockfile))
	{
		bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_good[1])+strlen(config.sections[0].lockfile)+1,sizeof(char));
		strcpy(bufstr,my_time(out));
		strcat(bufstr,"\t");
		strcat(bufstr, inform_message_good[1]);
		strcat(bufstr, config.sections[0].lockfile);
		log_message(LOG_FILE, bufstr);
		syslog(LOG_NOTICE, "%s%s\n",inform_message_good[1],config.sections[0].lockfile);
		free(bufstr);
	}
	else
	{
		pfile = fopen("/var/run/mvd.pid", "a");
	        fclose(pfile);	
		bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_bad[1])+strlen(config.sections[0].lockfile)+1,sizeof(char));
		strcpy(bufstr, my_time(out));
		strcat(bufstr, "\t");
		strcat(bufstr, inform_message_bad[1]);
		strcat(bufstr, config.sections[0].lockfile);
		syslog(LOG_WARNING, "%s%s\n",inform_message_bad[1],config.sections[0].lockfile);
		log_message(LOG_FILE, bufstr);
		strncpy(LOCK_FILE, "/var/run/mvd.pid", sizeof(LOCK_FILE));
		free(bufstr);
	}

	int i;
	for ( i=0; i<MAXUSERS; i++)
	{	
		
		if ((config.sections[i].from != NULL) && (config.sections[i].to != NULL) && (config.sections[i].title != NULL) && (config.sections[i].email != NULL))
		{
			if ((pcre_comp(from_dir, config.sections[i].from)) && (stat(config.sections[i].from, &isdir) == 0))
			{
				bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_good[2])+strlen(config.sections[i].from)+1,sizeof(char));
				strcpy(bufstr,my_time(out));
				strcat(bufstr,"\t");
				strcat(bufstr, inform_message_good[2]);
				strcat(bufstr, config.sections[i].from);
				log_message(LOG_FILE, bufstr);
				free(bufstr);
			}
			else
			{
				bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_bad[2])+strlen(config.sections[i].from)+1,sizeof(char));
				strcpy(bufstr,my_time(out));
				strcat(bufstr,"\t");
				strcat(bufstr, inform_message_bad[2]);
				strcat(bufstr, config.sections[i].from);
				log_message(LOG_FILE, bufstr);
				syslog(LOG_ERR, "%s%s\n", inform_message_bad[2],config.sections[i].from);
				free(bufstr);
				exit(1);
			}

			if (pcre_comp(to_dir, config.sections[i].to) && stat(config.sections[i].to, &isdir) == 0)
			{
				bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_good[3])+strlen(config.sections[i].to)+1,sizeof(char));
				strcpy(bufstr,my_time(out));
				strcat(bufstr,"\t");
				strcat(bufstr, inform_message_good[3]);
				strcat(bufstr, config.sections[i].to);
				log_message(LOG_FILE, bufstr);
				free(bufstr);
			}
			else
			{
				bufstr = (char *) calloc(strlen(my_time(out))+strlen("\t")+strlen(inform_message_bad[3])+strlen(config.sections[i].to)+1,sizeof(char));
				strcpy(bufstr,my_time(out));
				strcat(bufstr,"\t");
				strcat(bufstr, inform_message_bad[3]);
				strcat(bufstr, config.sections[i].to);
				log_message(LOG_FILE, bufstr);
				syslog(LOG_ERR, "%s%s\n",inform_message_bad[3],config.sections[i].to);
				free(bufstr);
				exit(1);
			}
			//printf("%s\n", config.sections[i].title);
		}
	}
	//imysql("select * from file");
	//printf("%s", RUNNING_DIR);
	//printf("%s", LOCK_FILE);
	//printf("%s", LOG_FILE);
	//printf("%s", FROM_DIR);
	//printf("%s", TO_DIR);
	daemonize(config.sections[0].rdir, config.sections[0].lockfile);
	while(1)
	{
	     	sleep(1);
		for (i=0; i<=MAXUSERS; i++)
		{
			if ((config.sections[i].from != NULL) && (config.sections[i].to != NULL) && (config.sections[i].title != NULL) && (config.sections[i].email != NULL))
			{
				mv_dir(config.sections[i].title, config.sections[i].email, config.sections[i].from, config.sections[i].to);
				ch(config.sections[i].to);
			}
		}
	}
	return 0;
}

