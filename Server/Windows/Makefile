CC=gcc
CFLAGS=-std=c99 -W -Wall -pedantic 
LDFLAGS=-I../../library/include -L../../library/lib/x64 -lSDL2 -lpthread
EXEC=WarGameServer
DEBUGEXEC=WarGameServer.debug
SRC=main.c common.c ft_map.c ft_checkcollision.c ft_disconnect.c ft_client.c pb_common.c pb_decode.c pb_encode.c pb_functions.c unionproto.pb.c
OBJDIR=obj
OBJ=$(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

all: $(EXEC)
debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(DEBUGEXEC) $(LDFLAGS)
WarGameServer: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJDIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
clean:
	rm -f $(OBJ)
distclean: clean
	rm -f $(EXEC)
