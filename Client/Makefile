CC=gcc
CFLAGS=-std=c99 -W -Wall -pedantic 
LDFLAGS=-I../library/include -L../library/lib/x64 -lSDL2 -lSDL2_image -lSDL2_ttf -lm -lc -lpthread -l:libfmod.so
EXEC=WarGameClient
DEBUGEXEC=WarGameClient.debug
SRC=main.c common.c ft_configuration.c ft_explode.c ft_menu.c ft_socket.c ft_view.c ft_bullet.c ft_engine.c ft_map.c ft_sdl.c ft_sound.c ft_nearwall.c pb_decode.c pb_functions.c ft_playerPosition.c ft_delay.c ft_item.c ft_chat.c ini.c pb_common.c pb_encode.c unionproto.pb.c
OBJDIR=obj
OBJ=$(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

all: $(EXEC)

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(DEBUGEXEC) $(LDFLAGS)
WarGameClient: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJDIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
clean:
	rm -f $(OBJ)
distclean: clean
	rm -f $(EXEC)
