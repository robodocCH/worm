# RMD-Xx Drives

## Getting started using the command line

- Start Linux on x86
- [Connect motor1 with PC through PCAN](https://www.peak-system.com/PCAN-USB.199.0.html)
- Connect motor2 with CAN out connector of motor1
- Connect 120 Ohm resistor on both ends
- Install can-utils (sudo apt install can-utils), this allows to use CAN on the command line
- ip -a shows CAN interface -> can0
- sudo ip link set up can0 type can bitrate 1000000 -> initializes the interface 
- cansend can0 141#a500000500100000 -> drives forwards with moderate speed
- cansend can0 141#a500000500000000 -> drives back to initial position
- candump can0 -> use in a second terminal to show answers of CAN interface
- if you want to change the bitrate ..
  - sudo ip link set down can0
  - sudo ip link set up can0 type can bitrate 500000

## Use the EEROS program

- Get EEROS, build and install
- Build this project and run it
