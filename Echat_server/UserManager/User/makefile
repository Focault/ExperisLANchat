TARGET= UserTests.out
CC= gcc
CFLAGS= -ansi -g -pedantic -I $(DH)  -I ../../../CommunicationProtocol
OBJS=  User.o UserTests.o
DH= ../../../DLIB/DLIB-H/
LIB= -Wl,-rpath=../../../DLIB/ -L../../../DLIB/ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

UserTests.o: UserTests.c User.h 
	$(CC) $(CFLAGS) -c UserTests.c


User.o: User.c User.h  	
	$(CC) $(CFLAGS) -c User.c



clean:
	rm -f $(TARGET) $(OBJS)
