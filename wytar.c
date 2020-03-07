/*
 * wytar.c
 * Lucas Manker
 * 3/7/20
 * COSC 3750
 * Homework 6
 *
 * mimics tar
*/

#include<stdio.h>
#include<tar.h>
#include "wytar.h"
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>


struct tar_header* makeHeader(char *file);
void processDir(char* dir);
void processFile(char* file);
void extraction(char* dir, char* target);

int main(int argc, char** argv){
    int flags[3];
    flags[0] = 0;
    flags[1] = 0;
    flags[2] = 0;
    int optionsFlag = 0;
    int i;
    for(i = 1; i < argc; i++){
        const char first = argv[i][0];
        char *perCh;
        perCh = strchr(&first, '-');
        size_t num = perCh-&first;
        if(num < 2 && !optionsFlag && i < 4 && !flags[2]){
            int j;
            for(j = 1; j < strlen(argv[i]); j++){
                const char next = argv[i][j];
                char *peChc;
                peChc = strchr(&next, 'c');
                size_t cSize = peChc - &next;
                char *peChx;
                peChx = strchr(&next, 'x');
                size_t xSize = peChx - &next;
                char *peChf;
                peChf = strchr(&next, 'f');
                size_t fSize = peChf - &next;
                if(cSize < 2){
                    if(flags[1]) printf("incompatible flags\n");
                    else flags[0] = 1;
                }
                else if(xSize < 2){
                    if(flags[0]) printf("incompatible flags\n");
                    else flags[1] = 1;
                }
                else if(fSize < 2){
                    if(flags[2]){
                        printf("more than one instance of -f found\n");
                        exit(1);
                    }
                    else flags[2] = 1;
                }
                else{ 
                    printf("%.*s command not found\n",1,&next);
                    exit(1);
                }
            }
        }
        else if(flags[0] && flags[2] && !optionsFlag){
            printf("target file for compression is %s\n", argv[i]);
            optionsFlag = 1;
        }
        else if(flags[1] && flags[2] && !optionsFlag){
            if(!argv[i+1]){
                printf("Specify an output file\n");
                exit(2);
            }
            else{
                extraction(argv[i], argv[i+1]);
                break;
            }
        }
        else{
            DIR *d;
            d = opendir(argv[i]);
            if(!d){
                if(access(argv[i], F_OK) != -1){
                    processFile(argv[i]);
                }
                else{
                    printf("wytar: cannot access '%s': No Such file or "
                            "directory\n", argv[i]);
                }
            }
            else{
                processDir(argv[i]);
            }
        }
    }
    if((!flags[0] || !flags[1]) && !flags[2]){
        printf("incorrect options\nusage: \n-x extract \n-c create archive\
                \n-f filename to process\n");
    }
    return 0;
}

void processDir(char* direct){
    printf("processing directory %s\n", direct);
    struct dirent *dir;
    DIR *d;
    d = opendir(direct);
    while((dir=readdir(d)) != NULL){
        const char firstLet = dir->d_name[0];
        char *perCh;
        perCh = strchr(&firstLet, '.');
        size_t num = perCh - &firstLet;
        if(num > 2){
            DIR *tempD;
            char *fullPath = (char*) malloc(2+strlen(direct) + strlen(dir->d_name));
            strcpy(fullPath, direct);
            strcat(fullPath, dir->d_name);
            tempD = opendir(fullPath);
            if(!tempD){
                strcat(fullPath,"\0");
                processFile(fullPath);
                free(fullPath);
            }
            else {
                strcat(fullPath,"/");
                processDir(fullPath);
                free(fullPath);
            }
        }
    }
}

void processFile(char* file){
    printf("processing file %s\n", file);
}

void extraction(char* dir, char* target){
    printf("extracting file: %s\n", dir);
    printf("target of extraction: %s\n", target);
}

struct tar_header* makeHeader(char *file){
    struct tar_header *header = (struct tar_header *)\
        malloc(sizeof(struct tar_header));
    setfields(header);
    return header;
}

