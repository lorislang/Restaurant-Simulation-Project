# Restaurant Simulation Project

## Description
This project simulates a simple restaurant system with guests ordering meals and cooks preparing them. The program uses multi-threading to handle multiple guests and cooks simultaneously. The implementation involves:
- `restaurant.c`: The main C program that initializes the simulation.
- `queue.h`: Implementation of a thread-safe queue using a singly linked list.
- `makefile`: A build script to compile the project.

## Files Overview

### 1. `restaurant.c`
This is the main program that simulates a restaurant where:
- Guests place meal orders and wait for them to be prepared.
- Cooks take orders from a queue, prepare meals, and place them on a counter.
- Guests pick up their meals once they are ready.
- The process is managed using multi-threading with `pthread`.

### 2.`queue.h`
These files implement a simple queue using a singly linked list. The queue stores integer values representing meal orders. The queue has the following operations:
- `myqueue_init(myqueue* q)`: Initializes the queue.
- `myqueue_is_empty(myqueue* q)`: Checks if the queue is empty.
- `myqueue_push(myqueue* q, int value)`: Adds a value (meal order) to the queue.
- `myqueue_pop(myqueue* q)`: Removes and returns the front value from the queue.

## How It Works
1. The program initializes a queue for meal orders and a mutex for synchronization.
2. A specified number of guest threads are created, each placing an order in the queue and waiting for their meal.
3. A predefined number of cook threads pick up orders from the queue, simulate meal preparation, and place the completed orders on the counter.
4. Guests check if their meal is ready and pick it up once available.
5. The program waits for all threads to complete before exiting.

## Compilation & Execution
To compile the program, use the provided `makefile`:
```sh
make
```
This compiles the `restaurant.c` program using the provided `CFLAGS` and generates an executable named `restaurant`.

To run the program, use:
```sh
./restaurant <number_of_guests> <number_of_cooks>
```
Replace `<number_of_guests>` with the desired number of guests and `<number_of_cooks>` with the number of cooks in the simulation.

To clean up compiled files:
```sh
make clean
```

## Dependencies
This program requires:
- GCC (for compiling C code with `-pthread` support)
- POSIX Threads (`pthread.h`)
- System headers (`unistd.h`, `sys/types.h`, `sys/socket.h`, etc.)
- GNU Make (for building the project)

## Contributing
If you want to contribute, feel free to open a pull request or create an issue for any suggestions and improvements.

