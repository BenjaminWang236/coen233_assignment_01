# Programming Assignment #1: Custom Protocol on top of UDP for Client-Server communication 

COEN 233 Introduction to Computer Networks (SCU W2022)

Author: Benjamin Wang (1179478)

---
## About this Project
Client using customized protocol on top of UDP protocol for sending information to the server

Custom Protocol are defined, commented, and implemented in `customProtocol.h` header file and `customProtocol.c` soure code file

Client `myclient` send custom `data_packet` to the Server `myserver` and wait for a response with `ACK_Timer` set to `3 seconds`. Client sends packets in a sequence of 5 segments.

Server `myserver` reads in the `data_packet` from Client `myclient` and check if the packet contains correct information as defined by the Programming Assignment 01 instruction.

If correct, Server `myserver` responds with an ACK as a custom `ack_packet` with the received segment number and other relevant information

If incorrect, Server `myserver` responds with an REJ as a custom 'reject_packet` with one of the following error cases:

1. Out of Sequence segment number
2. Length Mismatch
3. End of Packet Missing
4. Duplicate Packet (checked by comparing sequence number as specified in instruction)

---
## Project Structure
Main program files and executables are in the project root folder, containing this `readme`, `myclient`, `myserver`, `customProtocol`, `testing`, and `Makefile` source codes and compiled executables

`examples` folder has the UDP-server-client tutorial codes I based my programming assignment off of

`input_files` contain the input files used for testing myclient and myserver's verification database

`output_files` contain the output files generated from myclient and myserver executions, their converted PDF version, and finally screenshots within a sub-folder `screenshots`

`instruction` contain the instruction docx and PDFs for COEN 233 Programming Assignment 02

---
## How to Compile the Code
Use the provided Makefile to compile the code, preferrably after clearing compiled executables first.
```C
make
```
or better yet:
```C
make clean && make
```
Note that verbose debugging mode can be activated by not commenting out line 16 of customProtocol.h: `// #define DEBUGGING 0` before compilation

Individual compilation options are also available within the `Makefile`

## How to Run the Code
Open two terminals on the same computer. By default Port number `8080` and host "`localhost`" are used. Navigate to project's main folder first!

Teste compilation and running successfully on Santa Clara university's ECC (previously Data Center) Linux Computer

---
### Run Server
```C
./myserver
```
Alternatively myserver also supports specifying port number:
```C
./myserver 8080
```
Note that since this simple implementation of myserver runs the while-loop indefinitely, the user will have to quit myserver by keyboard interrupt such as
```
Ctrl + C
```
This also means you can't redirect the command line outputs to an output-file, so that have to be done manually

---
### Run Client
Must give an input file as argument:
```C
./myclient ./input_files/test_input.txt
```
To save output to an output file, preferrably in the `output_files` folder,
```C
./myclient ./input_files/test_input.txt > ./output_files/test_output_client_noServer.txt
```
The `test_input.txt` file in the `input_files` folder contains two sequences of data-packets, one with all 5 correct packets, and another one with 1 correct packet followed by 4 erroneous data packets showcasing all 4 error cases as specified by the procedure instruction given 

---
