

module DE1_SoC_Computer (
	////////////////////////////////////
	// FPGA Pins
	////////////////////////////////////

	// Clock pins
	CLOCK_50,
	CLOCK2_50,
	CLOCK3_50,
	CLOCK4_50,

	// ADC
	ADC_CS_N,
	ADC_DIN,
	ADC_DOUT,
	ADC_SCLK,

	// Audio
	AUD_ADCDAT,
	AUD_ADCLRCK,
	AUD_BCLK,
	AUD_DACDAT,
	AUD_DACLRCK,
	AUD_XCK,

	// SDRAM
	DRAM_ADDR,
	DRAM_BA,
	DRAM_CAS_N,
	DRAM_CKE,
	DRAM_CLK,
	DRAM_CS_N,
	DRAM_DQ,
	DRAM_LDQM,
	DRAM_RAS_N,
	DRAM_UDQM,
	DRAM_WE_N,

	// I2C Bus for Configuration of the Audio and Video-In Chips
	FPGA_I2C_SCLK,
	FPGA_I2C_SDAT,

	// 40-Pin Headers
	GPIO_0,
	GPIO_1,
	
	// Seven Segment Displays
	HEX0,
	HEX1,
	HEX2,
	HEX3,
	HEX4,
	HEX5,

	// IR
	IRDA_RXD,
	IRDA_TXD,

	// Pushbuttons
	KEY,

	// LEDs
	LEDR,

	// PS2 Ports
	PS2_CLK,
	PS2_DAT,
	
	PS2_CLK2,
	PS2_DAT2,

	// Slider Switches
	SW,

	// Video-In
	TD_CLK27,
	TD_DATA,
	TD_HS,
	TD_RESET_N,
	TD_VS,

	// VGA
	VGA_B,
	VGA_BLANK_N,
	VGA_CLK,
	VGA_G,
	VGA_HS,
	VGA_R,
	VGA_SYNC_N,
	VGA_VS,

	////////////////////////////////////
	// HPS Pins
	////////////////////////////////////
	
	// DDR3 SDRAM
	HPS_DDR3_ADDR,
	HPS_DDR3_BA,
	HPS_DDR3_CAS_N,
	HPS_DDR3_CKE,
	HPS_DDR3_CK_N,
	HPS_DDR3_CK_P,
	HPS_DDR3_CS_N,
	HPS_DDR3_DM,
	HPS_DDR3_DQ,
	HPS_DDR3_DQS_N,
	HPS_DDR3_DQS_P,
	HPS_DDR3_ODT,
	HPS_DDR3_RAS_N,
	HPS_DDR3_RESET_N,
	HPS_DDR3_RZQ,
	HPS_DDR3_WE_N,

	// Ethernet
	HPS_ENET_GTX_CLK,
	HPS_ENET_INT_N,
	HPS_ENET_MDC,
	HPS_ENET_MDIO,
	HPS_ENET_RX_CLK,
	HPS_ENET_RX_DATA,
	HPS_ENET_RX_DV,
	HPS_ENET_TX_DATA,
	HPS_ENET_TX_EN,

	// Flash
	HPS_FLASH_DATA,
	HPS_FLASH_DCLK,
	HPS_FLASH_NCSO,

	// Accelerometer
	HPS_GSENSOR_INT,
		
	// General Purpose I/O
	HPS_GPIO,
		
	// I2C
	HPS_I2C_CONTROL,
	HPS_I2C1_SCLK,
	HPS_I2C1_SDAT,
	HPS_I2C2_SCLK,
	HPS_I2C2_SDAT,

	// Pushbutton
	HPS_KEY,

	// LED
	HPS_LED,
		
	// SD Card
	HPS_SD_CLK,
	HPS_SD_CMD,
	HPS_SD_DATA,

	// SPI
	HPS_SPIM_CLK,
	HPS_SPIM_MISO,
	HPS_SPIM_MOSI,
	HPS_SPIM_SS,

	// UART
	HPS_UART_RX,
	HPS_UART_TX,

	// USB
	HPS_CONV_USB_N,
	HPS_USB_CLKOUT,
	HPS_USB_DATA,
	HPS_USB_DIR,
	HPS_USB_NXT,
	HPS_USB_STP
);

//=======================================================
//  PARAMETER declarations
//=======================================================


//=======================================================
//  PORT declarations
//=======================================================

////////////////////////////////////
// FPGA Pins
////////////////////////////////////

// Clock pins
input						CLOCK_50;
input						CLOCK2_50;
input						CLOCK3_50;
input						CLOCK4_50;

// ADC
inout						ADC_CS_N;
output					ADC_DIN;
input						ADC_DOUT;
output					ADC_SCLK;

// Audio
input						AUD_ADCDAT;
inout						AUD_ADCLRCK;
inout						AUD_BCLK;
output					AUD_DACDAT;
inout						AUD_DACLRCK;
output					AUD_XCK;

// SDRAM
output 		[12: 0]	DRAM_ADDR;
output		[ 1: 0]	DRAM_BA;
output					DRAM_CAS_N;
output					DRAM_CKE;
output					DRAM_CLK;
output					DRAM_CS_N;
inout			[15: 0]	DRAM_DQ;
output					DRAM_LDQM;
output					DRAM_RAS_N;
output					DRAM_UDQM;
output					DRAM_WE_N;

// I2C Bus for Configuration of the Audio and Video-In Chips
output					FPGA_I2C_SCLK;
inout						FPGA_I2C_SDAT;

// 40-pin headers
inout			[35: 0]	GPIO_0;
inout			[35: 0]	GPIO_1;

// Seven Segment Displays
output		[ 6: 0]	HEX0;
output		[ 6: 0]	HEX1;
output		[ 6: 0]	HEX2;
output		[ 6: 0]	HEX3;
output		[ 6: 0]	HEX4;
output		[ 6: 0]	HEX5;

// IR
input						IRDA_RXD;
output					IRDA_TXD;

// Pushbuttons
input			[ 3: 0]	KEY;

// LEDs
output		[ 9: 0]	LEDR;

// PS2 Ports
inout						PS2_CLK;
inout						PS2_DAT;

inout						PS2_CLK2;
inout						PS2_DAT2;

// Slider Switches
input			[ 9: 0]	SW;

// Video-In
input						TD_CLK27;
input			[ 7: 0]	TD_DATA;
input						TD_HS;
output					TD_RESET_N;
input						TD_VS;

// VGA
output		[ 7: 0]	VGA_B;
output					VGA_BLANK_N;
output					VGA_CLK;
output		[ 7: 0]	VGA_G;
output					VGA_HS;
output		[ 7: 0]	VGA_R;
output					VGA_SYNC_N;
output					VGA_VS;



////////////////////////////////////
// HPS Pins
////////////////////////////////////
	
// DDR3 SDRAM
output		[14: 0]	HPS_DDR3_ADDR;
output		[ 2: 0]  HPS_DDR3_BA;
output					HPS_DDR3_CAS_N;
output					HPS_DDR3_CKE;
output					HPS_DDR3_CK_N;
output					HPS_DDR3_CK_P;
output					HPS_DDR3_CS_N;
output		[ 3: 0]	HPS_DDR3_DM;
inout			[31: 0]	HPS_DDR3_DQ;
inout			[ 3: 0]	HPS_DDR3_DQS_N;
inout			[ 3: 0]	HPS_DDR3_DQS_P;
output					HPS_DDR3_ODT;
output					HPS_DDR3_RAS_N;
output					HPS_DDR3_RESET_N;
input						HPS_DDR3_RZQ;
output					HPS_DDR3_WE_N;

// Ethernet
output					HPS_ENET_GTX_CLK;
inout						HPS_ENET_INT_N;
output					HPS_ENET_MDC;
inout						HPS_ENET_MDIO;
input						HPS_ENET_RX_CLK;
input			[ 3: 0]	HPS_ENET_RX_DATA;
input						HPS_ENET_RX_DV;
output		[ 3: 0]	HPS_ENET_TX_DATA;
output					HPS_ENET_TX_EN;

// Flash
inout			[ 3: 0]	HPS_FLASH_DATA;
output					HPS_FLASH_DCLK;
output					HPS_FLASH_NCSO;

// Accelerometer
inout						HPS_GSENSOR_INT;

// General Purpose I/O
inout			[ 1: 0]	HPS_GPIO;

// I2C
inout						HPS_I2C_CONTROL;
inout						HPS_I2C1_SCLK;
inout						HPS_I2C1_SDAT;
inout						HPS_I2C2_SCLK;
inout						HPS_I2C2_SDAT;

// Pushbutton
inout						HPS_KEY;

// LED
inout						HPS_LED;

// SD Card
output					HPS_SD_CLK;
inout						HPS_SD_CMD;
inout			[ 3: 0]	HPS_SD_DATA;

// SPI
output					HPS_SPIM_CLK;
input						HPS_SPIM_MISO;
output					HPS_SPIM_MOSI;
inout						HPS_SPIM_SS;

// UART
input						HPS_UART_RX;
output					HPS_UART_TX;

// USB
inout						HPS_CONV_USB_N;
input						HPS_USB_CLKOUT;
inout			[ 7: 0]	HPS_USB_DATA;
input						HPS_USB_DIR;
input						HPS_USB_NXT;
output					HPS_USB_STP;

//=======================================================
//  REG/WIRE declarations
//=======================================================
wire [31:0] initial_value;
wire [31:0] initial_value_back;
wire [31:0] address;
wire  enable;
wire  reset;
wire  reset_back;
wire [1:0] out_answer_to_hps;
wire [31:0] result;


wire			[15: 0]	hex3_hex0;
//wire			[15: 0]	hex5_hex4;

//assign HEX0 = ~hex3_hex0[ 6: 0]; // hex3_hex0[ 6: 0]; 
//assign HEX1 = ~hex3_hex0[14: 8];
//assign HEX2 = ~hex3_hex0[22:16];
//assign HEX3 = ~hex3_hex0[30:24];
assign HEX4 = 7'b1111111;
assign HEX5 = 7'b1111111;
assign reset_back = reset;
assign initial_value_back = initial_value;

HexDigit Digit0(HEX0, hex3_hex0[3:0]);
HexDigit Digit1(HEX1, hex3_hex0[7:4]);
HexDigit Digit2(HEX2, hex3_hex0[11:8]);
HexDigit Digit3(HEX3, hex3_hex0[15:12]);

sudoku_solver dut_all(
    .clk(CLOCK_50),
    .rst(reset),
    .in_addr(address),
    .in_hps_valid(enable),
    .in_data(initial_value),
    .export_pos(address),
    .out_answer(out_answer_to_hps),
    .export_value(result)
);
//=======================================================
//  Structural coding
//=======================================================

Computer_System The_System (
	////////////////////////////////////
	// FPGA Side
	////////////////////////////////////
	.initial_value_export    (initial_value),
	.initial_value_back_export    (initial_value_back),
	.address_export			 (address),
	.enable_export				 (enable),
	.reset_export 			 		(reset),
	.reset_back_export			 (reset_back),
	.out_answer_export		 (out_answer_to_hps),
	.result_export				 (result),

	// Global signals
	.system_pll_ref_clk_clk					(CLOCK_50),
	.system_pll_ref_reset_reset			(1'b0),

	// AV Config
	.av_config_SCLK							(FPGA_I2C_SCLK),
	.av_config_SDAT							(FPGA_I2C_SDAT),

	// VGA Subsystem
	.vga_pll_ref_clk_clk 					(CLOCK2_50),
	.vga_pll_ref_reset_reset				(1'b0),
	.vga_CLK										(VGA_CLK),
	.vga_BLANK									(VGA_BLANK_N),
	.vga_SYNC									(VGA_SYNC_N),
	.vga_HS										(VGA_HS),
	.vga_VS										(VGA_VS),
	.vga_R										(VGA_R),
	.vga_G										(VGA_G),
	.vga_B										(VGA_B),
	
	// SDRAM
	.sdram_clk_clk								(DRAM_CLK),
   .sdram_addr									(DRAM_ADDR),
	.sdram_ba									(DRAM_BA),
	.sdram_cas_n								(DRAM_CAS_N),
	.sdram_cke									(DRAM_CKE),
	.sdram_cs_n									(DRAM_CS_N),
	.sdram_dq									(DRAM_DQ),
	.sdram_dqm									({DRAM_UDQM,DRAM_LDQM}),
	.sdram_ras_n								(DRAM_RAS_N),
	.sdram_we_n									(DRAM_WE_N),
	
	////////////////////////////////////
	// HPS Side
	////////////////////////////////////
	// DDR3 SDRAM
	.memory_mem_a			(HPS_DDR3_ADDR),
	.memory_mem_ba			(HPS_DDR3_BA),
	.memory_mem_ck			(HPS_DDR3_CK_P),
	.memory_mem_ck_n		(HPS_DDR3_CK_N),
	.memory_mem_cke		(HPS_DDR3_CKE),
	.memory_mem_cs_n		(HPS_DDR3_CS_N),
	.memory_mem_ras_n		(HPS_DDR3_RAS_N),
	.memory_mem_cas_n		(HPS_DDR3_CAS_N),
	.memory_mem_we_n		(HPS_DDR3_WE_N),
	.memory_mem_reset_n	(HPS_DDR3_RESET_N),
	.memory_mem_dq			(HPS_DDR3_DQ),
	.memory_mem_dqs		(HPS_DDR3_DQS_P),
	.memory_mem_dqs_n		(HPS_DDR3_DQS_N),
	.memory_mem_odt		(HPS_DDR3_ODT),
	.memory_mem_dm			(HPS_DDR3_DM),
	.memory_oct_rzqin		(HPS_DDR3_RZQ),
		  
	// Ethernet
	.hps_io_hps_io_gpio_inst_GPIO35	(HPS_ENET_INT_N),
	.hps_io_hps_io_emac1_inst_TX_CLK	(HPS_ENET_GTX_CLK),
	.hps_io_hps_io_emac1_inst_TXD0	(HPS_ENET_TX_DATA[0]),
	.hps_io_hps_io_emac1_inst_TXD1	(HPS_ENET_TX_DATA[1]),
	.hps_io_hps_io_emac1_inst_TXD2	(HPS_ENET_TX_DATA[2]),
	.hps_io_hps_io_emac1_inst_TXD3	(HPS_ENET_TX_DATA[3]),
	.hps_io_hps_io_emac1_inst_RXD0	(HPS_ENET_RX_DATA[0]),
	.hps_io_hps_io_emac1_inst_MDIO	(HPS_ENET_MDIO),
	.hps_io_hps_io_emac1_inst_MDC		(HPS_ENET_MDC),
	.hps_io_hps_io_emac1_inst_RX_CTL	(HPS_ENET_RX_DV),
	.hps_io_hps_io_emac1_inst_TX_CTL	(HPS_ENET_TX_EN),
	.hps_io_hps_io_emac1_inst_RX_CLK	(HPS_ENET_RX_CLK),
	.hps_io_hps_io_emac1_inst_RXD1	(HPS_ENET_RX_DATA[1]),
	.hps_io_hps_io_emac1_inst_RXD2	(HPS_ENET_RX_DATA[2]),
	.hps_io_hps_io_emac1_inst_RXD3	(HPS_ENET_RX_DATA[3]),

	// Flash
	.hps_io_hps_io_qspi_inst_IO0	(HPS_FLASH_DATA[0]),
	.hps_io_hps_io_qspi_inst_IO1	(HPS_FLASH_DATA[1]),
	.hps_io_hps_io_qspi_inst_IO2	(HPS_FLASH_DATA[2]),
	.hps_io_hps_io_qspi_inst_IO3	(HPS_FLASH_DATA[3]),
	.hps_io_hps_io_qspi_inst_SS0	(HPS_FLASH_NCSO),
	.hps_io_hps_io_qspi_inst_CLK	(HPS_FLASH_DCLK),

	// Accelerometer
	.hps_io_hps_io_gpio_inst_GPIO61	(HPS_GSENSOR_INT),

	//.adc_sclk                        (ADC_SCLK),
	//.adc_cs_n                        (ADC_CS_N),
	//.adc_dout                        (ADC_DOUT),
	//.adc_din                         (ADC_DIN),

	// General Purpose I/O
	.hps_io_hps_io_gpio_inst_GPIO40	(HPS_GPIO[0]),
	.hps_io_hps_io_gpio_inst_GPIO41	(HPS_GPIO[1]),

	// I2C
	.hps_io_hps_io_gpio_inst_GPIO48	(HPS_I2C_CONTROL),
	.hps_io_hps_io_i2c0_inst_SDA		(HPS_I2C1_SDAT),
	.hps_io_hps_io_i2c0_inst_SCL		(HPS_I2C1_SCLK),
	.hps_io_hps_io_i2c1_inst_SDA		(HPS_I2C2_SDAT),
	.hps_io_hps_io_i2c1_inst_SCL		(HPS_I2C2_SCLK),

	// Pushbutton
	.hps_io_hps_io_gpio_inst_GPIO54	(HPS_KEY),

	// LED
	.hps_io_hps_io_gpio_inst_GPIO53	(HPS_LED),

	// SD Card
	.hps_io_hps_io_sdio_inst_CMD	(HPS_SD_CMD),
	.hps_io_hps_io_sdio_inst_D0	(HPS_SD_DATA[0]),
	.hps_io_hps_io_sdio_inst_D1	(HPS_SD_DATA[1]),
	.hps_io_hps_io_sdio_inst_CLK	(HPS_SD_CLK),
	.hps_io_hps_io_sdio_inst_D2	(HPS_SD_DATA[2]),
	.hps_io_hps_io_sdio_inst_D3	(HPS_SD_DATA[3]),

	// SPI
	.hps_io_hps_io_spim1_inst_CLK		(HPS_SPIM_CLK),
	.hps_io_hps_io_spim1_inst_MOSI	(HPS_SPIM_MOSI),
	.hps_io_hps_io_spim1_inst_MISO	(HPS_SPIM_MISO),
	.hps_io_hps_io_spim1_inst_SS0		(HPS_SPIM_SS),

	// UART
	.hps_io_hps_io_uart0_inst_RX	(HPS_UART_RX),
	.hps_io_hps_io_uart0_inst_TX	(HPS_UART_TX),

	// USB
	.hps_io_hps_io_gpio_inst_GPIO09	(HPS_CONV_USB_N),
	.hps_io_hps_io_usb1_inst_D0		(HPS_USB_DATA[0]),
	.hps_io_hps_io_usb1_inst_D1		(HPS_USB_DATA[1]),
	.hps_io_hps_io_usb1_inst_D2		(HPS_USB_DATA[2]),
	.hps_io_hps_io_usb1_inst_D3		(HPS_USB_DATA[3]),
	.hps_io_hps_io_usb1_inst_D4		(HPS_USB_DATA[4]),
	.hps_io_hps_io_usb1_inst_D5		(HPS_USB_DATA[5]),
	.hps_io_hps_io_usb1_inst_D6		(HPS_USB_DATA[6]),
	.hps_io_hps_io_usb1_inst_D7		(HPS_USB_DATA[7]),
	.hps_io_hps_io_usb1_inst_CLK		(HPS_USB_CLKOUT),
	.hps_io_hps_io_usb1_inst_STP		(HPS_USB_STP),
	.hps_io_hps_io_usb1_inst_DIR		(HPS_USB_DIR),
	.hps_io_hps_io_usb1_inst_NXT		(HPS_USB_NXT)
);


endmodule
module sudoku_solver(
    input  clk,
    input  rst,
    input  [6:0]in_addr,
    input  in_hps_valid,
    input  [3:0]in_data,
    input  wire [6:0]export_pos,
    output wire[3:0]export_value,
    output wire[1:0]out_answer
    );  
// reg clk;
// reg valid;
// reg [3:0]in_data;
// reg [6:0]in_pos;
// reg in_valid;
// reg in_finish;
// reg in_success;
// reg in_hps_valid;
// reg [6:0]in_addr;
// reg in_hps_finish;
wire input_finish;
wire [2:0]updater_state;
wire [6:0]out_pos;
wire [3:0]out_value;
wire out_write_en;
wire out_finish;
wire [6:0]out_current_pos;
wire [3:0]update_x,update_y;
wire out_updater_update;
wire [6:0]out_updater_pos;
wire out_updater_finish;
wire out_updater_valid;
wire out_updater_rst;
wire out_reg_rst;
wire out_reg_valid;
wire [6:0]out_checker_pos;
wire [3:0]out_reg_value;
wire [6:0]out_reg_addr;
wire out_reg_export;
wire success;
// wire [1:0]out_answer;
wire one_valid;
assign input_finish = (in_addr==81)? 1:0;

valid_one_cycle valid_one_cycle(
    .clk(clk),
    .in_hps_addr(in_addr),             //  from   hps
    .in_hps_valid(in_hps_valid),                 //  from   hps
    .in_hps_rst(rst),                   //  from   hps
    .out_control_valid(one_valid)
);
checker checker_dut(
    .check(out_finish),.we(out_reg_valid),.clk(clk),.rst(out_reg_rst),
    .pos(out_checker_pos),.writepos(out_reg_addr),.export_pos(export_pos),
    .writedata(out_reg_value),
    .success(success),.valid(),
    .export_value(export_value),.export_en(out_reg_export)//need signals from hps
);
updater updater(
    .in_update(out_updater_update),
    .in_pos(out_updater_pos),
    .in_finish(out_updater_finish),
    .in_valid(out_updater_valid),
    .in_success(success),////connect to checker
    .clk(clk),
    .rst(out_updater_rst),
    .out_pos(out_pos),
    .out_value(out_value),
    .out_write_en(out_write_en),
    .out_finish(out_finish),//connect to checker's check
    .state(updater_state),
    .out_current_pos(out_current_pos)
);
coordinate_transform coordinate_transform(
    .in_pos(out_checker_pos),
    .out_x(update_x),//connect to check's pos
    .out_y(update_y)//connect to check's pos
);
control control(
    .out_answer(out_answer),
    .clk(clk),
    .in_hps_addr(in_addr),
    .in_hps_value(in_data),
    .in_hps_valid(one_valid),
    .in_hps_rst(rst),
    .in_hps_finish(input_finish),//todo
    .in_updater_pos(out_pos),
    .in_updater_value(out_value),
    .in_updater_write_en(out_write_en),
    .in_updater_current_pos(out_current_pos),
    .in_updater_state(updater_state),
    .in_checker_valid(1'b1),//connect to checker's valid 
    .out_checker_pos(out_checker_pos),
    .out_updater_update(out_updater_update),//fake
    .out_updater_pos(out_updater_pos),
    .out_updater_finish(out_updater_finish),
    .out_updater_valid(out_updater_valid), 
    .out_updater_rst(out_updater_rst),
    .out_reg_rst(out_reg_rst),//connect to sudoku_reg
    .out_reg_valid(out_reg_valid),//connect to sudoku_reg
    .out_reg_value(out_reg_value),//connect to sudoku_reg
    .out_reg_addr(out_reg_addr),//connect to sudoku_reg
    .out_reg_export(out_reg_export)//connect to sudoku_reg
);
endmodule
module valid_one_cycle(
    input  clk,
    input  [6:0]in_hps_addr,             //  from   hps
    input  in_hps_valid,                 //  from   hps
    input  in_hps_rst,                   //  from   hps
    output reg out_control_valid
    );  
    reg [6:0]addr_r,addr_w;
    reg first_valid;
    always@(*)begin
        addr_w = (in_hps_valid) ? in_hps_addr : addr_r;
    end
    always@(*)begin
        if(first_valid)begin
            out_control_valid = (in_hps_valid)? 1:0;
        end
        else begin
            if(addr_w==addr_r)begin
                out_control_valid = 0;
            end
            else begin
                out_control_valid = 1;
            end
        end
    end
    always@(posedge clk)begin
        if(in_hps_rst)begin
            addr_r<=0;
            first_valid<=1;
        end
        else begin
            addr_r<=addr_w;
            first_valid<=(in_hps_valid)? 0: first_valid;
        end
    end

endmodule
module control(
    input  clk,
    input  [6:0]in_hps_addr,             //  from   hps
    input  [3:0]in_hps_value,            //  from   hps
    input  in_hps_valid,                 //  from   hps
    input  in_hps_rst,                   //  from   hps
    input  in_hps_finish,                //  from   hps
    input  [6:0]in_updater_pos,          //  from   updater
    input  [3:0]in_updater_value,        //  from   updater
    input  in_updater_write_en,          //  from   updater
    input  [6:0]in_updater_current_pos,  //  from   updater
    input  [2:0]in_updater_state,        //  from   updater
    input  in_checker_valid,             //  from   checker         
    output reg[1:0]out_answer,           //  to     hps
    output reg [6:0]out_checker_pos,     //  to     checker
    output reg out_updater_update,       //  to     updater         
    output reg [6:0]out_updater_pos,     //  to     updater         
    output out_updater_finish,           //  to     updater         
    output reg out_updater_valid,        //  to     updater         
    output out_updater_rst,              //  to     updater         
    output out_reg_rst,                  //  to     sudoku register 
    output out_reg_valid,                //  to     sudoku register 
    output reg[3:0]out_reg_value,        //  to     sudoku register 
    output reg[6:0]out_reg_addr,         //  to     sudoku register 
    output out_reg_export);              //  to     sudoku register 

    parameter IDLE=0;
    parameter RST_OTHER=1;
    parameter UPDATER_START=5;
    parameter CALC=2;
    parameter FINISH_SUCCESS=3;
    parameter FINISH_FAILED=4;
    reg [2:0] state_r,state_w;
    assign out_updater_finish = in_hps_finish;
    assign out_updater_rst = (state_r==RST_OTHER)? 1:0;
    assign out_reg_rst = (state_r==RST_OTHER)? 1:0;
    assign out_reg_valid = in_hps_valid||in_updater_write_en;
    assign out_reg_export = (state_r==FINISH_SUCCESS)? 1:0;
    always@(*)begin
        
        if(state_r==FINISH_SUCCESS)begin
            out_answer = 2;
        end
        else if(state_r==FINISH_FAILED)begin
            out_answer = 1;
        end
        else begin
            out_answer = 0;
        end
    end
    always@(*)begin
        if(in_hps_valid)begin
            out_reg_addr = in_hps_addr;
            out_reg_value = in_hps_value;
        end
        else if(in_updater_write_en)begin
            out_reg_addr = in_updater_pos;
            out_reg_value = in_updater_value;
        end
        else begin
            out_reg_addr = 0;
            out_reg_value = 0;
        end
    end
    always@(*)begin
        if(in_hps_valid)begin
            if(in_hps_value==0)begin
                out_updater_valid = 1;
            end
            else begin
                out_updater_valid = 0;
            end
        end
        else begin
            out_updater_valid = 0;
        end
        out_updater_pos = in_hps_addr;
    end
    always@(*)begin
        if(state_r==UPDATER_START)begin
            out_updater_update = 1;
        end
        else begin
            out_updater_update = in_checker_valid;
        end
    end
    always@(*)begin
        case(state_r)
            IDLE:               state_w = RST_OTHER;
            RST_OTHER:          state_w = UPDATER_START;
            UPDATER_START:      state_w = CALC;
            CALC:begin               
                if(in_updater_state==5)begin
                    state_w = FINISH_SUCCESS;
                end
                else if (in_updater_state==6) begin
                    state_w = FINISH_FAILED;
                end
                else begin
                    state_w = CALC;
                end
            end
            FINISH_SUCCESS:     state_w = FINISH_SUCCESS;
            FINISH_FAILED:      state_w = FINISH_FAILED;
            default:            state_w = IDLE;
        endcase
    end
    always@(posedge clk)begin
        if(in_hps_rst)begin
            state_r<=IDLE;
            out_checker_pos<=0;
        end
        else begin
            state_r<=state_w;
            out_checker_pos<=(out_reg_valid)?out_reg_addr:out_checker_pos;
        end
    end
endmodule
module coordinate_transform(
    input [6:0] in_pos,
    output [3:0] out_x,
    output [3:0] out_y);
    assign out_y=in_pos/9;
    assign out_x=in_pos-out_y*9;
endmodule

module updater(in_update,in_pos,in_finish,in_valid,in_success,clk,rst,out_pos,out_value,out_write_en,out_finish,out_current_pos,state);
    input [6:0] in_pos;
    input       in_valid,clk,rst,in_update,in_finish,in_success;
    output[6:0] out_pos,out_current_pos;
    output[3:0] out_value;
    output      out_write_en,out_finish;
    output[2:0] state;

    localparam  IDLE   = 0;
    localparam  INPUT  = 1;
    localparam  WAIT   = 2;
    localparam  UPDATE_FAILED  = 3;
    localparam  UPDATE_SUCCESS = 4;
    localparam  FINISH_SUCCESS = 5;
    localparam  FINISH_FAILED  = 6;
    integer i;
    reg init_wait_r,init_wait_w;
    reg [3:0]   shift_reg_r[80:0];
    reg [3:0]   shift_reg_w[80:0];
    reg [6:0]   shift_pos_r[80:0];
    reg [6:0]   shift_pos_w[80:0];
    reg signed[7:0]   current_pos_r,current_pos_w;
    reg [2:0]   state_r,state_w;
    reg [6:0]   empty_num_r;
    reg         finish_r,finish_w;
    wire        no_solution;
    reg[3:0]    o_value;
    reg         o_write_en;
    wire        finish;
    reg[6:0]    o_pos;
    assign finish = (current_pos_r==-1)? 1:(shift_reg_r[current_pos_r]==9) ? 0 : 1;
    assign no_solution = (current_pos_r==0)&&(shift_reg_r[0]==9)&&(state_r==UPDATE_FAILED);
    assign out_pos = o_pos;
    assign out_finish = finish_r;
    assign out_current_pos = current_pos_r;
    assign out_write_en = o_write_en;
    assign out_value = o_value;
    assign state = state_r;
    //o_pos
        always@(*)begin
            if(state_r==UPDATE_FAILED)begin
                o_pos = shift_pos_r[current_pos_r];
            end
            else if(state_r==UPDATE_SUCCESS)begin
                o_pos = shift_pos_r[current_pos_w];
            end
            else begin
                o_pos = 0;
            end
             
        end
    //init_wait_w
        always@(*)begin
            if((state_r==UPDATE_FAILED)||(state_r==UPDATE_SUCCESS)||(init_wait_r==1))begin
                init_wait_w = 1;
            end
            else begin
                init_wait_w = 0;
            end
        end
    //o_value o_write_en
        always@(*)begin
            if(state_r==UPDATE_FAILED)begin
                o_write_en = 1;
                if(shift_reg_r[current_pos_r]==9)begin
                    o_value = 0;
                end
                else begin
                    o_value = shift_reg_w[current_pos_r];
                end
            end
            else if(state_r==UPDATE_SUCCESS)begin
                o_write_en = 1;
                o_value = 1;
            end
            else begin
                o_write_en = 0;
                o_value = 0;
            end
        end
    //finish_w
        always@(*)begin
            if(((state_r==UPDATE_FAILED)||(state_r==UPDATE_SUCCESS))&&((state_w==WAIT)||(state_w==FINISH_SUCCESS)||(state_w==FINISH_FAILED)))begin
                finish_w = 1;
            end
            else begin
                finish_w = 0;
            end
        end
    //state_w 
        always@(*)begin
            case(state_r)
                IDLE:    state_w = in_valid? INPUT:IDLE;
                INPUT:   state_w = in_finish? WAIT:INPUT;
                WAIT:begin    
                    if(in_update)begin
                        if(in_success||(init_wait_r==0))begin
                            if(current_pos_r==empty_num_r-1)begin
                                state_w = FINISH_SUCCESS;
                            end
                            else begin
                                state_w = UPDATE_SUCCESS;
                            end
                        end
                        else begin
                            state_w = UPDATE_FAILED;
                        end
                    end
                    else begin
                        state_w = WAIT;
                    end
                end
                UPDATE_SUCCESS:  begin
                    if(current_pos_r==empty_num_r-1)begin
                        state_w = FINISH_SUCCESS;
                    end
                    else begin
                        state_w = WAIT;
                    end
                end
                UPDATE_FAILED:  begin
                    if(no_solution)begin
                        state_w = FINISH_FAILED;
                    end
                    else begin
                        state_w = finish? WAIT:UPDATE_FAILED;
                    end
                end
                FINISH_SUCCESS:  state_w = FINISH_SUCCESS;
                FINISH_FAILED:  state_w = FINISH_FAILED;
                default: state_w = IDLE;
            endcase
        end
    //shift_reg_w current_pos_w 
        always@(*)begin
            case(state_r)
                INPUT:   begin
                    for (i=1;i<81;i=i+1)begin
                        shift_reg_w[i] = shift_reg_r[i];
                    end
                    shift_reg_w[0] = shift_reg_r[0];
                    current_pos_w = 0;
                end
                UPDATE_SUCCESS:  begin
                    for (i=0;i<81;i=i+1)begin
                        if(init_wait_r==1)begin
                            if((current_pos_r+1)==i)begin
                                shift_reg_w[i]=1;
                            end
                            else begin
                                shift_reg_w[i]=shift_reg_r[i];
                            end
                        end
                        else begin
                            if(0==i)begin
                                shift_reg_w[i]=1;
                            end
                            else begin
                                shift_reg_w[i]=shift_reg_r[i];
                            end
                        end
                    end
                    if(init_wait_r==0)begin
                        current_pos_w = current_pos_r;
                    end
                    else begin
                        current_pos_w = current_pos_r + 1;
                    end
                    
                end
                UPDATE_FAILED:  begin
                    for (i=0;i<81;i=i+1)begin
                        if(i==current_pos_r)begin
                            if(shift_reg_r[i]==9)begin
                                shift_reg_w[i] = 0;
                            end
                            else begin
                                shift_reg_w[i] = shift_reg_r[i] + 1;
                            end
                        end
                        else begin
                            shift_reg_w[i] = shift_reg_r[i];
                        end
                    end
                    if(shift_reg_r[current_pos_r]==9)begin
                        current_pos_w = current_pos_r - 1;
                    end
                    else begin
                        current_pos_w = current_pos_r;
                    end
                end
                default: begin
                    for (i=0;i<81;i=i+1)begin
                        shift_reg_w[i] = shift_reg_r[i];
                    end
                    current_pos_w = current_pos_r;
                end
            endcase
        end
    //shift_pos_w
        always@(*)begin
            if((state_r==INPUT)||(state_r==IDLE))begin
                for (i=1;i<81;i=i+1)begin
                    shift_pos_w[i] = in_valid ? shift_pos_r[i-1]:shift_pos_r[i];
                end
                shift_pos_w[0] = in_valid ? in_pos:shift_pos_r[0];
            end
            else begin
                for (i=0;i<81;i=i+1)begin
                    shift_pos_w[i] = shift_pos_r[i];
                end
            end
        end

    always@(posedge clk)begin
        if(rst)begin
            init_wait_r<=0;
            current_pos_r<=0;
            for (i=0;i<81;i=i+1)begin
                shift_reg_r[i]<=0;
            end
            for (i=0;i<81;i=i+1)begin
                shift_pos_r[i]<=0;
            end
            state_r<=0;
            empty_num_r<=0;
            finish_r<=0;
        end else begin
            init_wait_r<=init_wait_w;
            current_pos_r<=current_pos_w;
            for (i=0;i<81;i=i+1)begin
                shift_reg_r[i]<=shift_reg_w[i];
            end
            for (i=0;i<81;i=i+1)begin
                shift_pos_r[i]<=shift_pos_w[i];
            end
            empty_num_r<= (in_valid&&((state_r==IDLE)||(state_r==INPUT)))? empty_num_r+1:empty_num_r;
            finish_r<=finish_w;
            state_r<=state_w;
        end
    end   

endmodule

module checker(
    input check,we,clk,rst,export_en,
    input [6:0]pos,writepos,export_pos,
    input [3:0]writedata,
    output success,valid,
    output wire[3:0]export_value );

    //reg array
    reg [3:0] regarr[80:0]; 
    wire [3:0]row, col;
    reg [8:0]row_equal, col_equal, square_equal;

    integer i,j;

    //square
    reg [3:0] square; //{2'b(0-2),2'b(0-2)}  
    reg [6:0] square_pos;
    
    assign valid=1'b0;
    assign row=pos/9;
    assign col=pos-row*9;

    always @(*)begin

        //row 
        for(i=0;i<9;i=i+1)begin
            if(i==col)begin
                row_equal[i]=0;
            end else begin
                row_equal[i]=(regarr[row*9+i]==regarr[pos])?1'b1:1'b0;
            end
        end

        //col
        for(i=0;i<9;i=i+1)begin 
            if(i==row)begin
                col_equal[i]=0;
            end else begin
                col_equal[i]=(regarr[i*9+col]==regarr[pos])?1'b1:1'b0;
            end      
        end

        
        //6 additional comparator for deciding each square
        if(row<=2)begin
            if(col<=2)begin//square 1
                square = 4'b0000;
            end else if (col<=5)begin
                square = 4'b0001;
            end else if (col<=8)begin
                square = 4'b0010;
            end
        end else if (row<=5)begin
            if(col<=2)begin//square 1
                square = 4'b0100;
            end else if (col<=5)begin
                square = 4'b0101;
            end else if (col<=8)begin
                square = 4'b0110;
            end
        end else if (row<=8)begin
            if(col<=2)begin//square 1
                square = 4'b1000;
            end else if (col<=5)begin
                square = 4'b1001;
            end else if (col<=8)begin
                square = 4'b1010;
            end
        end

        for(  i=0;i<3;i=i+1)begin 
            for( j=0;j<3;j=j+1)begin
                square_pos = 9*(3*square[3:2]+i)+(3*square[1:0]+j);
                if(square_pos==pos)begin
                    square_equal[3*i+j]=0;//(i*3+j)
                end else begin
                    square_equal[3*i+j]=(regarr[square_pos]==regarr[pos])?1'b1:1'b0;
                end 
            end
        end

    end  
    assign export_value=(export_en)?regarr[export_pos]:4'b0;  

    assign success=(square_equal==0) & (row_equal==0) & (col_equal==0);

    //write data
    always @ (posedge clk) begin
        if(rst)begin
            for(i=0;i<81;i=i+1)begin
                regarr[i]<=0;
            end
        end else begin
            if (we) begin
                regarr[writepos] <= writedata;
            end
        end
    end

endmodule