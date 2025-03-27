# wblock
 Simple command-line C program for Windows that blocks connections to a given hostname by editing the hosts file. 
 
 Great for wasting less time on the Internet.

## Installation
Clone the repo and compile `main.c`. No external dependencies are needed.

## Usage
The program must be run with administrator privileges. The simplest way to do that is to run a command processor (`cmd` or Powershell) as administrator and running `wblock` from it.

The program interprets all command-line arguments it receives as domains to be blocked. Therefore, run

`wblock website1.com website2.com website3.com`

Run `wblock -help` (`-h` or `-?`) for detailed usage information.

## Implementation details
Most of the code in this project is for checking through Windows API whether elevated privileges are given to the program.

As all this program does is edit the hosts file, websites can be unblocked by undoing the changes manually.
