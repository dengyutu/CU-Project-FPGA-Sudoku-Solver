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