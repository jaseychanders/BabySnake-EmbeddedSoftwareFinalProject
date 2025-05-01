
# Principles of Embedded Software Final Project
# Baby Snake
Jasey Chanders - April 30th, 2025

## Overview
Creates a playable game similar to the classic game snake that can be run on an STM Nucleo-64 board with a small external screen and 4 external buttons. This code is written in bare-metal C and does not use a HAL. Communication with the screen is done over SPI, buttons trigger game events via interrupts, game play is driven by a table-driven state machine and basic automated tests are provided. 

![snake image](Documents/Snake%20Image.png)

## Demo 
![Demo Video](https://youtu.be/bqw7dEJXwAk)


## Setup

### Hardware used:
- STM32-Nucleo-64 - STM32F091RCT6 Development Eval board: https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-F091RC/4989197
- SparkFun Micro-OLED display: https://www.sparkfun.com/sparkfun-micro-oled-breakout.html
- 4 standard buttons
- 7 male-male jumper wires
- 9 female-male jumper wires

### Wiring:
![Wiring Diagram](Documents/Wiring%20Diagram.png)

### Running the code
Use the STM32CubeIDE to build and download the code to the Nucleo board. The game should begin automatically 
Log messages can be seen in over UART using 
- 9600 Baud
- 8 Data bits
- No Parity
- 1 Stop bit

## Testing
main.c will automatically run tests on the data structure and some minimal tests on the state machine. Other tests must be performed manually following the testing procedure outlined in Documents/Testing Procedure.pdf

## Code Layout
- Src/
  - delay.c/h provides a blocking delay for x milliseconds
  - display.c/h handles all communication with the external screen and stores the image currently displayed
  - log.h allows for logging in debug mode but not in release
  - main.c runs basic tests and then starts the game and provides update ticks to the state machine
  - snake_list.c/h holds the data structure that stores the current location of the snake
  - snake_list_tests.c/h provides automated tests for the snake_list data structure
  - state_machine.c/h holds the game state machine
  - state_machine./h provides simple automated tests for the state_machine
  - switches.c/h handles interfacing with the external buttons
  - syscalls.c and sysmem.c are unaltered stm files
- Inc/ contains stm32 libraries for access to hardware registers and utility functions
- Startup/ contains the start up instructions
- Documents/ Contains extra information about the project

### State Machine Diagram
![State Machine Diagram](Documents/State%20Diagram.png)

## Reflection
### What was I able to achieve from the items outlined in the proposal
- SPI communications with an external display
- Interrupt communication with 4 external buttons
- A table driven state machine
- A playable game
- Communication with many GPIO Pins

### Feature scope changes from original proposal
- I did not end up needing any timers as there is not critical timing in the game and almost everything is driven by interrupts
- I was not able to create larger integration tests due to the program heavily relying on hardware
- The proposal did not specify the type of game to be played, in implementation I choose to build snake

### What did you learn from the project
- How to interface with serial communication protocols, specifically SPI
- How to debug serial communication protocols using a logic analyzer
- How to use a single interrupt to monitor multiple buttons
- How to create a table driven state machine
- How to reconstruct parts of an existing Arduino library for different hardware
- How to interact with an OLED screen
- How to select available GPIO pins and set them for different functions
- How to find information buried deep in documentation files

### What could you have done differently
- Started out by getting SPI working on a simpler device instead of complicating it with the OLED. I ended up getting it working on an accelerometer after many hours of working with the OLED and that cleared the way for me to get the OLED working
- Implement the data structure that store the snake in such a way that it would allow for the easy addition of collision detections
- Created the project from scratch to learn how to set up the clocks

## Resources
- Embedded Systems Fundamentals by Alexander G. Dean https://www.barnesandnoble.com/w/embedded-systems-fundamentals-with-arm-cortex-m-based-microcontrollers-alexander-g-dean/1139147863
- SparkFun Electronics Micro-OLED hookup guide https://www.sparkfun.com/sparkfun-micro-oled-breakout.html
- SparkFun Electronics SFE_MicroOLED.h Arduino library https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/tree/V_1.0.0
- STM32 Nucleo-64 User Manual https://www.st.com/content/ccc/resource/technical/document/user_manual/98/2e/fa/4b/e0/82/43/b7/DM00105823.pdf/files/DM00105823.pdf/jcr:content/translations/en.DM00105823.pdf
- STM32F091xC data sheet https://www.st.com/content/ccc/resource/technical/document/datasheet/95/3c/2e/5b/21/09/45/a6/DM00115237.pdf/files/DM00115237.pdf/jcr:content/translations/en.DM00115237.pdf
- STM32F0x1 Reference Manual https://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf
- https://blog.embeddedexpert.io/?p=1650
- AD3 Logic analyzer for SPI debugging
- ChatGPT for syntax and SPI debugging
