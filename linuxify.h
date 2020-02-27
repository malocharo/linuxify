#ifndef LINUXIFY_H
#define LINUXIFY_H
#include <stdlib.h>

#define FILENAME_BUFFER_SIZE 255
#define DIR_BUFFER_SIZE 4096


void inline usage(const char*);
int inline contains_file(const char *);
int check_file_access(const char *);
int get_file_name(char*, char*, int);
int get_dir_name(char*, char*, int);
int get_file_change_name(const char *, char*, char, int);
int change_name(const char*, const char*, const char*);

#endif //linuxify