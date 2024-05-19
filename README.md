# Project Overview: Sudoku Solver with FPGA Implementation

This collaborative project, completed as part of Cornell University's [ECE 5760 course](https://people.ece.cornell.edu/land/courses/ece5760/) under the guidance of Professor V. Hunter Adams, was a joint effort by Keyun Gao, Yen-An Lu, and Dengyu (Spud) Tu. The team successfully designed and implemented a FPGA accelerated Sudoku Puzzle Solver.

This project aims to solve Sudoku puzzles using Field-Programmable Gate Array (FPGA) technology, providing users with an elegant interface for interacting with the Sudoku grid. Sudoku puzzles are not only entertaining but also offer cognitive benefits such as problem-solving and critical thinking. Leveraging the rapid processing capabilities of FPGA, we have created an engaging platform for Sudoku enthusiasts to enjoy and improve their skills.

**Technical Details:**
- **Hardware Component:** [Intel DE1-SoC](https://www.intel.com/content/www/us/en/partner/showcase/offering/a5b3b0000004cbaAAA/de1soc-board.html), VGA display, Micro SD card, Keyboard, Mouse.
- **Programming Language:** Verilog, C

## Project Webpage
For comprehensive information on the project, including design details, implementation techniques, and results, visit the [Project Webpage](https://dengyutu.github.io/CU-Project-FPGA-Sudoku-Solver/).

## Video Demonstration
A visual representation of the project's capabilities is showcased in the [Video Demonstration](https://www.youtube.com/watch?v=6yiWjeeNqF0).

## Project Components
The project repository includes:
- Several C files to receive user input, draw on VGA display, control the game and send data to VGA.
- Verilog files to implement the hardware solver on Sudoku Puzzle.
- DE1 file that configures the FPGA and connects with the VGA display.
  
## Installation Procedure
1. **Connect Components:** Connect the SoC platform with the VGA display (**Caution:** our VGA pixel clock runs around 25 MHz. If your VGA clock runs at differnt frequency, you might need to configure the DE1_SoC_Computer file yourself.)
2. **Load Linux Image:** Load a Linux image onto the Micro SD card. (Windows User: use software like Win32 Disk Imager. Mac User: use software like [ApplePiBaker](https://www.tweaking4all.com/software/macosx-software/macosx-apple-pi-baker/).)
4. **Clone Repository:** Clone the repository to your local machine.
5. **Unzip Folder:** Unzip the FPGA+VGA_Final.zip file inside the DE1 folder.
6. **Compile Verilog Code:** Compile the DE1_SoC_Computer.qar file which is inside the computer_640_16bit_video/verilog folder using your preferred FPGA development environment. (We used [Quartus](https://www.intel.com/content/www/us/en/products/details/fpga/development-tools/quartus-prime.html).)
7. **Upload Bitstream:** Upload the generated bitstream to your FPGA board.
8. **Upload C file:** Upload the Final.c file which is inside the C folder to your Micro SD card.
9. **Compile C Code:** Compile the Final.c file with command `Final.c -o Final -O2 -lm` .
10. **Run Application:** Run the application on Linux with terminal command `./Final /dev/input/eventx (where x depends on the event number of the keyboard in Linux)`.

## Copyright and License
This project is licensed under the [MIT](https://github.com/StartBootstrap/startbootstrap-clean-blog/blob/master/LICENSE) License.
