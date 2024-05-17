/////////////////////////////////////////////////////////////////////
// From:
// http://www.thelinuxdaily.com/2010/05/grab-raw-keyboard-input-from-event-device-node-devinputevent/
//
// Requires an input parameter of the form /dev/input/event0
//
// compile: gcc keyboard.c -o key
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
 
void handler (int sig)
{
  printf ("nexiting...(%d)\n", sig);
  exit (0);
}
 
void perror_exit (char *error)
{
  perror (error);
  handler (9);
}
 
int main (int argc, char *argv[])
{
  struct input_event ev[64];
  int fd, rd, value, size = sizeof (struct input_event);
  char name[256] = "Unknown";
  char *device = NULL;
  int i = 0;
 
  //Setup check
  if (argv[1] == NULL){
      printf("Please specify (on the command line) the path to the dev event interface device\n");
      exit (0);
    }
 
  if ((getuid ()) != 0)
    printf ("You are not root! This may not work...\n");
 
  if (argc > 1)
    device = argv[1];
 
  //Open Device
  if ((fd = open (device, O_RDONLY)) == -1)
    printf ("%s is not a vaild device.\n", device);
 
  //Print Device Name
  ioctl (fd, EVIOCGNAME (sizeof (name)), name);
  printf ("Reading From : %s (%s)\n", device, name);

  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);

  while (1){
      // if ((rd = read (fd, ev, size * 64)) < size)
      //     perror_exit ("read()\n");
      
      rd = read (fd, ev, size * 64) ;
 
      value = ev[0].value;
 
      if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
       printf ("Code[%d]\n", (ev[1].code));
      }

      printf("Value is %d\n", i++);
      if(i>1000){
        i = 0;
      }

  }
 
  return 0;
} 