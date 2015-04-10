TARGET=matasano.bin
SRC=main.c matasano.c hex2base64.c fixedXor.c singleCharXor.c detectSingleCharXor.c repeatingKeyXor.c hammingDistance.c challenge6.c
CC=gcc
RANLIB=ranlib
OBJ=$(SRC:.c=.o)
WARNINGS=-Wall
INC=-I.
DEBUG=1
LNKFLAG=-lm

ifeq ($(DEBUG),1)
OPTIMIZE=-O0 -g
else
OPTIMIZE=-O3
endif

CFLAGS=$(OPTIMIZE) $(WARNINGS) $(INC) -MD -MP

all: build

build: $(TARGET) 

rebuild: clean build

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)

$(TARGET): $(OBJ) 
	$(CC) -o $(TARGET) $(OBJ) $(LNKFLAG)

%.o: %c $(SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(SRC:%.c=%.d)

