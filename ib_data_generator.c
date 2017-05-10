#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
  Generate binary data in ib format
  for table with structure

  CREATE TABLE test_bin
  (
     d DATE,
     f1       INT,
     f2       SMALLINT,
     f3       TINYINT,
     f4       VARCHAR(5),
     f5       INT,
     f6       BIGINT,
     f7       BIGINT
  );
 */

struct varchar {
    short L;
    char *payload;
} __attribute__((packed));

struct IBpayload {
    int d;
    int f1;
    short f2;
    char f3;
    struct varchar *f4;
    int f5;
    long f6;
    long f7;
} __attribute__((packed));

struct IBrow {
    short L;
    unsigned char n;
    struct IBpayload payload;
} __attribute__((packed));

int main(void) {
    struct IBrow irow; 
    struct varchar f4;
    char *payload="aBcDe";
    FILE *fp;
    irow.payload.d=1160831;
    irow.payload.f1=4432;
    irow.payload.f2=23865;
    irow.payload.f3=127;
    f4.L=strlen(payload);
    f4.payload = (char *) malloc(sizeof(char) * f4.L);
    strncpy(f4.payload, payload, f4.L);
    irow.payload.f4=&f4;
    irow.payload.f5=10000000;
    irow.payload.f6=LLONG_MAX;
    irow.payload.f7=LLONG_MAX;
    irow.n=0b00000000;
    irow.L=sizeof(irow.payload);
    fp=fopen("/tmp/myib.bin", "wb");
    fwrite(&irow.L, sizeof(short), 1, fp);
    fwrite(&irow.n, sizeof(unsigned char), 1, fp);
    fwrite(&irow.payload.d, sizeof(int), 1, fp);
    fwrite(&irow.payload.f1, sizeof(int), 1, fp);
    fwrite(&irow.payload.f2, sizeof(short), 1, fp);
    fwrite(&irow.payload.f3, sizeof(char), 1, fp);
    fwrite(&f4.L, sizeof(short), 1, fp);
    fwrite(f4.payload, sizeof(char), f4.L, fp);
    fwrite(&irow.payload.f5, sizeof(int), 1, fp);
    fwrite(&irow.payload.f6, sizeof(long), 1, fp);
    fwrite(&irow.payload.f7, sizeof(long), 1, fp);
    fclose(fp);
    free(f4.payload);
    return 0;
}
