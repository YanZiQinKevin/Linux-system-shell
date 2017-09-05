#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#define bool int
#define false 0
#define true 1
struct termios oldattr; //globle 

bool t=true;

void set_input_mode ();
void f(int signum){     //function from signal
   if(t==true)
   t=false;
   else
    t=true;
}
int main(int ac, char *args[])
{
   
   tcgetattr(STDIN_FILENO,&oldattr);
  signal(SIGINT, f);
  
   int i=atoi(args[1]);
   int temp=i, control=0;
  char c;
   set_input_mode();
 while(1){
   while(t){
         printf("%d\n",i);
     i--;
     sleep(1);
       
   }
}
  tcsetattr(STDIN_FILENO,TCSANOW,&oldattr);//reset
}
void set_input_mode ()
{
  struct termios tattr;
  char *name;

/* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

/* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON | ECHO);	/* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tattr.c_iflag |=IXON|IXOFF;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

