.POSIX:

include config.mk
SRC = bindiff.c
OBJ = $(SRC:.c=.o)

all: bindiff

.c.o:
	${CC} ${CFLAGS} -c $<

bindiff.o: defs.h
${OBJ}: config.mk



bindiff: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS}

clean:
	rm -f bindiff ${OBJ}

