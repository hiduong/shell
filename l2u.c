#include "ucode.c";

int main(int argc, char *argv[]){

  char character;
  int fd, fd2;
  int n;
  // if no parameters read from stdin if character is lower case print upper case
  if(argc == 1){
    while(n = read(0, &character, 1)){
      if(character >= 'a' && character <= 'z'){
	printc(character - 32);
      }
      else{
	printc(character);
      }
    }
    exit(1);
  }

  if(argc == 2){
    printf("ERROR : l2u expects 2 arguments\n");
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);
  fd2 = open(argv[2], O_WRONLY|O_CREAT);

  if(fd < 0){
    printf("ERROR : File cannot be opened\n");
    exit(1);
  }

  // read from file if character is lowercase write uppercase character
  while(n = read(fd,&character,1)){
   if(character >= 'a' && character <= 'z'){
     character = character - 32;
        write(fd2,&character,1);
      }
      else{
	write(fd2,&character,1);
      }
  }
  close(fd);
  close(fd2);
  exit(1);
}
