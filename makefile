IDIR = ./inc

CC = gcc
CFLAGS = -std=c11 -I$(IDIR) -Wall

SDIR = ./src
BUILD = ./build
BDIR = $(BUILD)/bin
ODIR = $(BUILD)/obj

_DEPS = aes-crack.h aes-sbox
DEPS = $(patsubt %,$(IDIR)/%,$(_DEPS))

_OBJ_dfa = dfa.o aes-crack.o aes-sbox.o
OBJ_dfa = $(patsubst %,$(ODIR)/%,$(_OBJ_dfa))

$(shell mkdir -p $(BUILD))
$(shell mkdir -p $(BDIR))
$(shell mkdir -p $(ODIR))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: dfa

dfa: $(OBJ_dfa)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	@rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~