#include "ucode.c";

int main(int argc, char *argv[]){
  int fd, fd2;
  char buf[1024];
  int n;

  if(argc < 3){
    printf("ERROR : cp expcets 2 arguments\n");
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);
  fd2 = open(argv[2], O_WRONLY|O_CREAT);

  if(fd < 0){
    printf("ERROR : File cannot be opened\n");
    exit(1);
  }

  // while reading the file
  while(n = read(fd, buf, 1024)){
    // copy to the new file
    write(fd2,buf,n);
  }

  close(fd);
  close(fd2);
  exit(1);
}
