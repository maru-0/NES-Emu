IDIR =./src
CC=gcc
CFLAGS=-I$(IDIR) -I/usr/include/SDL2 -D_REENTRANT -pthread -lSDL2 -g

ODIR=src

_DEPS = cpu.h bus.h ppu_2C02.h mappers.h cartridge.h rendering.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


_OBJ = main.o cpu.o bus.o ppu_2C02.o mappers.o cartridge.o rendering.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@ $(CC) -g -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	 $(CC) -o $@ $^ $(CFLAGS)

clean:
	@ rm -f $(ODIR)/*.o