#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "node.h"


#define size_buff 1474560
char buffer1 [size_buff];
char str[100];
int mount_status = 0;


int main(){

    while(1){
        printf("\n$");
        char str[100];
        gets(str);

        char mount_flop[100];
        strcpy(mount_flop, str);
        mount_flop[6] = '\0';

        char sector_num[100];
        strcpy(sector_num, str);
        sector_num[10] = '\0';

        if (mount_status == 1){
            if (strcmp(str, "help") == 0){
                help_cons();
            } else if (strcmp(str, "quit") == 0) {
                break;

            } else if (strcmp(str, "fumount") == 0) {
                unmount_floppy();
                mount_status = 0;

            } else if (strcmp(str, "showfat") == 0) {
                showfat();

            } else if (strcmp(sector_num, "showsector") == 0) {
                showsector(str, sizeof(str));

            } else if (strcmp(str, "ls") == 0) {
                ls();

            } else if (strcmp(str, "ls -l") == 0) {
                ls_l();

            } else if (strcmp(str, "structure") == 0) {
                structure();

            } else {
                printf("invalid input\n");
            }
        } else {
            if (strcmp(str, "help") == 0){
                help_cons();
            } else if (strcmp(str, "quit") == 0) {
                break;

            } else if (strcmp(mount_flop, "fmount") == 0) {
                mount_floppy(str, sizeof(str));
                mount_status = 1;

            } else {
                printf("File has not been mounted or invalid input\n");
            }
        }
    }
    return(0);
}





