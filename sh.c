#include "ucode.c";

char cline[128];
char cmd[16][64];
char splitCommands[16][64];
char ioCommands[2][64];
char home[128];

int do_pipes(int count,int i, int j){
  int pid, pd[2],ioCount;
  pipe(pd); // create a pipe: pd[0] = read, pd[1] = write
  pid = fork(); // fork a child to share the pipe
  if(pid){ // parent: as the reader
    close(pd[1]); // close the pipe's write end
    dup2(pd[0],0); // redirect stdin to the pipe read end

    if(count > 1){
      count = count - 1;
      i = i + 1;
      j = j + 1;
      do_pipes(count,i,j);
    }
    else{    
      memset(ioCommands[0],0,64);
      memset(ioCommands[1],0,64);
      int ioCount = contains_redirectionPipes(j);
      if(ioCount == 1 || ioCount == 2 || ioCount == 3){
	do_redirectionPipes(ioCount);
      }
      else{
	exec(splitCommands[j]);
      }
    }
  }
  else{ // child : is the piper writer
    close(pd[0]); // close the pipe's read end
    dup2(pd[1],1); // redirect stdout to piper write end
    memset(ioCommands[0],0,64);
    memset(ioCommands[1],0,64);
    int ioCount = contains_redirectionPipes(i);
    if(ioCount == 1 || ioCount == 2 || ioCount == 3){
      do_redirectionPipes(ioCount);
    }
    else{
      exec(splitCommands[i]);
    }
  }
}

int count_pipes(){
  int i = 0;
  int count = 0;
  char temp[64];
  temp[0] = 0;
  while(i < 16){
    if(strcmp(cmd[i],"|")==0){
      strcpy(splitCommands[count],temp);
      memset(temp,0,64);
      count++;
    }
    else{
      if(cmd[i][0] != 0){
	if(temp[0] == 0){
	  strcat(temp,cmd[i]);
	}
	else{
	  strcat(temp," ");
	  strcat(temp,cmd[i]);
	}
      }
    }
    i++;
  }
  strcpy(splitCommands[count],temp);
  return count;
}

int tokenize(char *line){
  char *temp = line;
  int i = 0;
  int j = 0;
  while(*temp != 0){
    if(*temp == ' '){
      cmd[i][j] = 0;
      i++;
      j = 0;
    }
    else{
      cmd[i][j] = *temp;
      j++;
    }
    temp++;
  }
}

int contains_redirectionPipes(int index){
  char *temp = splitCommands[index];
  int i = 0;
  int j = 0;
  int count = 0;
  while(*temp != 0){
    if(*temp == '<'){
      temp++;
      ioCommands[i][j] = 0;
      i++;
      j = 0;
      if(*temp == '<'){
	count = 3;
      }
      else{
	count = 1;
	temp--;
      }
    }
    else if(*temp == '>'){
      ioCommands[i][j] = 0;
      i++;
      j = 0;
      count = 2;
    }
    else{
      if(i == 1){
	if(*temp == ' '){	  
	}
	else{
	  ioCommands[i][j] = *temp;
	  j++;
	}
      }
      else{
	ioCommands[i][j] = *temp;
	j++;
      }
    }
    temp++;
  }
  
  return count;
}


int contains_redirection(){
  int i = 0;  
  int count = 0;
  char temp[64];
  
  i = 0;
  
  temp[0] = 0;
  while(i < 16){
    if(strcmp(cmd[i],"<")==0){
      strcpy(ioCommands[0],temp);
      strcpy(ioCommands[1],cmd[i + 1]);
      memset(temp,0,64);
      count = 1;
    }
    else if(strcmp(cmd[i],">")==0){
      strcpy(ioCommands[0],temp);
      strcpy(ioCommands[1],cmd[i + 1]);
      memset(temp,0,64);
      count = 2;
    }
    else if(strcmp(cmd[i],"<<")==0){
      strcpy(ioCommands[0],temp);
      strcpy(ioCommands[1],cmd[i + 1]);
      memset(temp,0,64);
      count = 3;
    }
    else{
      if(cmd[i][0] != 0){
	if(temp[0] == 0){
	  strcat(temp,cmd[i]);
	}
	else{
	  strcat(temp," ");
	  strcat(temp,cmd[i]);
	}
      }
    }
    i++;
  }
  return count;
}

int do_redirection(int count){
  int status, pid, fd;
  // < read
  if(count == 1){
    pid = fork();
    if(pid < 0){
      write(2, "Failed to fork\n", 14);
    }
    else if(pid){
      //  wait for child to die
      pid = wait(&status);
    }
    else{
      close(0);
      fd = open(ioCommands[1],O_RDONLY);
      exec(ioCommands[0]);
    }
  }
  else if(count == 2){
    // > write
    pid = fork();
    if(pid < 0){
      write(2, "Failed to fork\n", 14);
    }
    else if(pid){
      //  wait for child to die
      pid = wait(&status);
    }
    else{
      close(1);
      fd = open(ioCommands[1],O_WRONLY|O_CREAT|O_TRUNC);
      exec(ioCommands[0]);
    }
  }
   else if(count == 3){
    // >> append
    pid = fork();
    if(pid < 0){
      write(2, "Failed to fork\n", 14);
    }
    else if(pid){
      //  wait for child to die
      pid = wait(&status);
    }
    else{
      close(1);
      fd = open(ioCommands[1],O_WRONLY|O_APPEND|O_CREAT);
      exec(ioCommands[0]);
    }
  }   
}

int do_redirectionPipes(int count){
  int fd;
  // < read
  if(count == 1){
    close(0);
    fd = open(ioCommands[1],O_RDONLY);
    exec(ioCommands[0]);
  }
  else if(count == 2){
    close(1);
    fd = open(ioCommands[1],O_WRONLY|O_CREAT|O_TRUNC);
    exec(ioCommands[0]);
  }
  else if(count == 3){
    close(1);
    fd = open(ioCommands[1],O_WRONLY|O_APPEND|O_CREAT);
    exec(ioCommands[0]);
  }   
}

int main(int argc, char * argv[]){
  int i;
  int pid;
  int status;
 
  // get the home dir
  getcwd(home);
  
  // --------- HDSH PROCESSING LOOP ---------
  while(1){
    // reading the command
    write(2, "HDSH : ", 7);
    gets(cline);

    i = 0;
    //reset the commands
    while(i < 16){
      memset(cmd[i],0,64);
      i++;
    }

    i = 0;
    while(i < 16){
      memset(splitCommands[i],0,64);
      i++;
    }

    // tokenize by white space
    i = tokenize(cline);
    
    // if the command is cd
    if(strcmp(cmd[0],"cd") == 0){
      // if just cd , cd to home dir
      if(cmd[1][0] == 0){
	chdir(home);
      }
      else{
	// cd path
	chdir(cmd[1]);
      }
    }
    // if the command is exit
    else if(strcmp(cmd[0], "logout") == 0){
      printf("GOODBYE!\n");
      exit(0);
    }
    else if(cmd[0][0] == 0){
      continue;
    }
    else{
      // check if there are any pipes
      int count = count_pipes();
      if(count >= 1){
	pid = fork();
	if(pid < 0){
	  write(2, "Failed to fork\n", 14);
	}
	else if(pid){
	  //  wait for child to die
	  pid = wait(&status);
	}
	else{
	  do_pipes(count,0,1);
	}
      }
      else{
	// check to see if there are redirections
	int ioCount = contains_redirection();
	if(ioCount == 1 || ioCount == 2 || ioCount == 3){
	  do_redirection(ioCount);
	}
	else{
	  //if simple commands
	  pid = fork();
	  if(pid < 0){
	    write(2, "Failed to fork\n", 14);
	  }
	  else if(pid){
	    //  wait for child to die
	    pid = wait(&status);
	  }
	  else{
	    exec(cline);
	  }
	}
      }
    }   
  }
}
