CC = gcc
CFLAGS =
LIBS =

CROSS_COMPILE=

EXE = writer
SRC = writer.c

OBJ = $(SRC: .c=.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(EXE)

.PHONY: clean

clean:
	$(RM) *.o *~ $(EXE)