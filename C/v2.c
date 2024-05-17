///////////////////////////////////////
/// 640x480 version! 16-bit color
/// This code will segfault the original
/// DE1 computer
/// compile with
/// gcc v2.c -o gr -O2 -lm
///
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/mman.h>
#include <sys/time.h> 
#include <math.h>
//#include "address_map_arm_brl4.h"

// video display
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define SDRAM_SPAN			  0x04000000
// characters
#define FPGA_CHAR_BASE        0xC9000000 
#define FPGA_CHAR_END         0xC9001FFF
#define FPGA_CHAR_SPAN        0x00002000
/* Cyclone V FPGA devices */
#define HW_REGS_BASE          0xff200000
//#define HW_REGS_SPAN        0x00200000 
#define HW_REGS_SPAN          0x00005000 

// graphics primitives
void VGA_text (int, int, char *);
void VGA_text_clear();
void VGA_box (int, int, int, int, short);
void VGA_rect (int, int, int, int, short);
void VGA_line(int, int, int, int, short) ;
void VGA_Vline(int, int, int, short) ;
void VGA_Hline(int, int, int, short) ;
void VGA_disc (int, int, int, short);
void VGA_circle (int, int, int, int);
void VGA_one(int, int, short);
void VGA_two(int, int, short);
void VGA_three(int, int, short);
void VGA_four_1(int, int, short);
void VGA_four_2(int, int, short);
void VGA_five(int, int, short);
void VGA_six(int, int, short);
void VGA_seven(int, int, short);
void VGA_eight(int, int, short);
void VGA_nine(int, int, short);
// 16-bit primary colors
#define red  (0+(0<<5)+(31<<11))
#define dark_red (0+(0<<5)+(15<<11))
#define green (0+(63<<5)+(0<<11))
#define dark_green (0+(31<<5)+(0<<11))
#define blue (31+(0<<5)+(0<<11))
#define dark_blue (15+(0<<5)+(0<<11))
#define yellow (0+(63<<5)+(31<<11))
#define cyan (31+(63<<5)+(0<<11))
#define magenta (31+(0<<5)+(31<<11))
#define black (0x0000)
#define gray (15+(31<<5)+(51<<11))
#define white (0xffff)
int colors[] = {red, dark_red, green, dark_green, blue, dark_blue, 
		yellow, cyan, magenta, gray, black, white};

// pixel macro
#define VGA_PIXEL(x,y,color) do{\
	int  *pixel_ptr ;\
	pixel_ptr = (int*)((char *)vga_pixel_ptr + (((y)*640+(x))<<1)) ; \
	*(short *)pixel_ptr = (color);\
} while(0)

// the light weight buss base
void *h2p_lw_virtual_base;

// pixel buffer
volatile unsigned int * vga_pixel_ptr = NULL ;
void *vga_pixel_virtual_base;

// character buffer
volatile unsigned int * vga_char_ptr = NULL ;
void *vga_char_virtual_base;

// /dev/mem file id
int fd;

// measure time
struct timeval t1, t2;
double elapsedTime;

// size of the number in pixel
int s_length = 8;
int s_width = 2; // stroke of the number in pixel
	
int main(void)
{
  	
	// === need to mmap: =======================
	// FPGA_CHAR_BASE
	// FPGA_ONCHIP_BASE      
	// HW_REGS_BASE        
  
	// === get FPGA addresses ==================
    // Open /dev/mem
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) 	{
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
    
    // get virtual addr that maps to physical
	h2p_lw_virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );	
	if( h2p_lw_virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap1() failed...\n" );
		close( fd );
		return(1);
	}
    

	// === get VGA char addr =====================
	// get virtual addr that maps to physical
	vga_char_virtual_base = mmap( NULL, FPGA_CHAR_SPAN, ( 	PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FPGA_CHAR_BASE );	
	if( vga_char_virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap2() failed...\n" );
		close( fd );
		return(1);
	}
    
    // Get the address that maps to the FPGA LED control 
	vga_char_ptr =(unsigned int *)(vga_char_virtual_base);

	// === get VGA pixel addr ====================
	// get virtual addr that maps to physical
	vga_pixel_virtual_base = mmap( NULL, SDRAM_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, SDRAM_BASE);	
	if( vga_pixel_virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap3() failed...\n" );
		close( fd );
		return(1);
	}
    
    // Get the address that maps to the FPGA pixel buffer
	vga_pixel_ptr =(unsigned int *)(vga_pixel_virtual_base);

	//mouse reading
	int bytes;
    unsigned char data[3];

    const char *pDevice = "/dev/input/mice";

    // Open Mouse
    fd = open(pDevice, O_RDWR);
    if(fd == -1)
    {
        printf("ERROR Opening %s\n", pDevice);
        return -1;
    }

	//non-blocking
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    int left, middle, right;
    signed char x, y;

	int i;
	int cell = 9;
	int spacing = 35;
	int x_start = (640-spacing*cell)/2;
	int y_start = (480-spacing*cell)/2;
	int mouse_x = 320;
	int mouse_y = 240;
	int mouse_x_old = mouse_x;
	int mouse_y_old = mouse_y;

	//clear the screen
	VGA_box (0, 0, 639, 479, 0x0000);

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
			mouse_x = mouse_x + x;
			mouse_y = mouse_y - y;
            printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
        }

		if(mouse_x<=0){
			mouse_x = 0;
		}
		else if(mouse_x>=640){
			mouse_x = 640;
		}

		if(mouse_y<=0){
			mouse_y = 0;
		}
		else if(mouse_y>=480){
			mouse_y = 480;
		}
		VGA_disc(mouse_x_old,mouse_y_old,5,0x0000);
		VGA_disc(mouse_x,mouse_y,5,red);

		for(i=0;i<10;i++){
		if(i%3==0){
		VGA_Vline(x_start+i*spacing,y_start,y_start+spacing*cell,white); // x1,y1.y2
		VGA_Hline(x_start,y_start+i*spacing,x_start+spacing*cell,white); // x1.y1.x2
		}
		else{
		VGA_Vline(x_start+i*spacing,y_start,y_start+spacing*cell,gray); // x1,y1.y2
		VGA_Hline(x_start,y_start+i*spacing,x_start+spacing*cell,gray); // x1.y1.x2
		}
	}

		int digit_start = x_start+17;
		VGA_one(digit_start,y_start+8,cyan);
		VGA_two(digit_start,y_start+35+8,cyan);
		VGA_three(digit_start,y_start+35*2+8,cyan);
		VGA_four_2(digit_start,y_start+35*3+8,cyan);
		VGA_five(digit_start,y_start+35*4+8,cyan);
		VGA_six(digit_start,y_start+35*5+8,cyan);
		VGA_seven(digit_start,y_start+35*6+8,cyan);
		VGA_eight(digit_start,y_start+35*7+8,cyan);
		VGA_nine(digit_start,y_start+35*8+8,cyan);

		VGA_two(digit_start+spacing,y_start+8,cyan);
		VGA_three(digit_start+spacing*2,y_start+8,cyan);
		VGA_four_2(digit_start+spacing*3,y_start+8,cyan);
		VGA_five(digit_start+spacing*4,y_start+8,cyan);
		VGA_six(digit_start+spacing*5,y_start+8,cyan);
		VGA_seven(digit_start+spacing*6,y_start+8,cyan);
		VGA_eight(digit_start+spacing*7,y_start+8,cyan);
		VGA_nine(digit_start+spacing*8,y_start+8,cyan);

		mouse_x_old = mouse_x;
		mouse_y_old = mouse_y;
		usleep(33000);
	}



} // end main

/****************************************************************************************
 * Subroutine to send a string of text to the VGA monitor 
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr)
{
  	volatile char * character_buffer = (char *) vga_char_ptr ;	// VGA character buffer
	int offset;
	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		// write to the character buffer
		*(character_buffer + offset) = *(text_ptr);	
		++text_ptr;
		++offset;
	}
}

/****************************************************************************************
 * Subroutine to clear text to the VGA monitor 
****************************************************************************************/
void VGA_text_clear()
{
  	volatile char * character_buffer = (char *) vga_char_ptr ;	// VGA character buffer
	int offset, x, y;
	for (x=0; x<79; x++){
		for (y=0; y<59; y++){
	/* assume that the text string fits on one line */
			offset = (y << 7) + x;
			// write to the character buffer
			*(character_buffer + offset) = ' ';		
		}
	}
}

/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor 
****************************************************************************************/
#define SWAP(X,Y) do{int temp=X; X=Y; Y=temp;}while(0) 

void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
	char  *pixel_ptr ; 
	int row, col;

	/* check and fix box coordinates to be valid */
	if (x1>639) x1 = 639;
	if (y1>479) y1 = 479;
	if (x2>639) x2 = 639;
	if (y2>479) y2 = 479;
	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (x2<0) x2 = 0;
	if (y2<0) y2 = 0;
	if (x1>x2) SWAP(x1,x2);
	if (y1>y2) SWAP(y1,y2);
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			//640x480
			//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
			// set pixel color
			//*(char *)pixel_ptr = pixel_color;	
			VGA_PIXEL(col,row,pixel_color);	
		}
}

/****************************************************************************************
 * Draw a outline rectangle on the VGA monitor 
****************************************************************************************/
#define SWAP(X,Y) do{int temp=X; X=Y; Y=temp;}while(0) 

void VGA_rect(int x1, int y1, int x2, int y2, short pixel_color)
{
	char  *pixel_ptr ; 
	int row, col;

	/* check and fix box coordinates to be valid */
	if (x1>639) x1 = 639;
	if (y1>479) y1 = 479;
	if (x2>639) x2 = 639;
	if (y2>479) y2 = 479;
	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (x2<0) x2 = 0;
	if (y2<0) y2 = 0;
	if (x1>x2) SWAP(x1,x2);
	if (y1>y2) SWAP(y1,y2);
	// left edge
	col = x1;
	for (row = y1; row <= y2; row++){
		//640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;	
		VGA_PIXEL(col,row,pixel_color);		
	}
		
	// right edge
	col = x2;
	for (row = y1; row <= y2; row++){
		//640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;	
		VGA_PIXEL(col,row,pixel_color);		
	}
	
	// top edge
	row = y1;
	for (col = x1; col <= x2; ++col){
		//640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;	
		VGA_PIXEL(col,row,pixel_color);
	}
	
	// bottom edge
	row = y2;
	for (col = x1; col <= x2; ++col){
		//640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;
		VGA_PIXEL(col,row,pixel_color);
	}
}

/****************************************************************************************
 * Draw a horixontal line on the VGA monitor 
****************************************************************************************/
#define SWAP(X,Y) do{int temp=X; X=Y; Y=temp;}while(0) 

void VGA_Hline(int x1, int y1, int x2, short pixel_color)
{
	char  *pixel_ptr ; 
	int row, col;

	/* check and fix box coordinates to be valid */
	if (x1>639) x1 = 639;
	if (y1>479) y1 = 479;
	if (x2>639) x2 = 639;
	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (x2<0) x2 = 0;
	if (x1>x2) SWAP(x1,x2);
	// line
	row = y1;
	for (col = x1; col <= x2; ++col){
		//640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;	
		VGA_PIXEL(col,row,pixel_color);		
	}
}

/****************************************************************************************
 * Draw a vertical line on the VGA monitor 
****************************************************************************************/
#define SWAP(X,Y) do{int temp=X; X=Y; Y=temp;}while(0) 

void VGA_Vline(int x1, int y1, int y2, short pixel_color)
{
	char  *pixel_ptr ; 
	int row, col;

	/* check and fix box coordinates to be valid */
	if (x1>639) x1 = 639;
	if (y1>479) y1 = 479;
	if (y2>479) y2 = 479;
	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (y2<0) y2 = 0;
	if (y1>y2) SWAP(y1,y2);
	// line
	col = x1;
	for (row = y1; row <= y2; row++){
		//640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10)    + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;	
		VGA_PIXEL(col,row,pixel_color);			
	}
}


/****************************************************************************************
 * Draw a filled circle on the VGA monitor 
****************************************************************************************/

void VGA_disc(int x, int y, int r, short pixel_color)
{
	char  *pixel_ptr ; 
	int row, col, rsqr, xc, yc;
	
	rsqr = r*r;
	
	for (yc = -r; yc <= r; yc++)
		for (xc = -r; xc <= r; xc++)
		{
			col = xc;
			row = yc;
			// add the r to make the edge smoother
			if(col*col+row*row <= rsqr+r){
				col += x; // add the center point
				row += y; // add the center point
				//check for valid 640x480
				if (col>639) col = 639;
				if (row>479) row = 479;
				if (col<0) col = 0;
				if (row<0) row = 0;
				//pixel_ptr = (char *)vga_pixel_ptr + (row<<10) + col ;
				// set pixel color
				//*(char *)pixel_ptr = pixel_color;
				VGA_PIXEL(col,row,pixel_color);	
			}
					
		}
}

/****************************************************************************************
 * Draw a  circle on the VGA monitor 
****************************************************************************************/

void VGA_circle(int x, int y, int r, int pixel_color)
{
	char  *pixel_ptr ; 
	int row, col, rsqr, xc, yc;
	int col1, row1;
	rsqr = r*r;
	
	for (yc = -r; yc <= r; yc++){
		//row = yc;
		col1 = (int)sqrt((float)(rsqr + r - yc*yc));
		// right edge
		col = col1 + x; // add the center point
		row = yc + y; // add the center point
		//check for valid 640x480
		if (col>639) col = 639;
		if (row>479) row = 479;
		if (col<0) col = 0;
		if (row<0) row = 0;
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10) + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;
		VGA_PIXEL(col,row,pixel_color);	
		// left edge
		col = -col1 + x; // add the center point
		//check for valid 640x480
		if (col>639) col = 639;
		if (row>479) row = 479;
		if (col<0) col = 0;
		if (row<0) row = 0;
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10) + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;
		VGA_PIXEL(col,row,pixel_color);	
	}
	for (xc = -r; xc <= r; xc++){
		//row = yc;
		row1 = (int)sqrt((float)(rsqr + r - xc*xc));
		// right edge
		col = xc + x; // add the center point
		row = row1 + y; // add the center point
		//check for valid 640x480
		if (col>639) col = 639;
		if (row>479) row = 479;
		if (col<0) col = 0;
		if (row<0) row = 0;
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10) + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;
		VGA_PIXEL(col,row,pixel_color);	
		// left edge
		row = -row1 + y; // add the center point
		//check for valid 640x480
		if (col>639) col = 639;
		if (row>479) row = 479;
		if (col<0) col = 0;
		if (row<0) row = 0;
		//pixel_ptr = (char *)vga_pixel_ptr + (row<<10) + col ;
		// set pixel color
		//*(char *)pixel_ptr = pixel_color;
		VGA_PIXEL(col,row,pixel_color);	
	}
}

// =============================================
// === Draw a line
// =============================================
//plot a line 
//at x1,y1 to x2,y2 with color 
//Code is from David Rodgers,
//"Procedural Elements of Computer Graphics",1985
void VGA_line(int x1, int y1, int x2, int y2, short c) {
	int e;
	signed int dx,dy,j, temp;
	signed int s1,s2, xchange;
     signed int x,y;
	char *pixel_ptr ;
	
	/* check and fix line coordinates to be valid */
	if (x1>639) x1 = 639;
	if (y1>479) y1 = 479;
	if (x2>639) x2 = 639;
	if (y2>479) y2 = 479;
	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (x2<0) x2 = 0;
	if (y2<0) y2 = 0;
        
	x = x1;
	y = y1;
	
	//take absolute value
	if (x2 < x1) {
		dx = x1 - x2;
		s1 = -1;
	}

	else if (x2 == x1) {
		dx = 0;
		s1 = 0;
	}

	else {
		dx = x2 - x1;
		s1 = 1;
	}

	if (y2 < y1) {
		dy = y1 - y2;
		s2 = -1;
	}

	else if (y2 == y1) {
		dy = 0;
		s2 = 0;
	}

	else {
		dy = y2 - y1;
		s2 = 1;
	}

	xchange = 0;   

	if (dy>dx) {
		temp = dx;
		dx = dy;
		dy = temp;
		xchange = 1;
	} 

	e = ((int)dy<<1) - dx;  
	 
	for (j=0; j<=dx; j++) {
		//video_pt(x,y,c); //640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (y<<10)+ x; 
		// set pixel color
		//*(char *)pixel_ptr = c;
		VGA_PIXEL(x,y,c);			
		 
		if (e>=0) {
			if (xchange==1) x = x + s1;
			else y = y + s2;
			e = e - ((int)dx<<1);
		}

		if (xchange==1) y = y + s2;
		else x = x + s1;

		e = e + ((int)dy<<1);
	}
}

void VGA_one(int x1, int y1, short c) {
	// VGA_Vline(x1, y1, y1+2*s_length, c);
	VGA_box(x1+s_length/2, y1, x1+s_width+s_length/2, y1+2*s_length+s_width, c);
}

void VGA_two(int x1, int y1, short c) {
	// VGA_Vline(x1, y1, y1+2*s_length, c);
	VGA_box(x1+(s_width/2), y1, x1+s_length, y1+s_width, c);
	VGA_box(x1+s_length, y1, x1+s_length+s_width, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_width, y1+s_length*2, c);
	VGA_box(x1, y1+s_length*2, x1+s_length+(s_width/2), y1+s_length*2+s_width, c);
}

void VGA_three(int x1, int y1, short c) {
	// VGA_Vline(x1, y1, y1+2*s_length, c);
	VGA_box(x1, y1, x1+s_length, y1+s_width, c);
	VGA_box(x1+s_length, y1, x1+s_length+s_width, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	VGA_box(x1+s_length, y1+s_length, x1+s_length+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1, y1+s_length*2, x1+s_length+s_width, y1+s_length*2+s_width, c);
}

void VGA_four_1(int x1, int y1, short c) {
	VGA_box(x1, y1+s_width, x1+s_width, y1+s_width+s_length, c);
	VGA_box(x1, y1+s_length+s_width, x1+s_length, y1+s_length+s_width*2, c);
	VGA_box(x1+s_length-s_width, y1+s_width, x1+s_length, y1+s_width+s_length, c);
	VGA_box(x1+s_length-s_width, y1+s_width*2+s_length, x1+s_length, y1+s_width*2+s_length*2, c);
}

void VGA_four_2(int x1, int y1, short c) {
	VGA_box(x1, y1, x1+s_width, y1+s_length, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	VGA_box(x1+s_length, y1, x1+s_length+s_width, y1+s_width+s_length, c);
	VGA_box(x1+s_length, y1+s_width+s_length, x1+s_length+s_width, y1+s_width+s_length*2, c);
}

void VGA_five(int x1, int y1, short c) {
	VGA_box(x1, y1, x1+s_length+s_width, y1+s_width, c);
	VGA_box(x1, y1, x1+s_width, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	VGA_box(x1+s_length, y1+s_length, x1+s_length+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1, y1+s_length*2, x1+s_length, y1+s_length*2+s_width, c);
}

void VGA_six(int x1, int y1, short c) {
	VGA_box(x1, y1, x1+s_length+s_width, y1+s_width, c);
	VGA_box(x1, y1, x1+s_width, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1+s_length, y1+s_length, x1+s_length+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1, y1+s_length*2, x1+s_length, y1+s_length*2+s_width, c);
}

void VGA_seven(int x1, int y1, short c) {
	VGA_box(x1, y1, x1+s_length, y1+s_width, c);
	VGA_box(x1+s_length, y1, x1+s_length+s_width, y1+s_length+s_width, c);
	VGA_box(x1+s_length, y1+s_length, x1+s_length+s_width, y1+s_length*2+s_width, c);
}

void VGA_eight(int x1, int y1, short c) {
	VGA_box(x1, y1, x1+s_length+s_width, y1+s_width, c);
	VGA_box(x1, y1, x1+s_width, y1+s_length+s_width, c);
	VGA_box(x1+s_length, y1, x1+s_length+s_width, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1+s_length, y1+s_length, x1+s_length+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1, y1+s_length*2, x1+s_length, y1+s_length*2+s_width, c);
}

void VGA_nine(int x1, int y1, short c) {
	VGA_box(x1, y1, x1+s_length+s_width, y1+s_width, c);
	VGA_box(x1, y1, x1+s_width, y1+s_length+s_width, c);
	VGA_box(x1+s_length, y1, x1+s_length+s_width, y1+s_length+s_width, c);
	VGA_box(x1, y1+s_length, x1+s_length, y1+s_length+s_width, c);
	// VGA_box(x1, y1+s_length, x1+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1+s_length, y1+s_length, x1+s_length+s_width, y1+s_length*2+s_width, c);
	VGA_box(x1, y1+s_length*2, x1+s_length, y1+s_length*2+s_width, c);
}