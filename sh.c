#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
int cd(char *path){
   return chdir(path);
 }
char **parse(char* input)
{
 int i=0;
   char **command=malloc(8 * sizeof(char *));
  if(command==NULL)
{
     perror("malloc failed");
    exit(1);
 }
  
   char *separator =" ";
  char *parsed;
   int index=0;
   input[strlen(input)-1]='\0';
  parsed =strtok (input, separator);
 
   while(parsed!=NULL) 
 { 
   command[index]=parsed;
   index++;
   parsed=strtok(NULL, separator); 
   
  }
   command[index]=NULL;

  return command;
}
void f(int v)
{}
int main(int argc, char *argv[])
{
   char line[1024];
  char showdown[]="exit\n";
  char **command;
  char *input;
  pid_t child_pid;
  int stat_loc;
  int wait_v;
  signal(SIGINT,f);
 while(1){
    // line[0]='\0';
   //  line[sizeof(line)-1]=~'\0';//clear line
   printf("Yan@~$ ");
    input=fgets(line,sizeof(line),stdin);
    
     if(strcmp(line,showdown)==0){
       printf("Byb,exit!\n");
       while(1)
        exit(1);
    }  
    command=parse(input);
    
     if(!command[0]){
        //free(input);
     //   free(command);
     continue;
   }

    if(strcmp(command[0],"cd")==0){
       cd(command[1]);
      continue;
   }
    child_pid=fork();
     
   if(child_pid==0)
     {execvp(command[0],command);
     }else{
        waitpid(child_pid,&stat_loc,0);
        // wait_v=wait(NULL); 
     }
      
}  


return 0;

}
