CC=gcc
CFLAGS=-W -Wall -pedantic 
LDFLAGS=-Ilibrary/include -Llibrary/lib/x64 -lSDL2
EXEC=WarGameServer
#SRC=main.c common.c pb_common.c pb_decode.c pb_encode.c pb_functions.c unionproto.pb.c ft_map.c
SRC=main.c common.c ft_map.c pb_common.c pb_decode.c pb_encode.c pb_functions.c unionproto.pb.c
OBJDIR=obj
OBJ=$(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

all: $(EXEC)

WarGameServer: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJDIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
clean:
	rm -f $(OBJ)
distclean: clean
	rm -f $(EXEC)
