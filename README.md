## Project Overview

This project is designed for a Linux environment using C programming language. The goal is to create a system that generates and visualizes data representing the time-varying graph of a random physical quantity. The generated graph is stored in a 1-bit depth BMP file ("black-and-white" image).

The program operates in two modes:
1. **Data Sender**: Generates random data.
2. **Data Receiver**: Visualizes the generated data.

The two modes communicate either through a file or a socket.

## Features
- Command-line arguments support for versioning (`--version`), help (`--help`), and operating modes (`-send`, `-receive`).
- Data generation using a 3-state random walk algorithm.
- Graphical representation of the data in a BMP file with 1-bit color depth.
- Process communication through file or socket.
- Signal handling for interruptions and user-defined events.

## Requirements
- Linux environment
- C compiler (e.g., GCC)
- Required libraries: `stdlib.h`, `stdio.h`, `unistd.h`, `signal.h`, `sys/socket.h`

## Compilation and Execution

### Compilation
Use `gcc` to compile the program:
```bash
gcc -o chart main.c -lpthread
