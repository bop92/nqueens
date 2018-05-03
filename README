The 8 queens puzzle asks how 8 queens can be positioned on an 8x8 chess board such that no two queens threaten each other.

The N queens problem is the abstraction of that to include a NxN chess board with N number of queens.

This is a multithreaded client sever program which splits the problem between the available client list. It reads in a provided file to obtain a list of ip address which have servers waiting for requsets. The client then creates a thread which issues a calculation request to the server. The server creates a thread to handle the request and sends it's data back to the client thread when finished. The client waits for all threads to send back their data and prints out the total number of permutations found.

The client and server make use of functions found in the uici libraries provided to set up and breakdown the network connections. The file restart.c contains thread safe read and write functions among other things.

The output of this program will be a string of numbers as well as a result line and some other control information. The numbers correlate to the rows and columns of a chess board.

to compile type:

make

and make sure that you have the pthread library.
