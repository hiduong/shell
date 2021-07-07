#include "ucode.c"
#define printk printf

int tty0;
int ttys0;
int ttys1;

int main(int argc, char *argv[ ])
{
  int in, out;
  int pid, status;

  // open stdin and stdout for read write
  in  = open("/dev/tty0", O_RDONLY);
  out = open("/dev/tty0", O_WRONLY);

  pid = getpid();

  printf("HDINIT %d: fork a login proc on console\n", pid);

  // fork a process
  tty0 = fork();

  // parent
  if(tty0)
  {
    // fork second proc for ttys0
    ttys0 = fork();
    //parent
    if(ttys0)
    {
      // fork third proc for ttys1
      ttys1 = fork();
      // parent
      if(ttys1)
      {
	// wait for childs to die
	while(1){
	  printf("HDINIT : waiting for ZOMBIE child\n");
	  pid = wait(&status);
	  
	  // if dead child is proc for tty0
	  if(pid == tty0){
	    printf("HDINIT %d: fork a login proc on console\n", pid);
	    tty0 = fork();
	    // if parent just continue
	    if(tty0){
	      continue;
	    }
	    else{
	      // child exec a new login for tty0
	      exec("login /dev/tty0");
	    }
	  }

	   // if dead child is proc for ttyS0
	  if(pid == ttys0){
	    printf("HDINIT %d: fork a login proc on console\n", pid);
	    ttys0 = fork();
	    // if parent just continue
	    if(ttys0){
	      continue;
	    }
	    else{
	      // child exec a new login for ttyS0
	      exec("login /dev/ttyS0");
	    }
	  }

	  // if dead child is proc for ttyS1
	  if(pid == ttys1){
	    printf("HDINIT %d: fork a login proc on console\n", pid);
	    ttys1 = fork();
	    // if parent just continue
	    if(ttys1){
	      continue;
	    }
	    else{
	      // child exec a new login for ttyS1
	      exec("login /dev/ttyS1");
	    }
	  }
	  printf("HDINIT : I just buried an orphan child proc %d\n", pid);
	}
      }
      else{
	exec("login /dev/ttyS1");
      }
    }
    else{
      // child executes login ttys0
      exec("login /dev/ttyS0");
    }
  }
  else{
    // child executes login tty0
    exec("login /dev/tty0");
  }
}




