#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

static int cmpstringp(const void *p1, const void *p2) {
           return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int main(int argc, char *argv[]) {
  char *directory;
  struct dirent *ent;
  DIR *dir_stream;
  char **files = NULL;
  if (argc < 2) {
    printf("Please specify directory to read\n");
    exit(1);
  } else {
    directory = (char *) malloc(strlen(argv[1]) + 1);
    strcpy(directory, argv[1]);
    if (!(dir_stream = opendir(directory))) {
      printf("can't access directory %s", directory);
      exit(1);
    }
    size_t files_len = 0;
    while ((ent = readdir(dir_stream))) {
      size_t len = strlen(ent->d_name);
      if (len == 0 || ent->d_name[0] == '.')
        continue;
      files_len += 1;
      if (files == NULL) {
        files = (char **) malloc(files_len * sizeof(char *));
      } else {
        files = (char **) realloc(files, files_len * sizeof(char *));
      }
      files[files_len - 1] = (char *) malloc(strlen(ent->d_name) + 1);
      strcpy(files[files_len - 1], ent->d_name);
    }

    qsort(files, files_len, sizeof(char *), cmpstringp);

    char *path;
    size_t i;
    for (i = 0; i<files_len; i++){
      path = (char *) malloc(strlen(files[i]) + strlen(directory) + 2);
      strcpy(path, directory);
      strcat(path, "/");
      strcat(path, files[i]);
      printf("%s\n", path);
    }

    free(directory);
    free(path);
    free(files);
  }
  return 0;
}
