
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
typedef struct stat s;

int checktype(char*); // if file return 1, if directory return 2, if other return 0;
int checkdir(char *); // if current dir is empty,return 0;  if not, return 1;
int main(int ac, char* args[])
{
  
  if (ac!=2){
    printf("usage:%s ENTRY\n",args[0]);
    exit(1);
  }
  
  int a= checktype(args[1]);
  if(a==1)   //file
  {
  remove(args[1]);
  }
   else if(a==2)  //dir
   {
     int t=checkdir(args[1]);
     if(t==0)
   {
     remove(args[1]); 
    }else if(t==1)
     printf("failed to remove directory: Directory not empty\n");
     exit(3);
   }
	
  return 0;
}



int checktype(char* fname)
 {
  struct stat s;
   int a=stat(fname,&s);
 
if(a==-1)  //check empty
  {
       printf("failed to remove: Not find\n");
	exit(2);}  //not find 
 else if((s.st_mode&0170000)==0100000)  //it is file
   
      return 1;     
    else if ((s.st_mode&0170000)==0040000)
    
	return 2;  // it is dir
  
   return 0;
  
}

 int checkdir(char *cname)
 {
   DIR *d;
   struct dirent *dir;
  int sum=0;
 d=opendir(cname);
  if(d){
    
    while((dir=readdir(d))!=NULL)
     {
    sum++;
   }
    closedir(d);
   //printf("%d\n",sum);       
   if(sum==2)   //only has ".",".."
     return 0 ; 
 else
   return 1;
  }
 return 1;
}























































