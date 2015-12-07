#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEGABYTE 1024*1024

int main()
{
    char str[10];
    unsigned long mem=0;
    void *myblock = NULL;

    while (strcmp(str, "end")) {
        printf("-> ");
        scanf("%s", &str);
        printf("\n");
        mem=atoi(str);
        myblock = (void *) malloc(mem*MEGABYTE);
        if (!myblock) {
            perror("Error: ");
            break;
        }
        memset(myblock, 1, mem*MEGABYTE);
        printf("Currently allocating %ld MB\n", mem);
    }
 
    return 0;
}
