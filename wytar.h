/*
 * wytar.h
 * 3-7-20
 * COSC 3750
 * hw6
 * Declares struct for wytar.c
 */

#include <tar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct tar_header { // byte offset
    char name[100]; // 0
    char mode[8]; // 100
    char uid[8]; // 108
    char gid[8]; // 116
    char size[12]; // 124
    char mtime[12]; //136
    char chksum[8]; //148
    char typeflag; // 156
    char linkname[100]; //157
    char magic[6]; // 257
    char version[2]; // 263
    char uname[32]; // 265
    char gname[12]; // 297
    char devmajor[8]; // 329
    char devminor[8]; // 337
    char prefix[155]; // 345
    char pad[12]; // 500
};

void set_const_fields(struct tar_header *header) {
    strncpy(header->magic, TMAGIC, TMAGLEN);
    strncpy(header->version, TVERSION, TVERSLEN);
    // Assume everything is a regular file
    header->typeflag = '0';
    strcpy(header->devmajor, "\0");
    strcpy(header->devminor, "\0");
}

struct tar_header *make_header(char *file) {
    struct tar_header *h = (struct tar_header *)malloc(sizeof(struct tar_header));
    set_const_fields(h);

    struct stat s;
    lstat(file, &s);

    // Name
    snprintf(h->name, 100, "%s", file);

    // Mode
    mode_t bits = s.st_mode;
    int sperms = 0, operms = 0, gperms = 0, wperms = 0;
    (bits & S_ISUID) ? sperms += 4 : 0;
    (bits & S_ISGID) ? sperms += 2 : 0;
    (bits & S_ISVTX) ? sperms += 1 : 0;
    (bits & S_IRUSR) ? operms += 4 : 0;
    (bits & S_IWUSR) ? operms += 2 : 0;
    (bits & S_IXUSR) ? operms += 1 : 0;
    (bits & S_IRGRP) ? gperms += 4 : 0;
    (bits & S_IWGRP) ? gperms += 2 : 0;
    (bits & S_IXGRP) ? gperms += 1 : 0;
    (bits & S_IROTH) ? wperms += 4 : 0;
    (bits & S_IWOTH) ? wperms += 2 : 0;
    (bits & S_IXOTH) ? wperms += 1 : 0;
    snprintf(h->mode, 8, "000%d%d%d%d", sperms, operms, gperms, wperms);

    // UID and GID
    snprintf(h->uid, 8, "%07do", s.st_uid);
    snprintf(h->gid, 8, "%07do", s.st_gid);
    snprintf(h->size, 12, "%011lld",s.st_size);
    // Typeflag
    //if

    return h;
}

void dumpHeader(struct tar_header *header, char *fileName){
    FILE *file;
    file = fopen(fileName, "a");
    fwrite(header->name, 1, sizeof(header->name), file);
    fwrite(header->mode, 1, sizeof(header->mode), file);
    fwrite(header->uid, 1, sizeof(header->uid), file);
    fwrite(header->gid, 1, sizeof(header->gid), file);
    fwrite(header->size, 1, sizeof(header->size), file);
    fwrite(header->mtime, 1, sizeof(header->mtime), file);
    fwrite(header->chksum, 1, sizeof(header->chksum), file);
    fwrite(&header->typeflag, 1, sizeof(header->typeflag), file);
    fwrite(header->linkname, 1, sizeof(header->linkname), file);
    fwrite(header->magic, 1, sizeof(header->magic), file);
    fwrite(header->version, 1, sizeof(header->version), file);
    fwrite(header->uname, 1, sizeof(header->uname), file);
    fwrite(header->gname, 1, sizeof(header->gname), file);
    fwrite(header->devmajor, 1, sizeof(header->devmajor), file);
    fwrite(header->devminor, 1, sizeof(header->devminor), file);
    fwrite(header->prefix, 1, sizeof(header->prefix), file);
    fwrite(header->pad, 1, sizeof(header->pad), file);
    fclose(file);
    return;
}

void blankHeader(struct tar_header *h){
    memset(h->name, 0, sizeof(h->name));
    memset(h->mode, 0, sizeof(h->name));
    memset(h->uid, 0, sizeof(h->name));
    memset(h->gid, 0, sizeof(h->name));
    memset(h->size, 0, sizeof(h->name));
    memset(h->mtime, 0, sizeof(h->name));
    memset(h->chksum, 0, sizeof(h->name));
    memset(&h->typeflag, 0, sizeof(h->name));
    memset(h->linkname, 0, sizeof(h->name));
    memset(h->magic, 0, sizeof(h->name));
    memset(h->version, 0, sizeof(h->name));
    memset(h->uname, 0, sizeof(h->name));
    memset(h->gname, 0, sizeof(h->name));
    memset(h->devmajor, 0, sizeof(h->name));
    memset(h->devminor, 0, sizeof(h->name));
    memset(h->prefix, 0, sizeof(h->name));
    memset(h->pad, 0, sizeof(h->name));
    return;
}
