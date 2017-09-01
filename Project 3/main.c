#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char cd_buff[100];
char input_buff[100];
char PATH[100][100];

int execute();
int add_path();
int remove_path();
void print_PATH();
int exec_cd();
void print_cd();
int exec_fork();
int exec_pipe1();
int exec_pipe2();
int in_redir();
int out_redir();

int main( void )
{
	while (1){
		printf("\n$ ");
		fgets(input_buff,100,stdin);
		input_buff[strcspn(input_buff,"\n")] = '\0';
		
		if (strcmp(input_buff,"quit")==0){
			printf("Exiting\n");
            kill(0,SIGTERM);
			break;
		}
		if (execute() == 1){
			//break;
		}	
	}
	return 0;
}
//--------------------------------------------------------------execute()
int execute(){
	char cd_comp[10];
	strcpy(cd_comp,input_buff);
	cd_comp[2] = '\0';
	
	char path_comp[10];
	strcpy(path_comp,input_buff);
	path_comp[6] = '\0';
		
	char pathp_comp[10];
	strcpy(pathp_comp,input_buff);
	pathp_comp[6] = '\0';
	
	char pathm_comp[10];
	strcpy(pathm_comp,input_buff);
	pathm_comp[6] = '\0';
	
	int count = 0;
	int pipe_count = 0;
	int in_out_redir = 0;
	while (input_buff[count]!='\0'){
		if (input_buff[count]=='|'){
			pipe_count++;
		}
		if (input_buff[count]=='<'){
			in_out_redir = 1;
		} else if (input_buff[count]=='>'){
			in_out_redir = 2;
		} else {}
		count++;
	}
	//printf ("in_out_redir %d \n", in_out_redir);
	if (strcmp(input_buff,"quit")==0){
		return 1;
		
	} else if (strcmp(pathp_comp,"path +")==0){
		add_path();
		
	} else if (strcmp(pathm_comp,"path -")==0){
		remove_path();
		
	} else if (strcmp(path_comp,"path")==0){
		print_PATH();
		
	} else if (strcmp(cd_comp,"cd")==0){
		exec_cd();

	} else if (pipe_count==0 & in_out_redir==0) {
		exec_fork();
	
	} else if (pipe_count==1) {
		exec_pipe1();
		
	} else if (pipe_count==2) {
		exec_pipe2();
		
	} else if (in_out_redir==1) {
		in_redir();
		
	} else if (in_out_redir==2) {
		//printf("exec print 2\n");
		out_redir();
		
	}
	return 0;
}
//--------------------------------------------------------------add_path()
int add_path(){

	char path_buff[100];
	path_buff[0] = '\0';
	int i = 0;
	int count = 0;
	int start_flag = 0;
	while (input_buff[i]!='\0'){

		if (input_buff[i]=='/'){
			start_flag = 1;
		}
		if (start_flag == 1){
			path_buff[count] = input_buff[i];
			count++;
		}
		i++;
	}
	path_buff[count] = '\0';
	if (path_buff[0] == '\0'){
		printf("Error. No path was entered\n");
		return 0;
	}
	
	int path_row = 0;
	while (path_row<100){
		if (PATH[path_row][0] == '\0'){
			break;
		}
		path_row++;
	}
	
	i = 0;
	while (path_buff[i] != '\0'){
		PATH[path_row][i] = path_buff[i];
		PATH[path_row][i+1] = '\0';
		i++;
	}

	print_PATH();
	
	return 0;
}
//--------------------------------------------------------------remove_path()
int remove_path(){

	char path_buff[100];
	path_buff[0] = '\0';
	int i = 0;
	int ii = 0;
	int count = 0;
	int start_flag = 0;
	while (input_buff[i]!='\0'){

		if (input_buff[i]=='/'){
			start_flag = 1;
		}
		if (start_flag == 1){
			path_buff[count] = input_buff[i];
			count++;
		}
		i++;
	}
	path_buff[count] = '\0';
	if (path_buff[0] == '\0'){
		printf("Error. No path was entered\n");
		return 0;
	} 
	
	i = 0;
	ii = 0;
	char remove_buff_comp [100];
	for (i=0; i<100; i++){
		for (ii=0; ii<100;ii++){
			remove_buff_comp[ii] = PATH[i][ii];
		}
		if (strcmp(remove_buff_comp,path_buff)==0){
			PATH[i][0] = '\0';
			break;		
		}
	}
	
	print_PATH();
	
	return 0;
}
//--------------------------------------------------------------------print_PATH()
void print_PATH(){
	int i = 0;
	int ii = 0;
	for (i=0; i<100; i++){
		ii = 0;
		if (PATH[i][0] == '\0'){
			//break; // do nothing
		} else {
			while (PATH[i][ii] != '\0'){
				printf("%c", PATH[i][ii]);
				ii++;
			}
			printf(":");
		}
	}
	printf("\n");
}
//--------------------------------------------------------------------exec_cd()
int exec_cd(){
	char cd_temp[100];
	cd_temp[0] = '\0';
	int i = 0;
	int ii = 0;
	int count = 0;
	int start_flag = 0;
	while (input_buff[i]!='\0'){

		if (input_buff[i]=='/'){
			start_flag = 1;
		}
		if (start_flag == 1){
			cd_temp[count] = input_buff[i];
			count++;
		}
		i++;
	}
	cd_temp[count] = '\0';
	if (cd_temp[0] == '\0'){
		printf("Error. No cd was entered\n");
		return 0;
	} 
	
	i = 0;
	while (cd_temp[i] != '\0'){
		cd_buff[i] = cd_temp[i];
		i++;
	}
	print_cd();
	return 0;
}
//--------------------------------------------------------------------print_cd()
void print_cd(){
	int i = 0;
	while (cd_buff[i] != '\0'){
		printf("%c", cd_buff[i]);
		i++;
	}
	printf("\n");
}
//--------------------------------------------------------------------exec_fork()
int exec_fork(){ //IMPLEMENTING ONLY ONE COMMAND WITHOUT ANY PIPE
	//printf ("Entered single command\n");
	int count = 0;
	int i = 0;
	int ii = 0;
	int PATH_num = 0;
	for (count=0; count<100; count++){
		if (PATH[count][0]!='\0'){
			PATH_num++;
		} 
	}
	
	//printf("%d\n",PATH_num);
	if (PATH_num == 0){
		printf("Error. Path Empty");
		return -1;
	}
	
	////---dividing the command nto two parts 
	int sec_arg_flg = 0;
	char command_seg [2][20] = {0};
	count = 0;
	i = 0;
	ii = 0;
	while (input_buff[count]!='\0'){
		if (input_buff[count]==' '){
			if (sec_arg_flg==0){
				sec_arg_flg = 1;
				command_seg[0][ii] = '\0';	
			}
			count++;
			ii=0;
				
		} else if (sec_arg_flg == 0){
			command_seg[0][ii] = input_buff[count];
			ii++;
			count++;
		} else if (sec_arg_flg == 1){
			command_seg[1][ii] = input_buff[count];
			ii++;
			count++;
		}
	}
	if (sec_arg_flg == 1){
		command_seg[1][ii] = '\0';
	}
	///---END OF DIVIDE
	//printf("command_seg[0]: %s\n", command_seg[0]);
	//printf("command_seg[1]: %s\n", command_seg[1]);
	
		int status;
	if ( fork() == 0 ){
		i = 0;
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[0]);
			int status;
			
			char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			
			if (sec_arg_flg==0){
				char *args[2];
				args[0] = array;				
				args[1] = '\0';
				execv( args[0], args ); // child: call execv with the path and the args
			
			} else if (sec_arg_flg==1){
				char *args[3];
				args[0] = array;				
				args[1] = command_seg[1];//"-l";				
				args[2] = '\0';
				//printf ("Here is string %s\n", command_seg[1]);
				execv( args[0], args ); // child: call execv with the path and the args
			
			} else {
				printf("Error parsing command. Too many argument");
			}
		}
	}
	else {
		wait( &status );  // parent: wait for the child (not really necessary)
		//printf("Parent!\n");	
	}	
	return 0;
}
//--------------------------------------------------------------------IMPLEMENTING PIPE 2
int exec_pipe2(){
	//printf("entered fork\n");
	int i = 0;
	int ii = 0;
	int count =0;
	int pipe_num = 1;
	while (input_buff[count]!= '\0'){
		if (input_buff[count]=='|'){
			pipe_num++;

		}
		count++;
	}
	//printf("1st %s\n", pipe_com[0]);
	//printf("2nd %s\n", pipe_com[1]);
	//printf("3rd %s\n", pipe_com[2]);
	//printf("pipe_num %d \n", pipe_num);

	//printf("PIPE\n");
	
	count = 0;
	i = 0;
	ii = 0;
	int PATH_num = 0;
	for (count=0; count<100; count++){
		if (PATH[count][0]!='\0'){
			PATH_num++;
		} 
	}

	//printf("%d\n",PATH_num);
	if (PATH_num == 0){
		printf("Error. Path Empty");
		return -1;
	}
	
	////---dividing the command nto two parts 
	char command_seg [10][20] = {0};
	count = 0;
	i = 0;
	ii = 0;
	int flag = 0;
	int sec_arg_flg_pipe[10] = {0};
	
	while(flag ==0){
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='|' & input_buff[count]!='\0'){
			command_seg[i][ii] = input_buff[count];
			command_seg[i][ii+1] = '\0';
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
			while (input_buff[count]!=' ' & input_buff[count]!='|' & input_buff[count]!='\0'){
			command_seg[i+1][ii] = input_buff[count];
			command_seg[i+1][ii+1] = '\0';
			sec_arg_flg_pipe[i] = 1;
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]=='|' ){
			count++;
			ii = 0;
			i = i+2;
		}
		if (input_buff[count]=='\0'){
			flag = 1;
		}
		
	}
	//printf("debug");
	
	//printf("1st %s\n", command_seg[0]);
	//printf("1sd %s\n", command_seg[1]);
	//printf("2nd %s\n", command_seg[2]);
	//printf("2nt %s\n", command_seg[3]);
	//printf("3rd %s\n", command_seg[4]);
	//printf("3rd %s\n", command_seg[5]);		
	
	
	//printf("sec_arg_flg_pipe: %d\n", sec_arg_flg_pipe[0]);
	int pipefd[2];
    if(pipe(pipefd) == -1) {
    perror("Pipe failed");
     //exit(1);
    }
    int pipefd1[2];
    if(pipe(pipefd1) == -1) {
    perror("Pipe failed");
     //exit(1);
    }
    
    if ( fork() == 0 ){
		
   		close(STDOUT_FILENO);  //closing stdout
        dup(pipefd[1]);         //replacing stdout with pipe write 
        close(pipefd[0]);       //closing pipe read
        close(pipefd[1]);
        close(pipefd1[0]);       //closing pipe read
        close(pipefd1[1]);
        
   		
		i = 0;
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[0]);
				char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe[0]==0){
				execl(array, command_seg[0], NULL );
			
			} else if (sec_arg_flg_pipe[0]==1){
				execl(array, command_seg[0], command_seg[1], NULL );
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
			
	} 
		
	if ( fork() == 0 ){

      	close(STDIN_FILENO);  
        dup(pipefd[0]); 
        close(STDOUT_FILENO); 
        dup(pipefd1[1]);     
        close(pipefd[1]);       
        close(pipefd[0]);
        close(pipefd1[0]);       //closing pipe read
        close(pipefd1[1]); 
        
		i = 0;
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[2]);
				char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe[2]==0){
				execl(array, command_seg[2], NULL );
			
			} else if (sec_arg_flg_pipe[2]==1){
				execl(array, command_seg[2], command_seg[3], NULL );
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
	} 
	
	if ( fork() == 0 ){
		
      	close(STDIN_FILENO);   //closing stdin
        dup(pipefd1[0]);         //replacing stdin with pipe read
        close(pipefd[1]);       //closing pipe write
        close(pipefd[0]);
        close(pipefd1[0]);       //closing pipe read
        close(pipefd1[1]); 
        
		i = 0;
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[4]);
				char *args1[4];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe[4]==0){
				execl(array, command_seg[4], NULL );
			
			} else if (sec_arg_flg_pipe[4]==1){
				execl(array, command_seg[4], command_seg[5], NULL );
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
	} 
	
	close(pipefd[0]);
    close(pipefd[1]);
    close(pipefd1[0]);
    close(pipefd1[1]);
    
    return 0;
}
//--------------------------------------------------------------------IMPLEMENTING PIPE 1
int exec_pipe1(){
	

	int i = 0;
	int ii = 0;
	int count =0;
	int pipe_num = 1;
	while (input_buff[count]!= '\0'){
		if (input_buff[count]=='|'){
			pipe_num++;

		}
		count++;
	}
	
	count = 0;
	i = 0;
	ii = 0;
	int PATH_num = 0;
	for (count=0; count<100; count++){
		if (PATH[count][0]!='\0'){
			PATH_num++;
		} 
	}

	if (PATH_num == 0){
		printf("Error. Path Empty");
		return -1;
	}
	
	////---dividing the command nto two parts 
	char command_seg [10][20] = {0};
	count = 0;
	i = 0;
	ii = 0;
	int flag = 0;
	int sec_arg_flg_pipe[10] = {0};
	
	while(flag ==0){
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='|' & input_buff[count]!='\0'){
			command_seg[i][ii] = input_buff[count];
			command_seg[i][ii+1] = '\0';
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
			while (input_buff[count]!=' ' & input_buff[count]!='|' & input_buff[count]!='\0'){
			command_seg[i+1][ii] = input_buff[count];
			command_seg[i+1][ii+1] = '\0';
			sec_arg_flg_pipe[i] = 1;
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]=='|' ){
			count++;
			ii = 0;
			i = i+2;
		}
		if (input_buff[count]=='\0'){
			flag = 1;
		}
		
	}
	
	int pipefd[2];
    if(pipe(pipefd) == -1) {
    perror("Pipe failed");
     //exit(1);
    }
    if ( fork() == 0 ){
		close(STDOUT_FILENO);  //closing stdout
        dup(pipefd[1]);         //replacing stdout with pipe write 
        close(pipefd[0]);       //closing pipe read
        close(pipefd[1]);
   		
		i = 0;
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[0]);
				char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe[0]==0){
				execl(array, command_seg[0], NULL );
			
			} else if (sec_arg_flg_pipe[0]==1){
				execl(array, command_seg[0], command_seg[1], NULL );
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
			
	} 
		

	if ( fork() == 0 ){
		
      	close(STDIN_FILENO);   //closing stdin
        dup(pipefd[0]);         //replacing stdin with pipe read
        close(pipefd[1]);       //closing pipe write
        close(pipefd[0]); 
		i = 0;
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[2]);
				char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe[0]==0){
				execl(array, command_seg[2], NULL );
			
			} else if (sec_arg_flg_pipe[0]==1){
				execl(array, command_seg[2], command_seg[3], NULL );
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
	} 
	close(pipefd[0]);
    close(pipefd[1]);
    
    return 0;
}

//--------------------------------------------------------------------IMPLEMENTING in_redir
int in_redir(){
	printf("Entered in redirect\n");
	
	char command_seg [3][20] = {0};
	int count = 0;
	int i = 0;
	int ii = 0;
	int flag = 0;
	int sec_arg_flg_pipe = 0;
	
	int fd1;
	fd1 = open (strcat(command_seg[2], ".txt"),  O_RDONLY );
	count = 0;
	while(flag ==0){
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='<' & input_buff[count]!='\0'){
			command_seg[0][ii] = input_buff[count];
			command_seg[0][ii+1] = '\0';
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='<' & input_buff[count]!='\0'){
			command_seg[1][ii] = input_buff[count];
			command_seg[1][ii+1] = '\0';
			sec_arg_flg_pipe = 1;
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]=='<' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='<' & input_buff[count]!='\0'){
			command_seg[2][ii] = input_buff[count];
			command_seg[2][ii+1] = '\0';
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		if (input_buff[count]=='\0'){
			flag = 1;
		}
		
	}
	if ( fork() == 0 ){
		close(0);				//  close the read side of the pipe
		dup(fd1);				// replace it with the file descriptor
		close(fd1);				// clsoe fd1
		
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[0]);
			
			char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe==0){
				execl(array, command_seg[0], NULL );
			
			} else if (sec_arg_flg_pipe==1){
				execl(array, command_seg[0], command_seg[1], NULL );
				
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
	}
	
	
	
	
}
//--------------------------------------------------------------------IMPLEMENTING out_redir
int out_redir(){
	printf("Entered out redirect\n");
	
	char command_seg [3][20] = {0};
	int count = 0;
	int i = 0;
	int ii = 0;
	int flag = 0;
	int sec_arg_flg_pipe = 0;
	
	while(flag ==0){
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='>' & input_buff[count]!='\0'){
			command_seg[0][ii] = input_buff[count];
			command_seg[0][ii+1] = '\0';
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='>' & input_buff[count]!='\0'){
			command_seg[1][ii] = input_buff[count];
			command_seg[1][ii+1] = '\0';
			sec_arg_flg_pipe = 1;
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]=='>' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		while (input_buff[count]!=' ' & input_buff[count]!='>' & input_buff[count]!='\0'){
			command_seg[2][ii] = input_buff[count];
			command_seg[2][ii+1] = '\0';
			ii++;
			count++;
		}
		while (input_buff[count]==' ' ){
			count++;
			ii = 0;
		}
		if (input_buff[count]=='\0'){
			flag = 1;
		}
		
	}
	
	//printf("1st : %s\n", command_seg[0]);
	//printf("2nd : %s\n", command_seg[1]);
	//printf("3rd : %s\n", command_seg[2]);
	
	//close(1);  // youwant to see the results on screen which is also saved in file
	int fd1;
	fd1 = open (strcat(command_seg[2], ".txt"),  O_RDWR |O_CREAT | O_TRUNC);
	if ( fork() == 0 ){
		close(1);
		dup(fd1);
		close(fd1);
		
		for (i=0; i<100; i++){
			char array[100] = "\0";
			strcpy(array, PATH[i]);
			strcat(array, "/");
			strcat(array, command_seg[0]);
			
			char *args1[2];
			args1[0] = cd_buff;
			args1[1] = '\0';
			chdir(args1[0]);
			
			if (sec_arg_flg_pipe==0){
				execl(array, command_seg[0], NULL );
			
			} else if (sec_arg_flg_pipe==1){
				execl(array, command_seg[0], command_seg[1], NULL );
				
			} else {
				//printf("Error parsing command. Too many argument");
			}
		}
	}
	
}


		







































