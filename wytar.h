/*
 * wytar.h
 * Lucas Manker
 * 3/7/20
 * COSC 3750
 * Homework 6
 *
 * header file for wytar
*/

#include<tar.h>
#include<string.h>

struct tar_header{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
};

void setfields(struct tar_header *header){
    strncpy(header->magic, TMAGIC, TMAGLEN);
    strncpy(header->version, TVERSION, TVERSLEN);
    header->typeflag = '0';
    strcpy(header->devmajor, "\0");
    strcpy(header->devminor, "\0");
    return;
}

