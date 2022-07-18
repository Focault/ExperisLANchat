TARGET= GroupTests.out
CC= gcc
CFLAGS= -ansi -g -pedantic   -I ../../../CommunicationProtocol
OBJS=  Group.o GroupTests.o


$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

UserTests.o: GroupTests.c Group.h 
	$(CC) $(CFLAGS) -c GroupTests.c


User.o: Group.c Group.h  	
	$(CC) $(CFLAGS) -c Group.c



clean:
	rm -f $(TARGET) $(OBJS)
