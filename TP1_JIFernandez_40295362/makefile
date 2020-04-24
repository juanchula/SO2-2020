# Directorios
CLIENT_SOURCE=cliente
SERVER_SOURCE=server
BINARY_DIR=bin
#Binarios
CLIENT=client
SERVER=server
SERVER_FS=fileserv
SERVER_AUTH=auth
SERVER_SHAREDFUN=sharedfun

# Opt de compilacion 
CC=gcc
CFLAGS=-std=gnu11  -Wall -Werror -pedantic -Wextra -Wconversion -O1
LDFLAGS=-lrt -lssl -lcrypto

#Doxigen
DOX=doxygen
HTML_DIR=html
LATEX_DIR=latex
all : client server docs

client : $(CLIENT_SOURCE)/main.c
	mkdir -p $(BINARY_DIR)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(CLIENT).o -c $(CLIENT_SOURCE)/main.c $(LDFLAGS)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(CLIENT)   $(BINARY_DIR)/$(CLIENT).o $(LDFLAGS)

server : $(SERVER_SOURCE)/sharedfun.c $(SERVER_SOURCE)/fileserv.c $(SERVER_SOURCE)/auth.c $(SERVER_SOURCE)/server.c
	mkdir -p $(BINARY_DIR)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(SERVER_SHAREDFUN).o -c $(SERVER_SOURCE)/sharedfun.c $(LDFLAGS)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(SERVER_FS).o -c $(SERVER_SOURCE)/fileserv.c $(LDFLAGS)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(SERVER_AUTH).o -c $(SERVER_SOURCE)/auth.c $(LDFLAGS)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(SERVER).o -c $(SERVER_SOURCE)/server.c $(LDFLAGS)
	$(CC) $(CFLAGS)  -o $(BINARY_DIR)/$(SERVER)  $(BINARY_DIR)/$(SERVER_SHAREDFUN).o $(BINARY_DIR)/$(SERVER_FS).o $(BINARY_DIR)/$(SERVER_AUTH).o $(BINARY_DIR)/$(SERVER).o   $(LDFLAGS)

docs:
	$(DOX) documentation

.PHONY: clean
clean :
	rm  -Rf $(BINARY_DIR)
	rm  -Rf $(HTML_DIR)
	rm  -Rf $(LATEX_DIR)

delqueue:
	rm	/dev/mqueue/auth_service
	rm	/dev/mqueue/file_service
