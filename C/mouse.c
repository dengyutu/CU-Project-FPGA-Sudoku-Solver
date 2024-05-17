///////////////////////////////////////////////////////////////////////
// Mouse test from 
// http://stackoverflow.com/questions/11451618/how-do-you-read-the-mouse-button-state-from-dev-input-mice
//
// Native ARM GCC Compile: gcc mouse.c -o mouse
//
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int fd, bytes;
    unsigned char data[3];

    const char *pDevice = "/dev/input/mice";

    // Open Mouse
    fd = open(pDevice, O_RDWR);
    if(fd == -1)
    {
        printf("ERROR Opening %s\n", pDevice);
        return -1;
    }

    // int flags = fcntl(fd, F_GETFL, 0);
    // fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    int left, middle, right;
    signed char x, y;
    int i = 0;
    while(1)
    {
        // Read Mouse     
        bytes = read(fd, data, sizeof(data));

        if(bytes > 0)
        {
            left = data[0] & 0x1;
            right = data[0] & 0x2;
            middle = data[0] & 0x4;

            x = data[1];
            y = data[2];
            printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
        } 
        
        // printf("Value: %d\n", i++);
        // if(i>1000){
        //     i = 0;
        // }

        // usleep(100000);
    }
    return 0; 
}
