///////////////////////////////////////
/// 640x480 version! 16-bit color
/// This code will segfault the original
/// DE1 computer
/// compile with
/// gcc Final.c -o Final -O2 -lm
/// run it will ./Final /dev/input/eventx (where x depends on the event number of the keyboard in Linux)
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
#include <linux/input.h>

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
void VGA_four(int, int, short);
void VGA_five(int, int, short);
void VGA_six(int, int, short);
void VGA_seven(int, int, short);
void VGA_eight(int, int, short);
void VGA_nine(int, int, short);
int Correct_xpos(int);
int Correct_ypos(int);
int Index(int, int);
int Index_to_x(int);
int Index_to_y(int);
int isRepetitionInRow(int);
int isRepetitionInCol(int);
int isRepetitionInSquare(int);
// int Index_to_col(int);
// int Index_to_row(int);
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

//////ptr
#define addr_address_ptr 0x10
#define addr_enable_ptr 0x20
#define addr_reset_ptr 0x30
#define addr_result_ptr 0x40
#define addr_out_answer_ptr 0x50
#define addr_initial_value_ptr 0x60
#define addr_reset_back_ptr 0x70
#define addr_initial_value_back_ptr 0x80

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

volatile unsigned int * initial_value_ptr = NULL ;
volatile unsigned int * address_ptr = NULL ;
volatile unsigned int * reset_ptr = NULL ;
volatile unsigned int * enable_ptr = NULL ;
volatile unsigned int * result_ptr = NULL ;
volatile unsigned int * out_answer_ptr = NULL ;
volatile unsigned int * reset_back_ptr = NULL ;
volatile unsigned int * initial_value_back_ptr = NULL ;

// /dev/mem file id
int fd;

// measure time
struct timeval t1, t2;
double elapsedTime;

//suduku 
int cell = 9; //number of cells in the grid
int spacing = 35; //spacing between cells
int x_start = 162; //(640-spacing*cell)/2;
int y_start = 82; //(480-spacing*cell)/2;

// size of the number in pixel
int s_length = 8; //length of the number in pixel value
int s_width = 2; // stroke of the number in pixel value

struct KeyValuePair{
	int key;
	int value;
};

struct puzzle_info{
	int value;
	int init;
    int user;
};

struct puzzle_info s_data[81];

int main(int argc, char *argv[])
{
	//mapping between the keyboard value and number
  	struct KeyValuePair mapping[] = {{71,7},{72,8},{73,9},{75,4},{76,5},{77,6},{79,1},{80,2},{81,3}};
	int d;
	for(d = 0; d<81; d++){
		s_data[d].value = 0;
		s_data[d].init = 0;
        s_data[d].user = 0;
	}
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


	initial_value_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_initial_value_ptr);
	address_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_address_ptr);
	enable_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_enable_ptr);
	reset_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_reset_ptr);
	result_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_result_ptr);
	out_answer_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_out_answer_ptr);
	reset_back_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_reset_back_ptr);
	initial_value_back_ptr = (unsigned int *)(h2p_lw_virtual_base+addr_initial_value_back_ptr);

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

	//////////keyboard reading//////////
	struct input_event ev[64];
  	int fd2, rd, value, size = sizeof (struct input_event);
	char *device = NULL;

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
	if ((fd2 = open (device, O_RDONLY)) == -1)
		printf ("%s is not a vaild device.\n", device);
	
	int flags2 = fcntl(fd2, F_GETFL, 0);
	fcntl(fd2, F_SETFL, flags2 | O_NONBLOCK);
	//////////keyboard reading//////////


	int i;
	int mouse_x = 320;
	int mouse_y = 240;
	int mouse_x_old = mouse_x;
	int mouse_y_old = mouse_y;
	int mouse_x_click = 0;
	int mouse_y_click = 0;
	int mouse_x_eraze = 0;
	int mouse_y_eraze = 0;
	int select = 0;
	int eraze = 0;
	int initialize = 1;
	int index;
	int quit = 0;
	int user_mode = 0;
	int FPGA_mode = 0;

	char text_init[40] = "Initialization Done!\0";
	char text_user[40] = "User mode\0";
	char text_FPGA[40] = "FPGA mode\0";
	char text_restart[40] = "Restart!\0";
	char text_quit[40] = "Quit!\0";
	char time_string[20];

	//clear the screen
	VGA_box (0, 0, 639, 479, white);
	// clear the text
	VGA_text_clear();

	VGA_text(4, 55, text_init);
	VGA_text(29, 55, text_user);
	VGA_text(42, 55, text_FPGA);
	VGA_text(55, 55, text_restart);
	VGA_text(68, 55, text_quit);

    while(!quit)
    {
		if(initialize == 1 || user_mode == 1){
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
            // printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
			if(left==1){
				select = 1;
				mouse_x_click = mouse_x;
				mouse_y_click = mouse_y;
			}
			if(right==2){
				eraze = 1;
				mouse_x_eraze = mouse_x;
				mouse_y_eraze = mouse_y;
			}
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
		VGA_disc(mouse_x_old,mouse_y_old,5,white);
		VGA_disc(mouse_x,mouse_y,5,red);
		if(select==1){
			if( mouse_x_click >= 30 && mouse_x_click <= 200 && 430 <= mouse_y_click && mouse_y_click <= 455){
				initialize = 0;
				select = 0;
                user_mode = 1;				
			}
			else if( mouse_x_click >= 225 && mouse_x_click <= 308 && 430 <= mouse_y_click && mouse_y_click <= 455){
				user_mode = 1;
				FPGA_mode = 0;
				select = 0;					
			}
			else if( mouse_x_click >= 332 && mouse_x_click <= 410 && 430 <= mouse_y_click && mouse_y_click <= 455){
				user_mode = 0;
				FPGA_mode = 1;
				select = 0;					
			}
			else if( mouse_x_click >= 435 && mouse_x_click <= 510 && 430 <= mouse_y_click && mouse_y_click <= 455){
				int a;
				for(a = 0; a < 81; a++){
					s_data[a].value = 0;
					s_data[a].init = 0;
                    s_data[a].user = 0;
				}
				VGA_box (0, 0, 639, 479, white);
				select = 0;
				initialize = 1;
				sprintf(time_string, "                  ");
				VGA_text(2,1,time_string);			
			}
			else if( mouse_x_click >= 534 && mouse_x_click <= 590 && 430 <= mouse_y_click && mouse_y_click <= 455){
				quit = 1;
				VGA_text_clear();
			}
			else
			{
			//read the keyboard
			rd = read (fd2, ev, size * 64) ;
	
			value = ev[0].value;
	
			if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
				int j;
				for(j = 0; j < 9; j++){
					if(mapping[j].key == ev[1].code){
						// printf("%d\n",mapping[j].value);
						int num = mapping[j].value;
						if (num == 1){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 2){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 3){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 4){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 5){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 6){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 7){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 8){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
						else if (num == 9){
							index = Index(mouse_x_click,mouse_y_click);
							s_data[index].value = num;
							s_data[index].init = initialize;
                            s_data[index].user = user_mode;
						}
					}
				select = 0;
				}
				}
			}
		}

		if(eraze==1){
			VGA_box(Correct_xpos(mouse_x_eraze),Correct_ypos(mouse_y_eraze),Correct_xpos(mouse_x_eraze)+20,Correct_ypos(mouse_y_eraze)+20,white);
			index =  Index(mouse_x_eraze,mouse_y_eraze);
			s_data[index].value = 0;
			s_data[index].init = 0;
            s_data[index].user = 0;
			eraze = 0;
		}

		for(i=0;i<10;i++){
		if(i%3==0){
		VGA_Vline(x_start+i*spacing,y_start,y_start+spacing*cell,black); // x1,y1.y2
		VGA_Hline(x_start,y_start+i*spacing,x_start+spacing*cell,black); // x1.y1.x2
		}
		else{
		VGA_Vline(x_start+i*spacing,y_start,y_start+spacing*cell,gray); // x1,y1.y2
		VGA_Hline(x_start,y_start+i*spacing,x_start+spacing*cell,gray); // x1.y1.x2
			}
		}

		VGA_box(30,430,200,455,gray);
		VGA_box(225,430,308,455,gray);
		VGA_box(332,430,410,455,gray);
		VGA_box(435,430,510,455,gray);
		VGA_box(534,430,590,455,gray);
		VGA_box(0,0,150,25,gray);

		mouse_x_old = mouse_x;
		mouse_y_old = mouse_y;
		usleep(33000);
		}


		else if(initialize == 0 && FPGA_mode == 1){
			printf("Initialization done!\n");

			int row;
			for(row = 0;row<9;row++){
				if(isRepetitionInRow(row)!=0){
					VGA_rect(Index_to_x(0)-15,Index_to_y(row*9)-10,Index_to_x(0)+spacing*cell-10,Index_to_y(row*9)+30,red);
				}
			}

			int col;
			for(col = 0;col<9;col++){
				if(isRepetitionInCol(col)!=0){
					VGA_rect(Index_to_x(col)-15,Index_to_y(0)-10,Index_to_x(col)+spacing-10,Index_to_y(0)+spacing*cell-5,red);
				}
			}
			int square;
			for(square = 0; square<9;square++){
				if(isRepetitionInSquare(square)!=0){
					printf("%d\n",square);
					printf("%d\n",(Index_to_x((square /3)*3)-10));
					// VGA_rect(Index_to_x((square % 3)*3)-15,Index_to_x((square / 3)*3)-10,Index_to_x((square % 3)*3)+spacing*3-10,Index_to_x((square / 3)*3)+spacing*3-5,red);
					 VGA_rect(Index_to_x((square % 3)*3)-15,Index_to_x((square / 3)*3)-95,Index_to_x((square % 3)*3)+spacing*3-10,Index_to_x((square / 3)*3)+spacing*3-90,red);
				}
			}

			int s;
			int init_fail = 0;

			for(s = 0; s<9;s++){
				init_fail += isRepetitionInRow(s);
				init_fail += isRepetitionInCol(s);
				init_fail += isRepetitionInSquare(s);
			}
			printf("The initialization has %d errors.\n", init_fail);

			if(init_fail!=0){
				printf("Initialization Fails! Your initialization is invalid!!!\n");
				initialize = 1;
			}
			else{

			int l;
			*enable_ptr = 0;
			*reset_ptr = 1;
			usleep(1000);
			*reset_ptr = 0;
			//start timer
			gettimeofday(&t1, NULL);
			*initial_value_ptr = s_data[0].value;
			*address_ptr = 0;
			*enable_ptr = 1;
			for(l = 1; l<81; l++){
				*initial_value_ptr = s_data[l].value;
				*address_ptr = l;
				// printf("%d  ",s_data[l].value);
			}
			*enable_ptr = 0;
			*address_ptr = 81;
			while(*out_answer_ptr==0){
				// printf("%d",*out_answer_ptr);
			}
			gettimeofday(&t2, NULL);
			elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000000.0;      // sec to us
			elapsedTime += (t2.tv_usec - t1.tv_usec) ;   // us 
			sprintf(time_string, "T = %6.0f uSec", elapsedTime);
			VGA_text(2,1,time_string);
			if(*out_answer_ptr==2){
			for(l = 0; l<81; l++){
			*address_ptr = l;
			s_data[l].value = *result_ptr; 
				}
			}
			else if(*out_answer_ptr==1){
				printf("I cannot solve this problem!!! So sad!\n");
			}

			//check the result of FPGA
			for(row = 0;row<9;row++){
				if(isRepetitionInRow(row)!=0){
					VGA_rect(Index_to_x(0)-15,Index_to_y(row*9)-10,Index_to_x(0)+spacing*cell-10,Index_to_y(row*9)+30,red);
				}
			}

			for(col = 0;col<9;col++){
				if(isRepetitionInCol(col)!=0){
					VGA_rect(Index_to_x(col)-15,Index_to_y(0)-10,Index_to_x(col)+spacing-10,Index_to_y(0)+spacing*cell-5,red);
				}
			}
			for(square = 0; square<9;square++){
				if(isRepetitionInSquare(square)!=0){
					VGA_rect(Index_to_x((square % 3)*3)-15,Index_to_x((square / 3)*3)-95,Index_to_x((square % 3)*3)+spacing*3-10,Index_to_x((square / 3)*3)+spacing*3-90,red);
				}
			}

			int final_result_fail = 0;
			for(s = 0; s<9;s++){
				final_result_fail += isRepetitionInRow(s);
				final_result_fail += isRepetitionInCol(s);
				final_result_fail += isRepetitionInSquare(s);
			}
			printf("The result has %d errors.\n", final_result_fail);

			// if(final_result_fail!=0){
			// 	printf("The result from FPGA is wrong!\n");
			// }
			// else
			// 	{printf("The result from FPGA is correct!\n");}

			}
			user_mode = 1;
			FPGA_mode = 0;			
		}
		

		int k = 0;
		short color;
		for(k = 0; k<81; k++){
			if(s_data[k].init == 1){
				color = black;
			}
            else if(s_data[k].user == 1){
				color = dark_green;
			}
			else
				color = cyan;

			if(s_data[k].value==1){
				VGA_one(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==2){
				VGA_two(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==3){
				VGA_three(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==4){
				VGA_four(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==5){
				VGA_five(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==6){
				VGA_six(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==7){
				VGA_seven(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==8){
				VGA_eight(Index_to_x(k),Index_to_y(k),color);
				}
			else if(s_data[k].value==9){
				VGA_nine(Index_to_x(k),Index_to_y(k),color);
				}
		}
		
	}

	printf("Quit the game\n");
	//clear the screen
	VGA_box (0, 0, 639, 479, black);
	// VGA_box (0, 0, 639, 479, white);
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

void VGA_four(int x1, int y1, short c) {
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

int Correct_xpos(int x){
	int n = (x - x_start) / spacing;
	x = x_start + n * spacing + 12;
	return x;
}

int Correct_ypos(int y){
	int n = (y - y_start) / spacing;
	y = y_start + n * spacing + 8;
	return y;
}

int Index(int x, int y){
	int x_i = (x - x_start) / spacing;
	int y_i = (y - y_start) / spacing;
	return x_i + y_i*9;
}

int Index_to_x(int i){
	int n = i % 9;
	int x = x_start + n * spacing + 12;
	return x;
}

int Index_to_y(int i){
	int n = i / 9;
	int y = y_start + n * spacing + 8;
	return y;
}

// int Index_to_col(int i){
// 	int c = i / 9;
// 	return c;
// }

// int Index_to_row(int i){
// 	int r = i % 9;
// 	return r;
// }

int isRepetitionInRow(int row){ //check whether num is present in row or not
   int i;
   int j;
   int num;
   for (i = 0; i < 9; i++){
	   num = s_data[(row*9)+i].value;
	   if(num==0){
		   continue;
		   }
		else{
			for(j = i+1; j < 9; j++){
			   if(num==s_data[(row*9)+j].value){
				   return 1;
			   }
			   else
			   		{
					continue;
				   	}
		}
	   }
   }
   return 0;
}

int isRepetitionInCol(int col){ //check whether num is present in row or not
   int i;
   int j;
   int num;
   for (i = 0; i < 9; i++){
	   num = s_data[(i*9+col)].value;
	// 	printf("%d",i);
	// 	printf("%d",num);
	   if(num==0){
		   continue;
		   }
		else{
			for(j = i+1; j < 9; j++){
				// printf("%d",i);
				// printf("%d",j);
				// printf("%d",num);
				// printf("%d",s_data[(j*9+col)].value);
			   if(num==s_data[(j*9+col)].value){
				   return 1;
			   }
			   else
			   		{
					continue;
				   	}
		}
	   }
   }
   return 0;
}

int isRepetitionInSquare(int square){ //check whether num is present in row or not
   int i;
   int j;
   int num;
   int l,m;


   int row = (square / 3) * 3;
   int col = (square % 3) * 3;
	
	for(l=0;l<3;l++){
		for(m=0;m<3;m++){
			num = s_data[(row+l)*9+col+m].value;
			if(num==0){
				continue;
			}
			else{
				for(i = 0;i<3;i++){
					for(j = 0;j<3;j++){
						if((num == s_data[9*(row+i)+col+j].value)&&((i!=l)||(m!=j))){
							return 1;
						}
						else{
							continue;
						}
					}
		        }           
        	}
		}
	}

   return 0;
}