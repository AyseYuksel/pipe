#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(){
//firs pipe(fd1) using for the sending string from 
//parent process to child process
	
//second pipe(fd2) using for the child process will concatenate strings and  
//send it to main(parent) process


int fd1[2]; // fd1[0]=> read, fd1[1]=>write
int fd2[2]; //fd2[0]=>read,fd1[1]=>write

char fixed_str[]="@hotmail.com";
char input_str[100];

pid_t p;

if(pipe(fd1)==-1){
fprintf(stderr,"pipe failed");
return 1;
}
if(pipe(fd2)==-1){
fprintf(stderr,"pipe failed");
return 1;
}

scanf("%s",input_str);//scanning string using for concatenated with fixed_str

p=fork();
if(p<0){
fprintf(stderr,"fork failed");
return 1;
}else if(p>0){ //parent process

char concat_str[100]; //array to write the concatenated string

close(fd1[0]);//closing the reading part of the pipe

write(fd1[1],input_str,strlen(input_str)+1);
//input_str variable write to fd1[1] pipe 
//write() parameters => pipe's write part,string,string length
close(fd1[1]); //closing the writing part of the pipe

//parent process wait for the child process to finish its job.
wait(NULL);


//the writing part of the pipe is closed.Because the data from the child process//will be read
close(fd2[1]);
	
//after reading from fd2[0] , the read string is written to the screen and
//second pipe's reading part is closing
read(fd2[0],concat_str,100);
printf("Concatenated string %s \n",concat_str);
close(fd2[0]);
}else{ //child process

//the pipe that parent process writes and send will be read 
//the writing part of the fd1 is closed first.

close(fd1[1]);

//reading through pipe with fd1[0].data is kept in concat_str variable.
read(fd1[0],concat_str,100);

//getting size of scannig string (k variable) after than
//string concatenation with for loop starting from variable k.
int k=strlen(concat_str);
for(int i=0;i<strlen(fixed_str);i++){
concat_str[k++]=fixed_str[i];
}

concat_str[k]='\0'; 

//the concatenated string is written to the fd2[1] pipe by the child process 
//and sent to the parent process.then the pipe is closed.

close(fd1[0]);
close(fd2[0]);

write(fd2[1],concat_str,strlen(concat_str)+1);
close(fd2[1]);
exit(0);
}

//input => ayseyuksel
//output => ayseyuksel@hotmail.com

}
