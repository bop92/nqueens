ll: server client

server: queens_server.c queens.h nqueens_library.c uici.c uici.h uiciname.c uiciname.h restart.h restart.c 
	cc -pthread -o server queens_server.c queens.h nqueens_library.c uici.c uici.h uiciname.c uiciname.h restart.h restart.c 

client: queens_client.c queens.h nqueens_library.c uici.c uici.h uiciname.c uiciname.h restart.h restart.c copy2files.c
	cc -pthread -o client queens_client.c queens.h nqueens_library.c uici.c uici.h uiciname.c uiciname.h restart.h restart.c copy2files.c


