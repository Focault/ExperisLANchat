TARGET= TCPserver.out
CC= gcc
CFLAGS= -ansi -g3 -pedantic -Wall -I $(DH)
OBJS= TCPserver.o main.o
DH= ../../../DLIB/DLIB-H/
LIB= -Wl,-rpath=../../../DLIB/ -L../../../DLIB/ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

main.o: main.c TCPserver.h
	$(CC) $(CFLAGS) -c main.c

TCPserver.o: TCPserver.c TCPserver.h $(DH)ListItr.h $(DH)GenListAPI.h $(DH)ListFunctions.h
	$(CC) $(CFLAGS) -c TCPserver.c

clean:
	rm -f $(TARGET) $(OBJS)