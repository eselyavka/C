#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/local/include/libpq-fe.h"

static void exit_nicely(PGconn *conn);
void read_cfg(char *dbname_value, char *dbuser_value, char *dbpasswd_value);
void usage(int myerr, char *prog_name);

int main(int argc, char **argv)
{
	PGconn *conn;
	PGresult *result;
	int nFields,i,j;
	
	char dbname_value[80];	
	char dbuser_value[80];	
	char dbpasswd_value[80];	
	char dbconnectstr[240];
	char query[1000];
	
	if (argc <= 1)
	{
		usage(1,argv[0]);
	}
	
	read_cfg(&dbname_value, &dbuser_value, &dbpasswd_value);
	
	strcpy(dbconnectstr,"dbname=");
	strcat(dbconnectstr,dbname_value);
	strcat(dbconnectstr," user='");
	strcat(dbconnectstr,dbuser_value);
	strcat(dbconnectstr,"' password='");
	strcat(dbconnectstr,dbpasswd_value);
	strcat(dbconnectstr,"'");
	
	conn = PQconnectdb(dbconnectstr);
    	
	if (PQstatus(conn) == CONNECTION_BAD)          
    {
	    fprintf(stderr, "Connection to database failed.\n");
	    fprintf(stderr, "%s", PQerrorMessage(conn));
	    exit(1);
	}
	
	strcpy(query,"select * from users where username like '");
	strcat(query,argv[1]);
	strcat(query,"'");
	
	result=PQexec(conn,query);
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query error: %s\n", PQerrorMessage(conn));
        PQclear(result);
        exit_nicely(conn);
    }

	nFields = PQnfields(result);
	//printf("%d\n",nFields);
    /*for (i = 0; i < nFields; i++)
	{
		printf("%-15s", PQfname(result, i));
		printf("\n\n");
	}*/
	
	for (i = 0; i < PQntuples(result); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(result, i, j));
		printf("\n");
    }

	PQclear(result);
	PQfinish(conn);
}

void read_cfg(char *dbname_value, char *dbuser_value, char *dbpasswd_value)
{
	FILE *file;
	file = fopen("dbconfig.cfg", "r"); 
	rewind(file);
	fseek(file,0,SEEK_END);
	long int size=ftell(file);
	rewind(file);
	
	size_t code_s, result;
	char *code;
	
	char dbname[80];	
	char dbuser[80];	
	char dbpasswd[80];	
		
	if(file==NULL) 
	{
		printf("Error: can't open file dbconfig.cfg.\n");
		exit(1);
	}
	else 
	{
		code_s=sizeof(char) * size;
		code=malloc(code_s);
		result = fread(code, 1, size, file);
		sscanf(code, "%[a-zA-Z]=%[a-zA-Z1-9]\n%[a-zA-Z]=%[a-zA-Z1-9]\n%[a-zA-Z]=%[a-zA-Z1-9]", dbname, dbname_value, dbuser, dbuser_value, dbpasswd, dbpasswd_value);
		fclose(file);
	}
}

static void 	exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

void usage(int myerr,char *prog_name)
{
	switch(myerr)
	{
		case 1:
		printf("Too few arguments!\n\n\tUsage:\n\t%s rznum\n",prog_name);
		break;
	}
	exit(1);
}