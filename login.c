// login.c
#include "ucode.c"

int in, out, err;

char login[128], password[128];

char lines[128][256];

char user[10][128];

// function to split the lines by newline
int splitLines(char *buf){
  int j = 0;
  int k = 0;
  char *temp = buf;

  while(*temp != 0){
    if(*temp == '\n'){
      lines[j][k] = 0;
      j++;
      k = 0;
    }
    else{
      lines[j][k] = *temp;
      k++;
    }
    temp++;
  }
  
  return j;
}

// tokenize function that tokenizes :
int tokenize(char *line){
  char *temp = line;
  int i = 0;
  int j = 0;
  while(*temp != 0){
    if(*temp == ' '){
    }
    else if(*temp == ':'){
      user[i][j] = 0;
      i++;
      j = 0;
    }
    else{
      user[i][j] = *temp;
      j++;
    }
    temp++;
  }
}

main(int argc, char *argv[])
{
  int fd, n;
  char buf[1024];
  int i = 0;
  
  //1. close file descriptors 0, 1 inherited from INIT.
  close(0);
  close(1);

  //2. open argv[1] 3 times as in(0), out(1), err(2).
  in = open(argv[1], 0);
  out = open(argv[1], 1);
  err = open(argv[1], 2);

  //3. set the tty name string in PROC.tty
  settty(argv[1]);

  //4. open /etc/passwd file for READ;
  fd = open("/etc/passwd", O_RDONLY);

  // read the passwd file into the buffer
  n = read(fd, buf, 1024);
  buf[n] = 0;

  // split by the new line, to get the user information
  int size = splitLines(buf);
  printf("HDLOGIN : open %s as stdin=0 , stdout=1 , stderr=2\n", argv[1]);
  while(1){
    i = 0;
    // get login and password from user
    printf("login:"); gets(login);
    printf("password:"); gets(password);

    // for each line in /etc/passwd file
    while(i < size){
      // tokenize user account line
      tokenize(lines[i]);
      // if user has a valid account
      if(strcmp(user[0],login) == 0 && strcmp(user[1],password) == 0){
	printf("HDLOGIN : Welcome! %s\n", &user[0]);
	//change uid, gid to user's uid, gid;
	chuid(atoi(user[3]), atoi(user[2]));

	// change cwd to users home DIR
	chdir(user[5]);
	printf("HDLOGIN : cd to HOME=%s\n", &user[5]);

	printf("HDLOGIN : changed uid to %d\n", getuid());
	//close opened /etc/passwd file
	close(fd);

	printf("exec to /bin/sh .....\n");
	exec("sh");
	
	break;
      }
      i++;
    }
    printf("login failed, try again\n");
  }
}
