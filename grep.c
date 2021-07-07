#include "ucode.c";

int main(int argc, char *argv[]){
  STAT st0, myst;
  char tty[64];
  int red0;

  char c;
  int fd = 0;
  char line[1024];
  int n;
  char message[1024] = "******************************\n\r**** HD's GREP IN ACTION  ****\n\r******************************\n\r";

  // fd 0,1,2 inherited from shell
  write(2,message,strlen(message));

  gettty(tty);
  stat(tty, &myst);
  fstat(0, &st0);
  if (st0.st_ino == myst.st_ino)
    red0 = 0;
  else
    red0 = 1;
  
  // if there are 3 argument we grep from a file
  if(argc == 3){
    // close fd 0 so we can open the file at fd 0
    close(0);
    fd = open(argv[2],O_RDONLY);
    if(fd < 0){
      printf("ERROR : Failed to open file\n");
      exit(1);
    }
    // read each line of the file
    while(n = getline(line)){
      // if the pattern we want matches anywhere in the line, print the line.
      if(strstr(line,argv[1])!=0){
	printf("%s",line);
      }
    }
  }
  else{
    if(red0){
      while(n = getline(line)){
	line[n] = 0;
	// if the pattern we want matches anywhere in the line, print the line.
	if(strstr(line,argv[1])!=0){
	  printf("%s",line);
	}
      }
    }
    else{
      while(n = gets(line)){
	strcat(line,"\n");
	line[n] = 0;
	// if the pattern we want matches anywhere in the line, print the line.
	if(strstr(line,argv[1])!=0){
	  printf("%s",line);
	}
      }
    }
  }
}
