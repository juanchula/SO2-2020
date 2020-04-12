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
