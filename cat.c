#include "ucode.c";

int main(int argc, char *argv[])
{
  int i, fd, n;
  char buf[1024];
  char message[1024] = "******************************\n\r**** HD's crazy cat WOOF! ****\n\r******************************\n\r";

  // writing to stderr because if I write to stdout it will increase the size of cat
    write(2,message,strlen(message));
  
  fd = 0;
  
  // since this is a child process of the shell
  // it already has descriptor 0, 1, 2 open

  // if cat has a parameter
  if(argc > 1){
    // open file for read mode
    fd = open(argv[1], O_RDONLY);
    if(fd < 0){
      printf("ERROR : Failed to open file\n");
      exit(1);
    }
  }

  // if fd is 0, it's to the terminal
  if(fd == 0){
    // while getting user input just print it out
    while(n = gets(buf)){
      buf[n] = 0;
      printf("%s\n\r",buf);
    }
  }
  else{
    // else read from the file and print it to stdout
    while(n = read(fd, buf, 1024)){
      buf[n] = 0;
      printf("%s",buf);
    }
  }

  close(fd);
}
