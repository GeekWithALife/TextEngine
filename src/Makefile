CC = g++
CFLAGS = -g -Wall
LFLAGS = -lncurses
OBJ_FILE = main.o TextObject.o TextEngine.o
EXE_FILE = test

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${CFLAGS} ${LFLAGS} -o ${EXE_FILE} ${OBJ_FILE}

.cpp.o:
	${CC} -c ${CFLAGS} -o "$@" "$<"

main.o: main.cpp
TextObject.o: TextObject.cpp
TextEngine.o: TextEngine.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

