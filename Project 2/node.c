
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define size_buff 1474560
extern char buffer1 [size_buff];
int file = 0;
extern char str[100];

/// this function travrse the directory and list all the directories
void ls(){
    // buffer to store the path read from the root directory
    char buffer_path [100][100];
    // buffer to store the index of the directories which will be used eventually for sorting
    char buffer_index [100];


    // General counters
    int i = 0;
    int ii = 0;
    int count1 = 0;

    // size of root directory in chunks of 16 bit data
    int size_root = (32-19+1)*16;
    // root directory consisting of 32 byte data
    char root_dir [size_root][32];
    int root_1 = 0;
    int root_2 = 0;
    int start_RootDir = 19*512;
    int root_counter = 0;
    for (root_1 = 0; root_1<size_root; root_1++){
        for (root_2 = 0; root_2<32; root_2++){
            root_dir[root_1][root_2] = buffer1[start_RootDir + root_counter];
            root_counter++;
        }
    }

    char buff_temp [100];
    int buff_count = 0;
    char temp;
    printf("\n");

    /// BUBBLE SORT
    int max_buff_size = 0;
    int buff_row = 0;
    for (i = 0; i<size_root; i++){
        if ((root_dir[i][0]&0xFF) == 0x00 || (root_dir[i][0]&0xFF) == 0xE5 || (root_dir[i][11]&0xFF)==0x0F ){
            // unused or erased --> don't do anything
        } else {
            for (ii = 0; ii<8; ii++){
                temp = root_dir[i][ii];
                if (temp != 0x20){
                    buff_temp[buff_count] = temp;
                    buff_count++;
                }
            }
            if (root_dir[i][8] != 0x20){
                buff_temp[buff_count] = '.';
                buff_count++;
                while (ii<11){
                    temp = root_dir[i][ii];
                    buff_temp[buff_count] = temp;
                    buff_count++;
                    ii++;
                }
            }
            if ((root_dir[i][11]&0xFF)==0x10 ){
                while(buff_count < 25){
                    buff_temp[buff_count] = 0x20;
                    buff_count++;
                }
                buff_temp[buff_count] = '<';
                buff_count++;
                buff_temp[buff_count] = 'D';
                buff_count++;
                buff_temp[buff_count] = 'I';
                buff_count++;
                buff_temp[buff_count] = 'R';
                buff_count++;
                buff_temp[buff_count] = '>';
                buff_count++;
            }
        }
        buff_temp[buff_count] = NULL;
        buff_count = 0;
        if ((root_dir[i][0]&0xFF) == 0x00 || (root_dir[i][0]&0xFF) == 0xE5 || (root_dir[i][11]&0xFF)==0x0F ){
                // do nothing
        } else {
            count1 = 0;
            while (buff_temp[count1] != NULL){
                buffer_path[buff_row][count1] = buff_temp[count1];
                count1++;
            }
            buffer_path[buff_row][count1] = buff_temp[count1];
            buffer_index[buff_row] = buff_row;

            buff_row++;
        }
    }
    max_buff_size = buff_row;

    char temp_buff1 [100];
    char temp_buff2 [100];
    i = 0;
    ii = 0;
    count1 = 0;
    int comp_res = 0;
    char temp_swap = 0;
    for (i=0; i<max_buff_size-1; i++){
        for (ii=0; ii<max_buff_size-1; ii++){
            count1 = 0;
            for (count1 = 0; count1<100; count1++){
                temp_buff1[count1] = buffer_path[ii][count1];
                temp_buff2[count1] = buffer_path[ii+1][count1];
            }
            comp_res = strncmp(temp_buff1, temp_buff2, 100);

            if (comp_res>0){
                count1 = 0;
                for (count1 = 0; count1<100; count1++){
                    temp_swap = buffer_path[ii][count1];
                    buffer_path[ii][count1] = buffer_path[ii+1][count1];
                    buffer_path[ii+1][count1] =  temp_swap;
                }
                temp_swap = buffer_index[ii];
                buffer_index[ii] = buffer_index[ii+1];
                buffer_index[ii+1] = temp_swap;
            }
        }
    }

    i = 0;
    for (i=0; i<max_buff_size; i++ ){
        count1 = 0;

        while (buffer_path[i][count1] != NULL){
            printf("%c", buffer_path[i][count1]);
            count1++;
        }
        printf("\n");
    }
}

void ls_l(){


    printf("    *****************************\n");
    printf("    ** FILE ATTRIBUTE NOTATION **\n");
    printf("    **                         **\n");
    printf("    ** R ------ READ ONLY FILE **\n");
    printf("    ** S ------ SYSTEM FILE    **\n");
    printf("    ** H ------ HIDDEN FILE    **\n");
    printf("    ** A ------ ARCHIVE FILE   **\n");
    printf("    *****************************\n");

    // generating Root Directory
    char buffer_path [100][100];
    char buffer_index [100];

    int i = 0;
    int ii = 0;
    int count1 = 0;

    int size_root = (32-19+1)*16;
    char root_dir [size_root][32];
    int root_1 = 0;
    int root_2 = 0;
    int start_RootDir = 19*512;
    int root_counter = 0;
    for (root_1 = 0; root_1<size_root; root_1++){
        for (root_2 = 0; root_2<32; root_2++){
            root_dir[root_1][root_2] = buffer1[start_RootDir + root_counter];
            root_counter++;
        }
    }

    char buff_temp [100];
    int buff_count = 0;
    char temp;
    printf("\n");

    int max_buff_size = 0;
    int buff_row = 0;

    for (i = 0; i<size_root; i++){
        if ((root_dir[i][0]&0xFF) == 0x00 || (root_dir[i][0]&0xFF) == 0xE5 || (root_dir[i][11]&0xFF)==0x0F ){
            // unused or erased --> don't do anything
        } else {
            for (ii = 0; ii<8; ii++){
                temp = root_dir[i][ii];
                if (temp != 0x20){
                    buff_temp[buff_count] = temp;
                    buff_count++;
                }
            }
            if (root_dir[i][8] != 0x20){
                buff_temp[buff_count] = '.';
                buff_count++;
                while (ii<11){
                    temp = root_dir[i][ii];
                    buff_temp[buff_count] = temp;
                    buff_count++;
                    ii++;
                }
            }
            if (1){
                while(buff_count < 25){
                    buff_temp[buff_count] = 0x20;
                    buff_count++;
                }
            }
        }
        buff_temp[buff_count] = NULL;
        buff_count = 0;
        if ((root_dir[i][0]&0xFF) == 0x00 || (root_dir[i][0]&0xFF) == 0xE5 || (root_dir[i][11]&0xFF)==0x0F ){
            // do nothing
        } else {
            count1 = 0;
            while (buff_temp[count1] != NULL){
                buffer_path[buff_row][count1] = buff_temp[count1];
                count1++;
            }
            buffer_path[buff_row][count1] = buff_temp[count1];
            buffer_index[buff_row] = buff_row;

            buff_row++;
        }
    }
    max_buff_size = buff_row;

    /// BUBBLE SORT
    char temp_buff1 [100];
    char temp_buff2 [100];
    i = 0;
    ii = 0;
    count1 = 0;
    int comp_res = 0;
    char temp_swap = 0;
    for (i=0; i<max_buff_size-1; i++){
        for (ii=0; ii<max_buff_size-1; ii++){
            count1 = 0;
            for (count1 = 0; count1<100; count1++){
                temp_buff1[count1] = buffer_path[ii][count1];
                temp_buff2[count1] = buffer_path[ii+1][count1];
            }
            comp_res = strncmp(temp_buff1, temp_buff2, 100);
            if (comp_res>0){
                count1 = 0;
                for (count1 = 0; count1<100; count1++){
                    temp_swap = buffer_path[ii][count1];
                    buffer_path[ii][count1] = buffer_path[ii+1][count1];
                    buffer_path[ii+1][count1] =  temp_swap;
                }
                temp_swap = buffer_index[ii];
                buffer_index[ii] = buffer_index[ii+1];
                buffer_index[ii+1] = temp_swap;
            }
        }
    }

    char buffer_date [100][100];
    unsigned int  buffer_time [100][6];
    char DIR_dir [100][5];
    unsigned long buffer_size [100];
    unsigned int  first_sector [100];
    int buffer_date_count = 0;
    int buffer_date_row = 0;

    for (i = 0; i<size_root; i++){
        buffer_date_count = 0;
        if ((root_dir[i][0]&0xFF) == 0x00 || (root_dir[i][0]&0xFF) == 0xE5 || (root_dir[i][11]&0xFF)==0x0F ){
            // unused or erased --> don't do anything
        } else {
            buffer_date[buffer_date_row][buffer_date_count] = '-';
            buffer_date_count++;
            if (root_dir[i][11]&0x20){ // Archive
                buffer_date[buffer_date_row][buffer_date_count] = 'A';
                buffer_date_count++;
            } else {
                buffer_date[buffer_date_row][buffer_date_count] = '-';
                buffer_date_count++;
            }
            if (root_dir[i][11]&0x01){ // read only
                buffer_date[buffer_date_row][buffer_date_count] = 'R';
                buffer_date_count++;
            } else {
                buffer_date[buffer_date_row][buffer_date_count] = '-';
                buffer_date_count++;
            }
            if (root_dir[i][11]&0x04){ // System
                buffer_date[buffer_date_row][buffer_date_count] = 'S';
                buffer_date_count++;
            } else {
                buffer_date[buffer_date_row][buffer_date_count] = '-';
                buffer_date_count++;
            }
            if (root_dir[i][11]&0x02){ // Hidden
                buffer_date[buffer_date_row][buffer_date_count] = 'H';
                buffer_date_count++;
            } else {
                buffer_date[buffer_date_row][buffer_date_count] = '-';
                buffer_date_count++;
            }

            buffer_date[buffer_date_row][buffer_date_count] = 0x20;
            buffer_date_count++;
            buffer_date[buffer_date_row][buffer_date_count] = 0x20;
            buffer_date_count++;
            buffer_date[buffer_date_row][buffer_date_count] = 0x20;
            buffer_date_count++;
            buffer_date[buffer_date_row][buffer_date_count] = 0x20;
            buffer_date_count++;
            buffer_date[buffer_date_row][buffer_date_count] = 0x20;
            buffer_date_count++;

            /// DATE NOW

            unsigned int  day = 0;
            unsigned int  month = 0;
            unsigned int  year = 0;

            unsigned int Input_date = 0;
            Input_date = (unsigned int) (root_dir[i][17] << 8 );
            Input_date = (unsigned int) (root_dir[i][16] | Input_date);

            day = (Input_date&0b00011111);
            month = ((Input_date>>5)&(0b00001111));
            year = ((Input_date>>9)&(0b01111111))+1980;

            buffer_time[buffer_date_row][0] = month;
            buffer_time[buffer_date_row][1] = day;
            buffer_time[buffer_date_row][2] = year;

            /// FILE SIZE

            unsigned long Input_size = 0;
            Input_size = (unsigned long ) (root_dir[i][31] << 24 );
            Input_size = (unsigned long ) ((root_dir[i][30] << 16 )| Input_size);
            Input_size = (unsigned long ) ((root_dir[i][29] << 8 )| Input_size);
            Input_size = (unsigned long ) ((root_dir[i][28]&0xFF) | Input_size);
            buffer_size[buffer_date_row] = Input_size;


            /// FIRT SECTOR
            unsigned int first_sector_temp = 0;
            first_sector_temp = (unsigned int) (root_dir[i][27] << 8 );
            first_sector_temp = (unsigned int) (root_dir[i][26] | first_sector_temp);
            first_sector[buffer_date_row] = first_sector_temp;

            /// TIME
            // need work to do


            /// DIR directory

            if ((root_dir[i][11]&0xFF)==0x10 ){
                DIR_dir[buffer_date_row][0] = '<';
                DIR_dir[buffer_date_row][1] = 'D';
                DIR_dir[buffer_date_row][2] = 'I';
                DIR_dir[buffer_date_row][3] = 'R';
                DIR_dir[buffer_date_row][4] = '>';
            } else {
                DIR_dir[buffer_date_row][0] = 0x20;
                DIR_dir[buffer_date_row][1] = 0x20;
                DIR_dir[buffer_date_row][2] = 0x20;
                DIR_dir[buffer_date_row][3] = 0x20;
                DIR_dir[buffer_date_row][4] = 0x20;

            }
            buffer_date_row++;
        }
    }

    /// PRINTING DATA
    i = 0;
    for (i=0; i<max_buff_size;i++){

        printf("%c",buffer_date[buffer_index[i]][0]);
        printf("%c",buffer_date[buffer_index[i]][1]);
        printf("%c",buffer_date[buffer_index[i]][2]);
        printf("%c",buffer_date[buffer_index[i]][3]);
        printf("%c",buffer_date[buffer_index[i]][4]);
        printf("%c",buffer_date[buffer_index[i]][5]);
        printf("%c",buffer_date[buffer_index[i]][6]);
        printf("%c",buffer_date[buffer_index[i]][7]);
        printf("%c",buffer_date[buffer_index[i]][8]);
        printf("%c",buffer_date[buffer_index[i]][9]);

        printf("   %d/%d/%d       ",buffer_time[buffer_index[i]][0], buffer_time[buffer_index[i]][1], buffer_time[buffer_index[i]][2]);

        printf("%c",DIR_dir[buffer_index[i]][0]);
        printf("%c",DIR_dir[buffer_index[i]][1]);
        printf("%c",DIR_dir[buffer_index[i]][2]);
        printf("%c",DIR_dir[buffer_index[i]][3]);
        printf("%c",DIR_dir[buffer_index[i]][4]);

        if ( buffer_size[buffer_index[i]] == 0 ){
            printf("        ");
        } else {
            printf("%8lu", (unsigned long )buffer_size[buffer_index[i]]);
        }

        count1 = 0;
        printf("     ");
        while (buffer_path[buffer_index[i]][count1] != NULL){
            printf("%c", buffer_path[buffer_index[i]][count1]);
            count1++;
            }

        printf("%3d", first_sector[buffer_index[i]]);

        printf("\n");
    }
}

void help_cons(){
    printf("Here is a list of available functions:");
    printf("\n    fmount <file name>:  mounts the image file into the program and stores data.");
    printf("\n    fumount:             unmounts the file.");
    printf("\n    structure:           unmounts the file.");
    printf("\n    quit:                quits the program.");
    printf("\n    ls:                  lists the content in the root directory");
    printf("\n    ls -l:               lists the content in the root directory in details");
    printf("\n    showsector sector#:  shows the contents (in hex dump) of the specified sector number.");
    printf("\n    showfat:             shows the content of FAT table (display sector 01 only with hex values).");
    printf("\n");
}

void mount_floppy(char str[], int size){

    int i = size;
    char file_name[100];
    for (i = 0; i<100; i++){
        if (str[i] == '<'){
            i++;
            break;
        }
    }
    int ii = 0;
    for (ii = 0; ii<size;ii++){
        if (str[i+ii] == '>'){
            break;
            }
        file_name [ii] = str[i+ii];
    }

    file_name [ii] = '.';
    file_name [ii+1] = 'i';
    file_name [ii+2] = 'm';
    file_name [ii+3] = 'g';
    file_name [ii+4] = NULL;

    file = 0;
    if((file = open(file_name,O_RDONLY)) < -1){
        printf("Error");
    }

    int size_i = 10;
    char temp_buff[size_i];
    lseek(file,0,SEEK_SET);
    int result = 0;
    int count = 0;
    while (count<size_buff){
        result = read(file,temp_buff,size_i);

        if (result < 0){
            printf("Error reading");
            break;
        }
        if (result == 0){
            break;
        }
        int inter_loop = 0;
        for (inter_loop = 0; inter_loop<result; inter_loop++ ){
            buffer1[count + inter_loop] = temp_buff[inter_loop];
        }
        lseek(file,(int)result+(int)count,SEEK_SET);
        count = count + result;
    }
    if (result<0){
        // there is an error which has been cited above

    } else {
        printf("File mounted successfully\n");
    }

}

void unmount_floppy(){
    close(file);
    printf("File unmounted successfully\n");

}

void structure(){
    unsigned int value_1 = (unsigned int)(buffer1[16]);
    printf("number of FAT:                      ");
    printf("%d \n", value_1);

    unsigned int value_2 = 0;
    value_2 = (unsigned int) (buffer1[23] << 8 );
    value_2 = (unsigned int) (buffer1[22] | value_2);
    printf("number of sectors used by FAT:      ");
    printf("%d \n", value_2);

    unsigned int value_3 = (unsigned int)(buffer1[13]);
    printf("number of sectors per cluster:      ");
    printf("%d \n", value_3);

    unsigned int value_4 = 0;
    value_4 = (unsigned int) (buffer1[18] << 8 );
    value_4 = (unsigned int) (buffer1[17] | value_4);
    printf("number of ROOT Entries:           ");
    printf("%d \n", (unsigned char)value_4);

    unsigned int value_5 = 0;
    value_5 = (unsigned int) (buffer1[12] << 8 );
    value_5 = (unsigned int) (buffer1[11] | value_5);
    printf("number of bytes per sector:       ");
    printf("%d \n", value_5);

    unsigned int value_10 = (unsigned char)(value_4);

    if (buffer1[16] == 2){
        printf("---Sector #---     ---Sector Types---\n");
        printf("      0                  BOOT\n");
        printf("   01 -- 0%d              FAT1\n", value_2);
        printf("   %d -- %d              FAT2\n", value_2+1, 2*value_2);
        printf("   %d -- %d              ROOT DIRECTORY\n", 2*value_2+1, 2*value_2+ (unsigned int)(value_10/14) - 2 );
    } else if (buffer1[16] == 1){
        printf("---Sector #---     ---Sector Types---\n");
        printf("      0                  BOOT\n");
        printf("   01 -- 0%d              FAT1\n", value_2);
        printf("   %d -- %d              ROOT DIRECTORY\n", value_2+1, value_2 + (unsigned int)(value_10/14) - 2);
    }
}

void showfat(){

    int i = 0;
    int ii = 0;
    int column_count = 0;

    int size_FAT_3CHAR = 9*512/3*2;
    int size_FAT_1BYTE = 9*512;
    int FAT12 [size_FAT_3CHAR];
    int start_FAT = 512;
    int FAT_cell_counter = 0;

    for (i=start_FAT; i<size_FAT_1BYTE+start_FAT; i=i+3){
        int buf1 = buffer1[i] & 0xFF;
        int buf2 = buffer1[i+1] & 0xFF;
        int buf3 = buffer1[i+2] & 0xFF;

        int res1 = ((buf2 << 8) & 0xF00) | buf1;
        int res2 = (buf3 << 4) | (buf2>>4);

        FAT12[FAT_cell_counter] = res1;
        FAT_cell_counter++;
        FAT12[FAT_cell_counter] = res2;
        FAT_cell_counter++;
    }

    printf("      ");
    for (column_count = 0; column_count<16; column_count++){
        printf("%4x ", column_count);
    }
    printf("\n\n");

    i = 0;
    ii = 0;
    while (i<352){
        printf("%5x ", i);
        for (ii=0; ii<16; ii++){
            if (FAT12[i] == 0x000){
                printf("FREE ");
                i++;
            } else {
                printf("%4x ", FAT12[i]);
                i++;
            }
        }
        printf("%\n");
    }
}



void showsector(char str[], int size){

    int i = 0;
    int ii = 0;
    int column_count = 0;
    int iii = 0;


    char sectorNum_buff[100];
    for (i = 0; i<100; i++){
        if (str[i] == ' '){
            i++;
            break;
        }
    }

    for (ii = 0; ii<size;ii++){
        if (str[i+ii] == NULL){
            sectorNum_buff [ii] = str[i+ii];
            break;
            }
        sectorNum_buff [ii] = str[i+ii];
    }

    int sector_num = atoi(sectorNum_buff);
    //printf("Data available at sector #%d :\n", sector_num);

    printf("    ");
    for (column_count = 0; column_count<16; column_count++){
        printf("%3x ", column_count);
    }
    printf("\n\n");

    i = sector_num*512;
    ii = 0;
    while (i<(sector_num+1)*512){
        printf("%3x ", iii);
        for (ii=0; ii<16; ii++){
            printf("%3x ", buffer1[i]&0xFF);
            i++;
            iii++;
        }
        printf("%\n");
    }

}


