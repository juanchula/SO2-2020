# Directorios
CLIENT_SOURCE=cliente
SERVER_SOURCE=server
BINARY_DIR=bin
#Binarios
CLIENT=client
SERVER=server
SERVER_FS=fileserv
SERVER_AUTH=auth

# Opt de compilacion 
CC=gcc
CFLAGS=-std=gnu11  -Wall -Werror -pedantic -Wextra -Wconversion

all : client server

client : $(CLIENT_SOURCE)/main.c
	mkdir -p $(BINARY_DIR)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(CLIENT).o -c $(CLIENT_SOURCE)/main.c 
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(CLIENT)   $(BINARY_DIR)/$(CLIENT).o 

server : $(SERVER_SOURCE)/server.c $(SERVER_SOURCE)/common_structs.h $(SERVER_SOURCE)/fileserv.c $(SERVER_SOURCE)/auth.c


#Macros
CC=gcc
CFLAGS = -Wall -Werror -pedantic -Wextra -Wconversion -std=gnu11 -lrt -lssl -lcrypto 
FLOUT = rdproc
FL = main.c
FLS = filetools.c tp1a.c tp1b.c tp1c.c tp1d.c
RM = rm


clean:
	$(RM) $(OBJ)

all:
	$(CC) $(CFLAGS) -o $(FLOUT) $(FLS) $(FL)
