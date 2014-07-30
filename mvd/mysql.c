#include "/usr/local/include/mysql/mysql.h" 
#include <stdio.h>

int imysql (const char *file, const char *date, const char *query, const char *qtype) 
{
	MYSQL *conn;
   	MYSQL_RES *res;
   	MYSQL_ROW row;
   	FILE *pfile;

   	char *server = "localhost";
   	char *user = "user";
   	char *password = "password";
   	char *database = "database";
   	char *strbuf;
	int num_rows;

   	conn = mysql_init(NULL);
   	pfile = fopen (file,"a+");

   	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
   	{
   		strbuf=(char *) calloc(strlen(date)+strlen("\t")+strlen(mysql_error(conn))+1, sizeof(char));
	   	strcpy(strbuf, date);
	   	strcat(strbuf, "\t");
	   	strcat(strbuf,mysql_error(conn));
	   	fprintf(pfile, "%s\n", strbuf);
   	}

   	if (mysql_query(conn, query)) 
   	{
   		strbuf=(char *) calloc(strlen(date)+strlen("\t")+strlen(mysql_error(conn))+1, sizeof(char));
	   	strcpy(strbuf, date);
	   	strcat(strbuf, "\t");
	   	strcat(strbuf,mysql_error(conn));
	   	fprintf(pfile, "%s\n", strbuf);
   	}
   	
	res = mysql_store_result(conn);
   	fclose(pfile);
   	//printf("file=%s\tdate=%s\tquery=%s\n",file,date,query);
	
	if (strcmp(qtype,"s") == NULL)
	{
		num_rows = mysql_num_rows(res);
   		if( num_rows != 0 )
   		{
			mysql_free_result(res);
   			mysql_close(conn);
   			return 1;
   		}
   		else
   		{
			mysql_free_result(res);
   			mysql_close(conn);
   			return 0;
   		}
	}
	else if (strcmp(qtype, "i") == NULL)
	{
   		mysql_free_result(res);
   		mysql_close(conn);
		return 0;
	}
	else if (strcmp(qtype, "u") == NULL) 
	{
   		mysql_free_result(res);
   		mysql_close(conn);
		return 0;
	}
	else
	{
   		mysql_free_result(res);
   		mysql_close(conn);
		return 0;
	}
}

