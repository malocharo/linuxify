#include "linuxify.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>
#include <errno.h>

void usage(const char* name){
    printf("Usage : %s  path to the file name\n",name);
    return;
}

int check_file_access(const char* path){
    if(access(path,F_OK)){
        fprintf(stderr,"The specified file does not exist : %s\n",path);
        return -1;
    }
    return 0;
}

int contains_file(const char* path) {
    return (path && *path && path[strlen(path) - 1] == '/') ? 0 : 1;
}

int get_file_name(char* path, char* name_buffer, int buff_size){
    char * tmp_name;

    if(!contains_file(path)){
        fprintf(stderr,"path doesn't end with a file : %s\n",path);
        return -1;
    }

    tmp_name = basename(path);
    if(strlen(tmp_name) > buff_size){
        fprintf(stderr,"file name size is superior at 255, aborting\n");
        return -1;
    }

    if(name_buffer == NULL){
        fprintf(stderr,"Return buffer isn't initiated\n");
        return -1;
    }

    strncpy(name_buffer,tmp_name,buff_size);
    return 0;
}

int get_dir_name(char*path, char* path_buffer,int buff_size) {

    char * tmp_dir;

    if(path_buffer == NULL){
        fprintf(stderr,"Return buffer isn't initiated\n");
        return -1;
    }

    tmp_dir = dirname(path);

    if(strlen(tmp_dir) > buff_size){
        fprintf(stderr,"file name size is superior at 255, aborting\n");
        return -1;
    }

    strncpy(path_buffer,path,buff_size);
    return 0;

}

int get_file_change_name(const char * file_name, char* new_file_name_buffer, char space_attr, int new_file_buff_size)
{
    if(new_file_name_buffer == NULL){
        fprintf(stderr,"Return buffer isn't initiated\n");
        return -1;
    }

    strncpy(new_file_name_buffer,file_name,new_file_buff_size);

    while(*new_file_name_buffer){
        if(isspace(*new_file_name_buffer))
            *new_file_name_buffer = space_attr;
        new_file_name_buffer++;
    }

    return 0;
}

int change_name(const char* old_name, const char* new_name, const char* dir_name) {
    if(old_name == NULL || new_name == NULL || dir_name == NULL) {
        fprintf(stderr,"One argument is missing\n");
        return -1;
    }


    char *full_new_path = malloc(sizeof(char)*(strlen(new_name)+strlen(dir_name)+2));
    strcpy(full_new_path,dir_name);
    strcat(full_new_path,"/");
    strcat(full_new_path,new_name);

   /* printf("%s\n",old_name);
    printf("%s\n",full_new_path);*/

    if(rename(old_name,full_new_path)) {
        fprintf(stderr,"Error while using rename()\n");
        perror("");
        return -1;
    }
    free(full_new_path);

    return 0;
}


int main(int argc, char** argv)
{
    if(argc != 2){
        usage(argv[0]);
        exit(-1);
    }

    if(check_file_access(argv[1])) {
        fprintf(stderr,"Error while parsing file name\n");
        exit(-1);
    }

    char file_name[FILENAME_BUFFER_SIZE];
    char dir_name[DIR_BUFFER_SIZE];
    char full_path[FILENAME_BUFFER_SIZE + DIR_BUFFER_SIZE];
    strncpy(full_path,argv[1],FILENAME_BUFFER_SIZE + DIR_BUFFER_SIZE);

    if(get_file_name(argv[1],file_name,FILENAME_BUFFER_SIZE)) {
        fprintf(stderr,"Error while extracting file name\n");
        exit(-1);
    }

    if(get_dir_name(argv[1],dir_name,DIR_BUFFER_SIZE)){
        fprintf(stderr,"Error while extracting dir name\n");
        exit(-1);
    }

    char new_file_name[FILENAME_BUFFER_SIZE];

    if(get_file_change_name(file_name,new_file_name,'_',FILENAME_BUFFER_SIZE)){
        fprintf(stderr,"Error while creating new file name\n");
        exit(-1);
    }

    //fprintf(stdout,"[+] file name is %s\n",new_file_name);

    if(change_name(full_path,new_file_name,dir_name)){
        fprintf(stderr,"Error while changing file name\n");
        exit(-1);
    }

    exit(0);
}