#include "ucode.c"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int ls_file(char *fname)
{
  struct stat fstat, *sp = &fstat;
  int i;
  char sbuf[4096];

  // retrieve information about the file
  int r = stat(fname, sp);

  if(r < 0){
    printf("ERROR : %s could not be stated\n", fname);
    exit(1);
  }

  // if regular file
  if((sp->st_mode & 0xF000) == 0x8000){
    printc('-');
  }
  // if directory
  if((sp->st_mode & 0xF000) == 0x4000){
    printc('d');
  }
  // if link
  if((sp->st_mode & 0xF000) == 0xA000){
    printc('l');
  }
  
  for(i = 8; i>= 0; i--){
    if(sp->st_mode & (1 << i)){
      printc(t1[i]);
    }
    else{
      printc(t2[i]);
    }
  }

  // print the nlink
  if(sp->st_nlink < 10){
     printf("  %d ", sp->st_nlink);
  }
  else{
    printf(" %d ", sp->st_nlink);
  }
  // print the uid
  printf("%d ", sp->st_uid);
  // print the gid
  printf("%d ", sp->st_gid);
  // print the size
  printf("%d ",sp->st_size);
  // print the fname
  printf("%s ", fname);
  
  if((sp->st_mode)==0xA000){
    r = readlink(fname,sbuf);
    printf(" -> %s",sbuf);
  }
  printf("\n\r");
}

int ls_dir(char *dname){

  char name[256];
  char buffer[1024];
  DIR *dp;
  int fd;

  // open the dirname for read
  fd = open(dname,O_RDONLY);

  // read the contents into the buffer
  read(fd, buffer, 1024);

  // dp dir_entry
  dp = (DIR *)buffer;
  char *cp = buffer;

  while(cp < buffer + 1024){
    // get the name and length
    strncpy(name, dp->name, dp->name_len);
    name[dp->name_len] = '\0';
    ls_file(name);
    // advancing the cp by rec length
    cp += dp->rec_len;
    // pull the dp to the next entry 
    dp = (DIR *)cp;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  // stdin, stdout, stderr inherited from the shell
  
  printf("********************************************\n"); 
  printf("*         This is HD's ls in action        *\n");
  printf("********************************************\n");
  struct stat fstat, *sp = &fstat;
  char cwd[1024];

  // get the current directory
  getcwd(cwd);
  
  // if there are less than 2 arguments than that means that user just called ls, so ls cwd
  if(argc < 2){
    ls_dir(cwd);
  }
  else{
    // more than two argument so we will check if the pathname is a dir or not and run ls_dir or ls_file accordingly
    int r = stat(argv[1], sp);

    if(r < 0)
    {
       printf("ERROR : %s could not be stated\n", argv[1]);
       exit(1);
    }
    
    // if argument is a dir run ls_dir
    if((sp->st_mode & 0xF000) == 0x4000)
      {
	ls_dir(argv[1]);
      }
    else{
      // argument is a file so ls_file the filename
      ls_file(argv[1]);
    }
  }
}


