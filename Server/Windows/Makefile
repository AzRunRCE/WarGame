CC=gcc
CFLAGS=-W -Wall -pedantic
LDFLAGS=-Ilibrary/include
EXEC=WarGameServer
SRC=main.c common.c pb_common.c pb_decode.c pb_encode.c pb_functions.c unionproto.pb.c
OBJDIR=obj
OBJ := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

all: $(EXEC)

WarGameServer: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJDIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
clean:
	rm -f $(OBJ)
distclean: clean
	rm -f $(EXEC)