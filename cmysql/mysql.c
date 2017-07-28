#include "/usr/local/include/mysql/mysql.h" 
#include <stdio.h>

void my_select (void) {
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;

   char *server = "localhost";
   char *user = "cmysql";
   char *password = "123321";
   char *database = "file_manager";
   
   conn = mysql_init(NULL);
   
   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      //return(0);
   }

   /* send SQL query */
   if (mysql_query(conn, "SELECT * FROM file")) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      //return(0);
   }

   res = mysql_use_result(conn);
   
   /* output fields 1 and 2 of each row */
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s %s\n", row[1], row[2]);

   /* Release memory used to store results and close connection */
   mysql_free_result(res);
   mysql_close(conn);
}

