#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void inverter(char *);
void help(char *);
int getlen(char *);

int main(int argc, char *argv[]) {

  if (*(argv + 1) == NULL ) {
    help(argv[0]);
    exit(0);
  }

  if ( (strncmp("-h", argv[1], strlen("-h")) == 0) || (strncmp("--help", argv[1], strlen("--help"))) == 0 ) {
    help(argv[0]);
    exit(0);
  }

  if( argc == 2 ) {
    inverter( argv[1] ); 
  } else {
    printf("To many arguments\n");
    help(argv[0]);
  }

  return 0;
}

void help(char *scriptName) {
  printf("\tUsage: %s <string>\n\n\tExample: test->tset\n", scriptName);
}

void inverter(char *words) {
  int i;
  for (i=getlen(words)-1; i>=0; i--) {
    printf("%c",words[i]);
  }
  printf("\n");
}

int getlen(char *words) {
    return strlen(words);
}
