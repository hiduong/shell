#include "ucode.c"

int main(int argc, char *argv[])
{
  int fd, i, n;
  char input;
  char buffer[1024];
  int in;
  char tty[1024];
  char message[1024] = "******************************\n\r**** HD's MORE IN ACTION  ****\n\r******************************\n\r";

  write(2,message,strlen(message));

  // stdin, stdout, and stderr is inherited from the shell

  // obtain the tty
  gettty(tty);
  // open tty for user input
  in = open(tty, O_RDONLY);
  
  // if there is a parameter
  if(argc > 1){
    // close the stdin
     close(0);
    // open the file for read
    fd = open(argv[1], O_RDONLY);
    if(fd < 0){
      printf("---ERROR : FAILED TO OPEN FILE---\n");
      exit(1);
    }
  }

  // read the first screen of lines
  i = 0;
  while(i < 26){
    n = getline(buffer);
    buffer[n] = 0;
    printf("%s",buffer);
    i++;
  }
  // if nothing left to read exit
  if(n == 0){
    exit(1);
  }

  while(1){
    // read the user input
    read(in, &input, 1);

    // if space bar display more screen
    if(input == ' '){
      i = 0;
      while(i < 26){
	 n = getline(buffer);
	 buffer[n] = 0;
	 printf("%s",buffer);
	 i++;
      }
    }
    else{
      // else display 1 line
      n = getline(buffer);
      buffer[n] = 0;
      printf("%s",buffer);
    }
    // if full exit
    if(n == 0){
       exit(1);
    }
  }
}
